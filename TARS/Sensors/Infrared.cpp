#include "Infrared.h"

/** @copydoc TARS_Infrared::TARS_Infrared */
TARS_Infrared::TARS_Infrared(uint8_t leftPin, uint8_t rightPin)
    : _leftPin(leftPin),
      _rightPin(rightPin) {}

/** @copydoc TARS_Infrared::begin */
void TARS_Infrared::begin() {
    pinMode(_leftPin, INPUT);
    pinMode(_rightPin, INPUT);
}

/** @copydoc TARS_Infrared::readLeft */
bool TARS_Infrared::readLeft() const {
    return digitalRead(_leftPin) == HIGH;
}

/** @copydoc TARS_Infrared::readRight */
bool TARS_Infrared::readRight() const {
    return digitalRead(_rightPin) == HIGH;
}