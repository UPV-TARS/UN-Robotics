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
        TARS_Ultrasonic(uint8_t triggerPin, uint8_t echoPin);

        /**
         * @brief Inicializa los pines del sensor.
         *
         * Configura TRIG como salida y ECHO como entrada.
         */
        void begin();

        /**
         * @brief Lee la distancia estimada en centimetros.
         * @return Distancia en cm. Puede devolver 0 si hay timeout de lectura.
         */
        float readDistanceCM();

    private:
        uint8_t _triggerPin;
        uint8_t _echoPin;
};