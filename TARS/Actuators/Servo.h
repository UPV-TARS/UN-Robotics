#pragma once

#include <Arduino.h>

#if defined(ESP32)
  #include <ESP32Servo.h>
#else
  #include <Servo.h>
#endif

/**
 * @brief Control simple de microservo para proyectos educativos.
 *
 * Interfaz minima pensada para alumnos:
 * 1) begin()
 * 2) moveAngle(...)
 */
class TARS_Servo {
	public:
		/**
		 * @brief Crea un servo en el pin indicado.
		 * @param pin Pin de senal del servo (por ejemplo D5 en NodeMCU).
		 */
		explicit TARS_Servo(uint8_t pin)
			: _pin(pin),
			  _lastAngle(90) {}

		/**
		 * @brief Inicia el servo en un angulo inicial.
		 * @param initialAngle Angulo inicial (0 a 180).
		 */
		void begin(uint8_t initialAngle = 90) {
			_ensureAttached();
			moveAngle(initialAngle);
		}

		/**
		 * @brief Mueve el servo a un angulo.
		 * @param angle Angulo objetivo en grados.
		 */
		void moveAngle(uint8_t angle) {
			_ensureAttached();
			_lastAngle = _clampAngle(angle);
			_servo.write(_lastAngle);
		}

		/**
		 * @brief Devuelve el ultimo angulo escrito.
		 * @return Ultimo angulo enviado al servo.
		 */
		uint8_t getAngle() const {
			return _lastAngle;
		}

	private:
		uint8_t _pin;
		uint8_t _lastAngle;

		Servo _servo;

		uint8_t _clampAngle(uint8_t angle) const {
			if (angle > 180) {
				return 180;
			}

			return angle;
		}

		void _ensureAttached() {
			if (_servo.attached()) {
				return;
			}

			#if defined(ESP32)
				_servo.attach(_pin, 1000, 2000);
			#else
				_servo.attach(_pin);
			#endif
		}
};
