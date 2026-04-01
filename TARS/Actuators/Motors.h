#pragma once

#include <Arduino.h>

/**
 * @brief Controlador de motores diferenciales para TARS.
 *
 * Maneja dos canales de motor (izquierdo y derecho) mediante pines IN1/IN2
 * para dirección y un pin EN para velocidad por PWM.
 */
class TARS_Motors {
    public:
        /**
         * @brief Construye el controlador de motores.
         * @param leftIN1Pin Pin IN1 del motor izquierdo.
         * @param leftIN2Pin Pin IN2 del motor izquierdo.
         * @param leftENPin Pin EN (PWM) del motor izquierdo.
         * @param rightIN1Pin Pin IN1 del motor derecho.
         * @param rightIN2Pin Pin IN2 del motor derecho.
         * @param rightENPin Pin EN (PWM) del motor derecho.
         */
                TARS_Motors(uint8_t leftIN1Pin, uint8_t leftIN2Pin, uint8_t leftENPin, uint8_t rightIN1Pin, uint8_t rightIN2Pin, uint8_t rightENPin)
                        : _leftIN1Pin(leftIN1Pin),
                            _leftIN2Pin(leftIN2Pin),
                            _leftENPin(leftENPin),
                            _rightIN1Pin(rightIN1Pin),
                            _rightIN2Pin(rightIN2Pin),
                            _rightENPin(rightENPin) {}

        /**
         * @brief Inicializa los pines de control de ambos motores.
         */
        void begin() {
            pinMode(_leftIN1Pin, OUTPUT);
            pinMode(_leftIN2Pin, OUTPUT);

            pinMode(_rightIN1Pin, OUTPUT);
            pinMode(_rightIN2Pin, OUTPUT);

            // Configura los canales PWM para ESP32.
            ledcAttach(_leftENPin, 1000, 8);   // Pin PWM izquierdo: 5kHz, 8-bit
            ledcAttach(_rightENPin, 1000, 8);  // Pin PWM derecho: 5kHz, 8-bit

            // Arranca en estado seguro (motores detenidos).
            set(0, 0);
        }

        /**
         * @brief Aplica una señal PWM a cada motor.
         * @param pwmLeft Valor PWM para el motor izquierdo. Positivo = adelante, negativo = atrás. Rango: [-255, 255].
         * @param pwmRight Valor PWM para el motor derecho. Positivo = adelante, negativo = atrás. Rango: [-255, 255].
         */
        void set(int16_t pwmLeft, int16_t pwmRight) {
            _setMotor(_leftIN1Pin, _leftIN2Pin, _leftENPin, pwmLeft);
            _setMotor(_rightIN1Pin, _rightIN2Pin, _rightENPin, pwmRight);
        }

    private:
        /** @brief Pines de control del motor izquierdo (IN1, IN2, EN). */
        uint8_t _leftIN1Pin, _leftIN2Pin, _leftENPin;

        /** @brief Pines de control del motor derecho (IN1, IN2, EN). */
        uint8_t _rightIN1Pin, _rightIN2Pin, _rightENPin;

        /**
         * @brief Configura dirección y velocidad de un motor individual.
         * @param in1Pin Pin IN1 del motor.
         * @param in2Pin Pin IN2 del motor.
         * @param enPin Pin EN (PWM) del motor.
         * @param pwm Valor PWM a aplicar. Positivo = adelante, negativo = atrás. Rango absoluto: [0, 255].
         */
        void _setMotor(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin, int16_t pwm) {
            // Ajusta direccion y limita el duty PWM al rango valido [0, 255].
            uint8_t duty = 0;

            if (pwm > 0) {
                // Adelante: IN1 alto, IN2 bajo.
                digitalWrite(in1Pin, HIGH);
                digitalWrite(in2Pin, LOW);
                duty = pwm > 255 ? 255 : static_cast<uint8_t>(pwm);
            } else if (pwm < 0) {
                // Atras: IN1 bajo, IN2 alto.
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
};
