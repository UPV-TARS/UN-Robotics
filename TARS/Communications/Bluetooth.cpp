#include "Bluetooth.h"

#include <cmath>
#include <cstdlib>
#include <cstring>

TARS_Bluetooth::TARS_Bluetooth(uint8_t minPWM)
    : _minPWM(minPWM) {}

void TARS_Bluetooth::begin(const char* deviceName) {
    _bt.begin(deviceName);
}

bool TARS_Bluetooth::update() {
    while (_bt.available()) {
        const char c = static_cast<char>(_bt.read());

        if (c == 'V') {
            _bufferIndex = 0;
            _receiving = true;
        } else if (c == 'F' && _receiving) {
            _receiving = false;
            _buffer[_bufferIndex] = '\0';  // Cierra la cadena para parseo.
            return _parse(_buffer);
        } else if (_receiving) {
            if (_bufferIndex < kBufferSize - 1) {
                _buffer[_bufferIndex++] = c;
            } else {
                // Evita crecimiento indefinido de la trama.
                _bufferIndex = 0;
                _receiving = false;
            }
        }
    }

    return false;
}

bool TARS_Bluetooth::isConnected() const {
    return _bt.connected();
}

void TARS_Bluetooth::joystickToPWM(float x, float y, int16_t minPWM) {
    const int16_t min = minPWM < 0 ? static_cast<int16_t>(_minPWM) : minPWM;
    const float clampedX = constrain(x, -100.0f, 100.0f);
    const float clampedY = constrain(y, -100.0f, 100.0f);

    const int16_t scaledX = static_cast<int16_t>(std::lroundf((clampedX / 100.0f) * 255.0f));
    const int16_t scaledY = static_cast<int16_t>(std::lroundf((clampedY / 100.0f) * 255.0f));

    const int16_t left = static_cast<int16_t>(constrain(static_cast<int>(scaledY + scaledX), -255, 255));
    const int16_t right = static_cast<int16_t>(constrain(static_cast<int>(scaledY - scaledX), -255, 255));

    pwm1 = _applyMinPWM(left, min);
    pwm2 = _applyMinPWM(right, min);
}

int16_t TARS_Bluetooth::_applyMinPWM(int16_t pwm, int16_t minPWM) const {
    if (pwm == 0) {
        return 0;
    }
    const int sign = pwm > 0 ? 1 : -1;
    const int magnitude = max(static_cast<int>(std::abs(pwm)), static_cast<int>(minPWM));
    return static_cast<int16_t>(sign * magnitude);
}

bool TARS_Bluetooth::_parse(const char* data) {
    // Busca la coma separadora.
    const char* comma = std::strchr(data, ',');
    if (comma == nullptr || comma == data) {
        return false;  // No hay coma o esta al inicio.
    }

    // Verifica que haya datos despues de la coma.
    if (comma[1] == '\0') {
        return false;  // Nada despues de la coma.
    }

    // Convierte X (antes de la coma) a float.
    char* endX = nullptr;
    const float x = std::strtof(data, &endX);
    if (endX != comma) {
        return false;  // Error en parsing de X.
    }

    // Convierte Y (despues de la coma) a float.
    const float y = std::strtof(comma + 1, nullptr);

    joystickToPWM(x, y);
    return true;
}
