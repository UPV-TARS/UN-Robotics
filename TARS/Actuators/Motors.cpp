#include "Motors.h"

TARS_Motors::TARS_Motors(
    uint8_t leftIN1Pin,
    uint8_t leftIN2Pin,
    uint8_t leftENPin,
    uint8_t rightIN1Pin,
    uint8_t rightIN2Pin,
    uint8_t rightENPin
)
    : _leftIN1Pin(leftIN1Pin),
      _leftIN2Pin(leftIN2Pin),
      _leftENPin(leftENPin),
      _rightIN1Pin(rightIN1Pin),
      _rightIN2Pin(rightIN2Pin),
      _rightENPin(rightENPin) {}

void TARS_Motors::begin() {
    pinMode(_leftIN1Pin, OUTPUT);
    pinMode(_leftIN2Pin, OUTPUT);
    pinMode(_leftENPin, OUTPUT);

    pinMode(_rightIN1Pin, OUTPUT);
    pinMode(_rightIN2Pin, OUTPUT);
    pinMode(_rightENPin, OUTPUT);

    // Arranca en estado seguro (motores detenidos).
    set(0, 0);
}

void TARS_Motors::set(int16_t pwmLeft, int16_t pwmRight) {
    _setMotor(_leftIN1Pin, _leftIN2Pin, _leftENPin, pwmLeft);
    _setMotor(_rightIN1Pin, _rightIN2Pin, _rightENPin, pwmRight);
}

void TARS_Motors::_setMotor(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin, int16_t pwm) {
    // Saturar a [-255, 255] y obtener valor absoluto.
    uint8_t duty = 0;
    
    if (pwm > 0) {
        // Adelante: IN1 alto, IN2 bajo.
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, LOW);
        duty = pwm > 255 ? 255 : static_cast<uint8_t>(pwm);
    } else if (pwm < 0) {
        // Atrás: IN1 bajo, IN2 alto.
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, HIGH);
        duty = (-pwm) > 255 ? 255 : static_cast<uint8_t>(-pwm);
    } else {
        // Parado: ambos bajos.
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);
        duty = 0;
    }
    
    ledcWrite(enPin, duty);
}
