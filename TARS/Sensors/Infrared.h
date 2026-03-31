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
         * @param leftPin Pin digital conectado al sensor izquierdo.
         * @param rightPin Pin digital conectado al sensor derecho.
         */
                TARS_Infrared(uint8_t leftPin, uint8_t rightPin)
                        : _leftPin(leftPin),
                            _rightPin(rightPin) {}

        /**
         * @brief Inicializa los pines de los sensores como entrada.
         */
        void begin() {
            pinMode(_leftPin, INPUT);
            pinMode(_rightPin, INPUT);
        }

        /**
         * @brief Lee el estado del sensor infrarrojo izquierdo.
         * @return true si la lectura digital del pin es HIGH; false en caso contrario.
         */
        bool readLeft() const {
            return digitalRead(_leftPin) == HIGH;
        }

        /**
         * @brief Lee el estado del sensor infrarrojo derecho.
         * @return true si la lectura digital del pin es HIGH; false en caso contrario.
         */
        bool readRight() const {
            return digitalRead(_rightPin) == HIGH;
        }

    private:
        /// @brief Pin digital del sensor infrarrojo izquierdo.
        uint8_t _leftPin;

        /// @brief Pin digital del sensor infrarrojo derecho.
        uint8_t _rightPin;
};
