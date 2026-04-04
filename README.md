# UN-Robotics (TARS)

Libreria educativa para ESP32 pensada para clase.

Objetivo: que alumnado y profesorado puedan montar un robot y programarlo con pocas lineas, manteniendo una API clara y facil de recordar.

## 1. Que puedes hacer con esta libreria

- Mover un robot con motores DC.
- Controlar un microservo por angulo.
- Leer sensores de ultrasonidos, infrarrojo, magnetico y RGB.
- Recibir comandos por Bluetooth.

## 2. Instalacion (Arduino IDE)

### Requisitos
- Arduino IDE 1.8.x o superior.
- Placa ESP32 (probado en DOIT ESP32 DevKit V1).

### Instalar esta libreria
1. Ve a la seccion Releases del repositorio y descarga el ZIP de la ultima version.
2. En Arduino IDE entra en Programa > Incluir libreria > Anadir biblioteca .ZIP.
3. Selecciona el ZIP descargado.

Tambien puedes descargarla directamente desde:

https://github.com/UPV-TARS/UN-Robotics/releases/latest/download/TARS.zip

### Instalar dependencias
En Programa > Incluir libreria > Gestionar bibliotecas, instala:
- ESP32Servo
- Adafruit TCS34725
- Adafruit BusIO

## 3. Primer sketch (estructura minima)

```cpp
#include <TARS.h>

void setup() {
}

void loop() {
}
```

Con solo incluir `TARS.h` ya tienes acceso a todos los componentes.

## 4. API rapida para clase

### Motores (`TARS_Motors`)
- Constructor: `TARS_Motors(in1Izq, in2Izq, enIzq, in1Der, in2Der, enDer)`.
- `begin()` inicia pines.
- `set(pwmIzq, pwmDer)` controla ambos motores con PWM.

### Servo (`TARS_Servo`)
- Constructor: `TARS_Servo(pinServo)`.
- `begin(anguloInicial)`.
- `moveAngle(angulo)`.
- `getAngle()`.

### Ultrasonido (`TARS_Ultrasonic`)
- Constructor: `TARS_Ultrasonic(pinTrig, pinEcho)`.
- `begin()`.
- `readDistanceCM()`.

### Infrarrojo (`TARS_Infrared`)
- Constructor: `TARS_Infrared(pinIzq, pinDer)`.
- `begin()`.
- `readLeft()` / `readRight()`.

### Magnetico (`TARS_Magnetic`)
- Constructor: `TARS_Magnetic(pinSensor)`.
- `begin()`.
- `isMagnetDetected()`.

### RGB (`TARS_RGBSensor`)
- Constructor: `TARS_RGBSensor()` (sin parámetros).
- `begin()`.
- `readColorName()` (devuelve el nombre del color detectado).
- `readColorID()` (devuelve el ID del color detectado, útil para lógica de control).
- `readHue()`.

### Bluetooth (`TARS_Bluetooth`)
- Constructor: `TARS_Bluetooth(minPWM)` donde `minPWM` es opcional (por defecto 80).
- `begin("Nombre")`.
- `update()` para procesar la trama recibida.
- `pwm1` y `pwm2` para leer la orden.

## 5. Mapa rapido de parametros (constructores)

- `TARS_Motors`: 6 pines del driver en este orden: izquierda `IN1, IN2, EN` y derecha `IN1, IN2, EN`.
- `TARS_Servo`: 1 pin de senal PWM del servo.
- `TARS_Ultrasonic`: 2 pines, primero `TRIG` y despues `ECHO`.
- `TARS_Infrared`: 2 pines digitales, primero izquierdo y despues derecho.
- `TARS_Magnetic`: 1 pin digital del sensor.
- `TARS_RGBSensor`: sin parámetros, usa I2C por defecto.
- `TARS_Bluetooth`: 1 parametro opcional de ajuste (`minPWM`).

## 6. Ejemplo rapido: mover el robot

```cpp
#include <TARS.h>

TARS_Motors motors(14, 27, 26, 25, 33, 32);

void setup() {
  motors.begin();
}

void loop() {
  motors.set(180, 180);
  delay(1000);

  motors.set(-160, 160);
  delay(500);

  motors.set(0, 0);
  delay(600);
}
```

## 7. Ejemplo rapido: lectura de color

```cpp
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
  Serial.print("Color: ");
  Serial.print(rgb.readColorName());
  Serial.print(" | ID: ");
  Serial.println(rgb.readColorID());
  delay(300);
}
```

## 8. Recomendaciones para docentes

- Empieza por secuencias cortas con `set(pwmIzq, pwmDer)` y tiempos (`delay`).
- Introduce primero lectura de color con `readColorName()`.
- Usa la carpeta `examples/` como punto de partida para cada practica.

## 9. Ejemplos incluidos

- `examples/Bluetooth_Basico`
- `examples/Infrarrojo_Basico`
- `examples/Magnetico_Basico`
- `examples/Motores_Basico`
- `examples/RGB_Basico`
- `examples/Servo_Basico`
- `examples/Ultrasonido_Basico`

