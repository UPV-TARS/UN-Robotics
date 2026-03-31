#include "Ultrasonic.h"

/**
 * @copydoc TARS_Ultrasonic::TARS_Ultrasonic(uint8_t, uint8_t)
 */
TARS_Ultrasonic::TARS_Ultrasonic(uint8_t triggerPin, uint8_t echoPin)
    : _triggerPin(triggerPin), _echoPin(echoPin) {}

/**
 * @copydoc TARS_Ultrasonic::begin()
 */
void TARS_Ultrasonic::begin() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    digitalWrite(_triggerPin, LOW);
}

/**
 * @copydoc TARS_Ultrasonic::readDistanceCM()
 *
 * @note Usa un timeout de 30000 us en pulseIn, equivalente a ~5 m maximos
 * de recorrido ida y vuelta en aire.
 */
float TARS_Ultrasonic::readDistanceCM() {
     // Envia pulso de trigger.
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);

     // Mide duracion del pulso de echo.
     const unsigned long duration = pulseIn(_echoPin, HIGH, 30000);  // Timeout de 30 ms.

     // Convierte duracion a distancia (velocidad del sonido ~343 m/s).
    const float distanceCM = (duration / 2.0f) * 0.0343f;

    return distanceCM;
}