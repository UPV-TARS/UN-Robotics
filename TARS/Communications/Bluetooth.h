#pragma once

#include <Arduino.h>
#include <BluetoothSerial.h>

/** @brief Tamaño máximo del buffer interno (optimizado para "x,y" con rango [-100,100]). */
#define TARS_BT_BUFFER_SIZE 32

/**
 * @brief Canal Bluetooth para control remoto de TARS.
 *
 * Recibe tramas del mando (formato Vx,yF), convierte joystick a PWM
 * diferencial y expone el resultado en pwm1/pwm2.
 */
class TARS_Bluetooth {
    public:
        /** @brief PWM izquierdo del ultimo paquete valido. */
        int16_t pwm1 = 0;

        /** @brief PWM derecho del ultimo paquete valido. */
        int16_t pwm2 = 0;

        /**
         * @brief Construye el controlador Bluetooth.
         * @param minPWM PWM minimo aplicado cuando hay comando distinto de cero.
         */
        explicit TARS_Bluetooth(uint8_t minPWM = 80);

        /**
         * @brief Inicializa Bluetooth con el nombre del dispositivo.
         * @param deviceName Nombre visible por Bluetooth.
         */
        void begin(const char* deviceName = "TARS");

        /**
         * @brief Procesa bytes entrantes y actualiza pwm1/pwm2 si llega una trama completa.
         * @return true si se parseo una trama valida.
         */
        bool update();

        /**
         * @brief Indica si hay cliente Bluetooth conectado.
         */
        bool isConnected() const;

        /**
         * @brief Convierte posicion de joystick a PWM diferencial y lo guarda en pwm1/pwm2.
         * @param x Eje X en rango esperado [-100, 100].
         * @param y Eje Y en rango esperado [-100, 100].
         * @param minPWM PWM minimo en comandos no nulos.
         */
        void joystickToPWM(float x, float y, int16_t minPWM = -1);

    private:
        BluetoothSerial _bt;
        char _buffer[TARS_BT_BUFFER_SIZE];
        uint8_t _bufferIndex = 0;
        bool _receiving = false;
        uint8_t _minPWM = 80;

        int16_t _applyMinPWM(int16_t pwm, int16_t minPWM) const;
        bool _parse(const char* data);
};