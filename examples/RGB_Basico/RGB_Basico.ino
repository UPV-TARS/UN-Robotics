#include <TARS.h>

// Constructor sin parametros (usa I2C por defecto).
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
  // Lectura simple para clase: pedir solo el color detectado.
  Serial.print("Color detectado: ");
  Serial.print(rgb.readColorName());
  Serial.print(" (ID: ");
  Serial.print(rgb.readColorID());
  Serial.println(")");

  Serial.print("Hue: ");
  Serial.println(rgb.readHue());

  delay(200);
}
