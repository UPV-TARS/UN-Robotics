#include <TARS.h>

TARS_Bluetooth bt;

void setup() {
  Serial.begin(115200);
  bt.begin("TARS");
}

void loop() {
  if (bt.update()) {
    Serial.print("PWM izq: ");
    Serial.print(bt.pwm1);
    Serial.print(" | PWM der: ");
    Serial.println(bt.pwm2);
  }

  delay(10);
}
