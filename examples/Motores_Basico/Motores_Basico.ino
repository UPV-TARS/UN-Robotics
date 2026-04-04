#include <TARS.h>

// Constructor: (IN1 izq, IN2 izq, EN/PWM izq, IN1 der, IN2 der, EN/PWM der).
TARS_Motors motors(14, 27, 26, 25, 33, 32);

void setup() {
  motors.begin();
}

void loop() {
  motors.set(200, 200);
  delay(1000);

  motors.set(-160, 160);
  delay(600);

  motors.set(-180, -180);
  delay(1000);

  motors.set(0, 0);
  delay(500);
}
