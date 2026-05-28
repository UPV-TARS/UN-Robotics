#include <TARS.h>

TARS_WiFi wifi;

void setup() {
    Serial.begin(115200);
    // Inicializa la red "TARS-Test" con clave "tars1234"
    // La URL saldrá automáticamente en la conexión serie
    wifi.begin("TARS-Test", "tars1234");
}

void loop() {
    if (wifi.update()) {
        // pwm1 y pwm2 ya actualizados automáticamente por la librería
        Serial.print("Motor Izquierdo: ");
        Serial.print(wifi.pwm1);
        Serial.print(" | Motor Derecho: ");
        Serial.println(wifi.pwm2);
    }
}