#pragma once

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

/**
 * @brief Canal Bluetooth para control remoto de TARS.
 *
 * Recibe tramas del mando con formato Vx,yF, convierte joystick a PWM
 * diferencial y expone el resultado en pwm1/pwm2.
 */
class TARS_Bluetooth {
    public:
        /** @brief PWM izquierdo del ultimo paquete valido. */
        int16_t pwm1 = 0;

        /** @brief PWM derecho del ultimo paquete valido. */
        int16_t pwm2 = 0;

        /**
         * @brief Construye el controlador Bluetooth.
         * @param minPWM PWM minimo aplicado cuando hay comando distinto de cero.
         */
        explicit TARS_Bluetooth(uint8_t minPWM = 80)
            : _minPWM(minPWM) {}

        /**
         * @brief Inicializa Bluetooth con el nombre del dispositivo.
         * @param deviceName Nombre visible por Bluetooth.
         */
        void begin(const char* deviceName = "TARS") {
            _bt.begin(deviceName);
        }

        /**
         * @brief Procesa bytes entrantes y actualiza pwm1/pwm2 si llega una trama completa.
         * @return true si se parseo una trama valida.
         */
        bool update() {
            while (_bt.available()) {
                const char c = static_cast<char>(_bt.read());

                if (c == 'V') {
                    _bufferIndex = 0;
                    _receiving = true;
                } else if (c == 'F' && _receiving) {
                    _receiving = false;
                    _buffer[_bufferIndex] = '\0';
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

        /**
         * @brief Indica si hay cliente Bluetooth conectado.
         */
        bool isConnected() {
            return _bt.connected();
        }

        /**
         * @brief Convierte posicion de joystick a PWM diferencial y lo guarda en pwm1/pwm2.
         * @param x Eje X en rango esperado [-100, 100].
         * @param y Eje Y en rango esperado [-100, 100].
         * @param minPWM PWM minimo en comandos no nulos.
         */
        void joystickToPWM(float x, float y, int16_t minPWM = -1) {
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

    private:
        static constexpr uint8_t kBufferSize = 32;

        BluetoothSerial _bt;
        char _buffer[kBufferSize];
        uint8_t _bufferIndex = 0;
        bool _receiving = false;
        uint8_t _minPWM = 80;

        int16_t _applyMinPWM(int16_t pwm, int16_t minPWM) const {
            if (pwm == 0) {
                return 0;
            }
            const int sign = pwm > 0 ? 1 : -1;
            const int magnitude = max(static_cast<int>(std::abs(pwm)), static_cast<int>(minPWM));
            return static_cast<int16_t>(sign * magnitude);
        }

        bool _parse(const char* data) {
            const char* comma = std::strchr(data, ',');
            if (comma == nullptr || comma == data) {
                return false;
            }

            if (comma[1] == '\0') {
                return false;
            }

            char* endX = nullptr;
            const float x = std::strtof(data, &endX);
            if (endX != comma) {
                return false;
            }

            const float y = std::strtof(comma + 1, nullptr);
            joystickToPWM(x, y);
            return true;
        }
};