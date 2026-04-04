#include <TARS.h>

// Constructor: (pin de senal PWM del servo).
// Pin D5 en NodeMCU suele mapear a GPIO 14.
TARS_Servo servo(14);

void setup() {
  servo.begin(90);
}

void loop() {
  servo.moveAngle(0);
  delay(700);

  servo.moveAngle(180);
  delay(700);

  servo.moveAngle(90);
  delay(700);
}
