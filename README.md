# UN-Robotics

Libreria educativa para controlar robots con ESP32 de forma sencilla en clase. Esta guia esta pensada para alumnado y profesorado: instalar, conectar componentes y empezar a programar rapido.

## Que incluye

- Control de motores DC y servo.
- Lectura de sensores ultrasonico, infrarrojo, magnetico y RGB.
- Comunicacion por Bluetooth.
- Ejemplos listos para usar en Arduino IDE.

## Uso rapido

Solo necesitas importar una linea:

```cpp
#include <TARS.h>
```

Con eso tienes disponibles todos los componentes de la libreria.

## Instalación

### Requerimientos
- Arduino IDE 1.8.0 o superior.
- Placa ESP32 (testeado en DOIT ESP32 DevKit V1).

### Pasos
1. Descarga este repositorio como ZIP.
2. En Arduino IDE: **Programa > Incluir librería > Añadir biblioteca .ZIP**.
3. Selecciona el ZIP descargado.

### Dependencias necesarias
Instala estas librerías desde **Programa > Incluir librería > Gestionar bibliotecas**:
- `ESP32Servo`
- `Adafruit TCS34725`
- `Adafruit BusIO`

## Componentes soportados

| Componente | Clase | Método principal |
|-----------|-------|------------------|
| Servo (microservo) | `TARS_Servo` | `moveAngle(angle)` |
| Motores DC | `TARS_Motors` | `set(pwm_izq, pwm_der)` |
| Sensor Ultrasónico | `TARS_Ultrasonic` | `readDistanceCM()` |
| Sensor Infrarrojo | `TARS_Infrared` | `readLeft()` / `readRight()` |
| Sensor RGB | `TARS_RGBSensor` | `readColorName()` |
| Sensor Magnético | `TARS_Magnetic` | `isMagnetDetected()` |
| Bluetooth | `TARS_Bluetooth` | `update()` |

## Ejemplos rápidos

### Servo
```cpp
#include <TARS.h>

TARS_Servo servo(14);  // GPIO 14

void setup() {
  servo.begin(90);  // Posición inicial 90°
}

void loop() {
  servo.moveAngle(0);    // Izquierda
  delay(500);
  servo.moveAngle(180);  // Derecha
  delay(500);
}
```

### Motores
```cpp
#include <TARS.h>

TARS_Motors motors(14, 27, 26, 25, 33, 32);

void setup() {
  motors.begin();
}

void loop() {
  motors.set(200, 200);  // Adelante
  delay(1000);
  motors.set(0, 0);      // Parar
  delay(500);
}
```

### Sensor RGB
```cpp
#include <TARS.h>

TARS_RGBSensor rgb;

void setup() {
  Serial.begin(115200);
  rgb.begin();
}

void loop() {
  Serial.println(rgb.readColorName());
  delay(300);
}
```

## Más información

- Para ejemplos completos, ver la carpeta `examples/`.
- Todos los ejemplos están listos para compilar en Arduino IDE.

