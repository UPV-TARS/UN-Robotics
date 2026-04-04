#pragma once

#include <Arduino.h>

/**
 * @brief Driver simple para sensor ultrasonico (tipo HC-SR04).
 *
 * Esta clase encapsula el disparo del pulso de trigger y la lectura
 * del tiempo de retorno por echo para estimar distancia en centimetros.
 */
class TARS_Ultrasonic {
    public:
        /**
         * @brief Construye un sensor ultrasonico.
         * @param triggerPin Pin digital conectado a TRIG.
         * @param echoPin Pin digital conectado a ECHO.
         */
                TARS_Ultrasonic(uint8_t triggerPin, uint8_t echoPin)
                        : _triggerPin(triggerPin),
                            _echoPin(echoPin) {}

        /**
         * @brief Inicializa los pines del sensor.
         *
         * Configura TRIG como salida y ECHO como entrada.
         */
        void begin() {
            pinMode(_triggerPin, OUTPUT);
            pinMode(_echoPin, INPUT);
            digitalWrite(_triggerPin, LOW);
        }

        /**
         * @brief Lee la distancia estimada en centimetros.
         * @return Distancia en cm. Puede devolver 0 si hay timeout de lectura.
         */
        float readDistanceCM() {
            // Envia pulso de trigger.
            digitalWrite(_triggerPin, LOW);
            delayMicroseconds(2);
            digitalWrite(_triggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(_triggerPin, LOW);

            // Mide duracion del pulso de echo.
            const unsigned long duration = pulseIn(_echoPin, HIGH, 5800);

            // Convierte duracion a distancia (velocidad del sonido ~343 m/s).
            return (duration / 2.0f) * 0.0343f;
        }

    private:
        /** @brief Pin digital conectado a TRIG. */
        uint8_t _triggerPin;

        /** @brief Pin digital conectado a ECHO. */
        uint8_t _echoPin;
};
