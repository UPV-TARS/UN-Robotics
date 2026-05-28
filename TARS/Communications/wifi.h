#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <cmath>
#include <cstdlib>
#include <cstring>

/**
 * @brief Canal WiFi para control remoto de TARS via joystick web.
 *
 * Levanta un Access Point, sirve una pagina HTML con joystick tactil
 * y recibe comandos por WebSocket en formato JSON {"x": float, "y": float}
 * con rango [-1, 1]. Convierte a PWM diferencial y expone el resultado
 * en pwm1/pwm2.
 * 
 * @note Solo debe existir una instancia activa a la vez debido a la 
 * naturaleza estatica del controlador de eventos WebSocket. 
 */
class TARS_WiFi {
    public:
        /** @brief PWM izquierdo del ultimo paquete valido en rango [-255, 255]. */
        int16_t pwm1 = 0;

        /** @brief PWM derecho del ultimo paquete valido en rango [-255, 255]. */
        int16_t pwm2 = 0;

        /**
         * @brief Construye el controlador WiFi.
         * @param minPWM PWM minimo aplicado cuando hay comando distinto de cero.
         */
        explicit TARS_WiFi(uint8_t minPWM = 80)
            : _minPWM(minPWM), _server(80), _webSocket(81) {}

        ~TARS_WiFi() {
            if (_getInstance() == this) {
                _getInstance() = nullptr;
            }
        }

        /**
         * @brief Inicializa el Access Point, el servidor HTTP y el WebSocket.
         * @param ssid Nombre de la red WiFi creada.
         * @param password Contrasena de la red WiFi creada.
         */
        void begin(const char* ssid = "TARS", const char* password = "tars1234") {
            if (_getInstance() != nullptr && _getInstance() != this) {
                return; // Evitar sobreescribir si ya hay otra instancia activa
            }
            WiFi.softAP(ssid, password);
            _server.on("/", [this]() {
                _server.send_P(200, "text/html", _getHTML());
            });
            _server.begin();
            _webSocket.begin();
            _webSocket.onEvent(_wsEventHandler);
            _getInstance() = this;
        }

        /**
         * @brief Procesa clientes HTTP y WebSocket y actualiza pwm1/pwm2 si llega un mensaje.
         * @return true si se recibio y proceso un mensaje de joystick valido.
         */
        bool update() {
            _server.handleClient();
            _webSocket.loop();
            if (_newData) {
                _newData = false;
                return true;
            }
            return false;
        }

        /**
         * @brief Indica si hay al menos un cliente WebSocket conectado.
         * @return true si existe un cliente conectado; false en caso contrario.
         */
        bool isConnected() const {
            return _clientCount > 0;
        }

        /**
         * @brief Convierte posicion de joystick a PWM diferencial y lo guarda en pwm1/pwm2.
         * @note Aunque los comandos por WebSocket llegan en el rango [-1, 1], esta funcion
         * requiere parametros multiplicados por 100, es decir en formato de porcentaje.
         * @param x Eje X en rango porcentual [-100, 100].
         * @param y Eje Y en rango porcentual [-100, 100].
         * @param minPWM PWM minimo en comandos no nulos; usa el del constructor si es -1.
         */
        void joystickToPWM(float x, float y, int16_t minPWM = -1) {
            const int16_t min = minPWM < 0 ? static_cast<int16_t>(_minPWM) : minPWM;
            const float clampedX = constrain(x, -100.0f, 100.0f);
            const float clampedY = constrain(y, -100.0f, 100.0f);

            const int16_t scaledX = static_cast<int16_t>(std::lroundf((clampedX / 100.0f) * 255.0f));
            const int16_t scaledY = static_cast<int16_t>(std::lroundf((clampedY / 100.0f) * 255.0f));

            const int16_t left  = static_cast<int16_t>(constrain(static_cast<int>(scaledY + scaledX), -255, 255));
            const int16_t right = static_cast<int16_t>(constrain(static_cast<int>(scaledY - scaledX), -255, 255));

            pwm1 = _applyMinPWM(left,  min);
            pwm2 = _applyMinPWM(right, min);
        }

    private:
        uint8_t          _minPWM;
        WebServer        _server;
        WebSocketsServer _webSocket;
        uint8_t          _clientCount = 0;
        bool             _newData     = false;

        static TARS_WiFi*& _getInstance() {
            static TARS_WiFi* inst = nullptr;
            return inst;
        }

        static void _wsEventHandler(uint8_t num, WStype_t type, uint8_t* payload, size_t len) {
            TARS_WiFi* self = _getInstance();
            if (!self) return;

            if (type == WStype_CONNECTED) {
                self->_clientCount++;
            } else if (type == WStype_DISCONNECTED) {
                if (self->_clientCount > 0) self->_clientCount--;
            } else if (type == WStype_TEXT) {
                char* buffer = new char[len + 1];
                std::memcpy(buffer, payload, len);
                buffer[len] = '\0';

                float x, y;
                if (_parseJSON(buffer, x, y)) {
                    self->joystickToPWM(x * 100.0f, y * 100.0f);
                    self->_newData = true;
                }

                delete[] buffer;
            }
        }

        static const char* _findJSONKey(const char* json, const char* key) {
            const char* p = json;
            size_t klen = std::strlen(key);
            while ((p = std::strstr(p, key)) != nullptr) {
                const char* after = p + klen;
                while (*after == ' ' || *after == '\t' || *after == '\n' || *after == '\r') {
                    after++;
                }
                if (*after == ':') return p;
                p += klen;
            }
            return nullptr;
        }

        static bool _parseJSON(const char* json, float& outX, float& outY) {
            const char* xKey = _findJSONKey(json, "\"x\"");
            const char* yKey = _findJSONKey(json, "\"y\"");
            if (!xKey || !yKey) return false;

            const char* xVal = std::strchr(xKey, ':') + 1;
            char* endX = nullptr;
            const float px = std::strtof(xVal, &endX);
            if (endX == xVal || std::isnan(px) || std::isinf(px)) return false;

            const char* yVal = std::strchr(yKey, ':') + 1;
            char* endY = nullptr;
            const float py = std::strtof(yVal, &endY);
            if (endY == yVal || std::isnan(py) || std::isinf(py)) return false;

            outX = px;
            outY = py;
            return true;
        }

        int16_t _applyMinPWM(int16_t pwm, int16_t minPWM) const {
            if (pwm == 0) return 0;
            const int sign      = pwm > 0 ? 1 : -1;
            const int magnitude = max(static_cast<int>(std::abs(pwm)), static_cast<int>(minPWM));
            return static_cast<int16_t>(sign * magnitude);
        }

        static const char* _getHTML() {
            static const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
  <title>TARS</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      background: #1a1a2e;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      height: 100vh;
      font-family: sans-serif;
      color: white;
      touch-action: none;
    }
    h1 { font-size: 2rem; margin-bottom: 8px; color: #f5c518; }
    #estado {
      font-size: 0.9rem;
      margin-bottom: 32px;
      padding: 6px 16px;
      border-radius: 20px;
      background: #16213e;
    }
    #estado.conectado   { color: #4ade80; }
    #estado.desconectado { color: #f87171; }
    #log {
      margin-top: 24px;
      font-size: 0.85rem;
      color: #94a3b8;
      text-align: center;
    }
    #zona-joystick {
      width: 250px;
      height: 250px;
      background: #16213e;
      border-radius: 50%;
      border: 2px solid #f5c518;
      position: relative;
      touch-action: none;
    }
    #stick {
      width: 80px;
      height: 80px;
      background: #f5c518;
      border-radius: 50%;
      position: absolute;
      top: 50%; left: 50%;
      transform: translate(-50%, -50%);
      pointer-events: none;
    }
  </style>
</head>
<body>
  <h1>TARS</h1>
  <div id="estado" class="desconectado">Conectando...</div>
  <div id="zona-joystick">
    <div id="stick"></div>
  </div>
  <div id="log">X: 0.00 | Y: 0.00</div>

  <script>
    const zona   = document.getElementById('zona-joystick');
    const stick  = document.getElementById('stick');
    const estado = document.getElementById('estado');
    const log    = document.getElementById('log');

    let ws, dragging = false, activeTouchId = null;
    const RADIO = 85;

    function conectar() {
      ws = new WebSocket('ws://' + location.hostname + ':81');
      ws.onopen  = () => { estado.textContent = 'Conectado'; estado.className = 'conectado'; };
      ws.onclose = () => { estado.textContent = 'Desconectado'; estado.className = 'desconectado'; setTimeout(conectar, 2000); };
    }

    function getPos(e) {
      const r  = zona.getBoundingClientRect();
      const cx = r.left + r.width  / 2;
      const cy = r.top  + r.height / 2;
      let dx = e.clientX - cx, dy = e.clientY - cy;
      const dist = Math.sqrt(dx*dx + dy*dy);
      if (dist > RADIO) { dx = dx/dist*RADIO; dy = dy/dist*RADIO; }
      return { dx, dy };
    }

    function moverStick(dx, dy) {
      stick.style.transform = `translate(calc(-50% + ${dx}px), calc(-50% + ${dy}px))`;
      const x = (dx / RADIO).toFixed(2);
      const y = (-dy / RADIO).toFixed(2);
      log.textContent = `X: ${x} | Y: ${y}`;
      if (ws && ws.readyState === 1) ws.send(JSON.stringify({ x: parseFloat(x), y: parseFloat(y) }));
    }

    function resetStick() {
      stick.style.transform = 'translate(-50%, -50%)';
      log.textContent = 'X: 0.00 | Y: 0.00';
      if (ws && ws.readyState === 1) ws.send(JSON.stringify({ x: 0, y: 0 }));
    }

    zona.addEventListener('touchstart', e => { e.preventDefault(); activeTouchId = e.changedTouches[0].identifier; dragging = true; }, { passive: false });
    zona.addEventListener('touchmove',  e => { e.preventDefault(); if (!dragging) return; for (const t of e.changedTouches) if (t.identifier === activeTouchId) moverStick(...Object.values(getPos(t))); }, { passive: false });
    zona.addEventListener('touchend',   e => { dragging = false; activeTouchId = null; resetStick(); });

    zona.addEventListener('mousedown', () => dragging = true);
    document.addEventListener('mousemove', e => { if (dragging) moverStick(...Object.values(getPos(e))); });
    document.addEventListener('mouseup',   () => { if (dragging) { dragging = false; resetStick(); } });

    conectar();
  </script>
</body>
</html>
)rawliteral";
            return html;
        }
};
