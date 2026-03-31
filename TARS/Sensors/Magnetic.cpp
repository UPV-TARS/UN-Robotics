#include "Magnetic.h"

/** @copydoc TARS_Magnetic::TARS_Magnetic */
TARS_Magnetic::TARS_Magnetic(uint8_t pin)
    : _pin(pin) {}

/** @copydoc TARS_Magnetic::begin */
void TARS_Magnetic::begin() {
    pinMode(_pin, INPUT);
}

/** @copydoc TARS_Magnetic::isMagnetDetected */
bool TARS_Magnetic::isMagnetDetected() const {
    return digitalRead(_pin) == HIGH;  // Asume sensor activo en HIGH.
}