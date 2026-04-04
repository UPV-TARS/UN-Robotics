#include <TARS.h>

// Constructor: (pin sensor izquierdo, pin sensor derecho).
TARS_Infrared ir(34, 35);

void setup() {
  Serial.begin(115200);
  ir.begin();
}

void loop() {
  Serial.print("Izq: ");
  Serial.print(ir.readLeft());
  Serial.print(" | Der: ");
  Serial.println(ir.readRight());
  delay(120);
}
