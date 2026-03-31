#include <TARS.h>

// Ajusta estos pines a tu driver de motores.
TARS_Motors motors(14, 27, 26, 25, 33, 32);

void setup() {
  motors.begin();
}

void loop() {
  motors.set(140, 140);
  delay(1000);

  motors.set(0, 0);
  delay(500);

  motors.set(-140, -140);
  delay(1000);

  motors.set(0, 0);
  delay(1000);
}
