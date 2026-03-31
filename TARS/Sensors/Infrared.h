#pragma once

#include <Arduino.h>

/**
 * @brief Clase para gestionar dos sensores infrarrojos digitales.
 *
 * Permite inicializar los pines de entrada y leer el estado de los
 * sensores izquierdo y derecho.
 */
class TARS_Infrared {
    public:
        /**
         * @brief Crea una instancia de sensores infrarrojos.
         * @param Leftpin Pin digital conectado al sensor izquierdo.
         * @param Rightpin Pin digital conectado al sensor derecho.
         */
        TARS_Infrared(uint8_t Leftpin, uint8_t Rightpin);

        /**
         * @brief Inicializa los pines de los sensores como entrada.
         */
        void begin();

        /**
         * @brief Lee el estado del sensor infrarrojo izquierdo.
         * @return `true` si la lectura digital del pin es `HIGH`; en caso contrario, `false`.
         */
        bool readLeft() const;

        /**
         * @brief Lee el estado del sensor infrarrojo derecho.
         * @return `true` si la lectura digital del pin es `HIGH`; en caso contrario, `false`.
         */
        bool readRight() const;

    private:
        /// @brief Pin digital del sensor infrarrojo izquierdo.
        uint8_t _Leftpin;

        /// @brief Pin digital del sensor infrarrojo derecho.
        uint8_t _Rightpin;
};
