#include <TARS.h>

TARS_RGBSensor rgb;

void setup() {
  Serial.begin(115200);

  if (!rgb.begin()) {
    Serial.println("No se encontro el sensor RGB");
    while (true) {
      delay(1000);
    }
  }
}

void loop() {
  Serial.print("Hue: ");
  Serial.print(rgb.readHue());
  Serial.print(" | Color: ");
  Serial.println(rgb.readColorName());
  delay(300);
}
