#include <TARS.h>

// Constructor: (pin TRIG, pin ECHO).
TARS_Ultrasonic ultra(5, 18);

void setup() {
  Serial.begin(115200);
  ultra.begin();
}

void loop() {
  float cm = ultra.readDistanceCM();
  Serial.print("Distancia (cm): ");
  Serial.println(cm);
  delay(250);
}
