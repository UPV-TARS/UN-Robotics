#include <TARS.h>

TARS_Magnetic magnetic(4);

void setup() {
  Serial.begin(115200);
  magnetic.begin();
}

void loop() {
  if (magnetic.isMagnetDetected()) {
    Serial.println("Iman detectado");
  } else {
    Serial.println("Sin iman");
  }

  delay(200);
}
