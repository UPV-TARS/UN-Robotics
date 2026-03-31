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
        TARS_Motors(uint8_t leftIN1Pin, uint8_t leftIN2Pin, uint8_t leftENPin, uint8_t rightIN1Pin, uint8_t rightIN2Pin, uint8_t rightENPin);

        /**
         * @brief Inicializa los pines de control de ambos motores.
         */
        void begin();

        /**
         * @brief Aplica una señal PWM a cada motor.
         * @param pwmLeft Valor PWM para el motor izquierdo. Positivo = adelante, negativo = atrás. Rango: [-255, 255].
         * @param pwmRight Valor PWM para el motor derecho. Positivo = adelante, negativo = atrás. Rango: [-255, 255].
         */
        void set(int16_t pwmLeft, int16_t pwmRight);

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
        void _setMotor(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin, int16_t pwm);
};