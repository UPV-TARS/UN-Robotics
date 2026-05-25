#include <TARS.h>

// minPWM opcional (80 por defecto).
TARS_WiFi wifi;

void setup() {
    Serial.begin(115200);
    wifi.begin("TARS-Test", "tars1234");
}

void loop() {
    if (wifi.update()) {
        // pwm1 y pwm2 ya actualizados
        Serial.print("PWM izq: ");
        Serial.print(wifi.pwm1);
        Serial.print(" | PWM der: ");
        Serial.println(wifi.pwm2);
    }
}