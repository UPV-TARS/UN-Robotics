#include "Infrared.h"

/** @copydoc TARS_Infrared::TARS_Infrared */
TARS_Infrared::TARS_Infrared(uint8_t Leftpin, uint8_t Rightpin):
    _Leftpin(Leftpin),
    _Rightpin(Rightpin)
{}

/** @copydoc TARS_Infrared::begin */
void TARS_Infrared::begin() {
    pinMode(_Leftpin, INPUT);
    pinMode(_Rightpin, INPUT);
}

/** @copydoc TARS_Infrared::readLeft */
bool TARS_Infrared::readLeft() const {
    return digitalRead(_Leftpin) == HIGH;
}

/** @copydoc TARS_Infrared::readRight */
bool TARS_Infrared::readRight() const {
    return digitalRead(_Rightpin) == HIGH;
}