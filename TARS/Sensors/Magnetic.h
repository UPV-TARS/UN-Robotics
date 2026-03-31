#pragma once

#include <Arduino.h>

/**
 * @brief Sensor magnetico digital para deteccion de campo magnetico.
 *
 * La lectura es booleana y depende del nivel logico entregado por el modulo.
 */
class TARS_Magnetic {
    public:
        /**
         * @brief Crea una instancia del sensor magnetico.
         * @param pin Pin digital conectado a la salida del sensor.
         */
        explicit TARS_Magnetic(uint8_t pin);

        /**
         * @brief Configura el pin del sensor.
         */
        void begin();

        /**
         * @brief Indica si el iman fue detectado.
         * @return true si el pin del sensor esta en HIGH; false en caso contrario.
         */
        bool isMagnetDetected() const;

    private:
        /**< Pin digital de entrada del sensor magnetico. */
        uint8_t _pin;
};