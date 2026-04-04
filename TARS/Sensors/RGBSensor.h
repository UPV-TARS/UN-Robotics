#pragma once

#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Sensor RGB (TCS34725) con API sencilla para uso educativo.
 *
 * La clase expone metodos directos para leer color o canales individuales,
 * mientras que toda la logica de filtrado y clasificacion permanece interna.
 */
class TARS_RGBSensor {
  public:
    /**
     * @brief Crea el sensor RGB con configuración por defecto.
     */
    TARS_RGBSensor()
        : _tcs(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X), _lastReadMs(0), _hasLastRGB(false) {}

    /**
     * @brief Inicializa la comunicacion con el sensor.
     * @return true si el sensor responde correctamente.
     */
    bool begin() {
        return _tcs.begin();
    }

    /**
     * @brief Lee el tono HSV de la muestra actual.
     * @return Tono en grados [0, 360).
     */
    float readHue() {
        const RGBReading &rgb = readRGBCached();
        float hue = 0.0f;
        float saturation = 0.0f;
        float value = 0.0f;
        calculateHSV(rgb, hue, saturation, value);
        return hue;
    }

    /**
     * @brief Lee el canal rojo crudo.
     * @return Valor del canal R.
     */
    uint16_t readRed() {
        const RGBReading &rgb = readRGBCached();
        return rgb.r;
    }

    /**
     * @brief Lee el canal verde crudo.
     * @return Valor del canal G.
     */
    uint16_t readGreen() {
        const RGBReading &rgb = readRGBCached();
        return rgb.g;
    }

    /**
     * @brief Lee el canal azul crudo.
     * @return Valor del canal B.
     */
    uint16_t readBlue() {
        const RGBReading &rgb = readRGBCached();
        return rgb.b;
    }

    /**
     * @brief Lee el canal clear (luminosidad).
     * @return Valor del canal C.
     */
    uint16_t readClear() {
        const RGBReading &rgb = readRGBCached();
        return rgb.c;
    }

    /**
     * @brief Lee los canales RGB crudos.
     * @param[out] r Canal rojo.
     * @param[out] g Canal verde.
     * @param[out] b Canal azul.
     */
    void readRGB(uint16_t &r, uint16_t &g, uint16_t &b) {
        uint16_t c = 0;
        readRGB(r, g, b, c);
    }

    /**
     * @brief Lee los canales RGBC crudos.
     * @param[out] r Canal rojo.
     * @param[out] g Canal verde.
     * @param[out] b Canal azul.
     * @param[out] c Canal clear.
     */
    void readRGB(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
        const RGBReading &rgb = readRGBCached();
        r = rgb.r;
        g = rgb.g;
        b = rgb.b;
        c = rgb.c;
    }

    /**
     * @brief Lee el color clasificado en formato numerico.
     * @return ID de color interno:
     * 0 Negro, 1 Blanco, 2 Rojo, 3 Verde, 4 Azul, 5 Amarillo.
     */
    uint8_t readColorID() {
        const RGBReading &rgb = readRGBCached();
        return classifyColorIDFromRGB(rgb);
    }

    /**
     * @brief Lee el nombre del color clasificado.
     * @return Cadena constante con el nombre del color.
     */
    const char *readColorName() {
        return colorNameFromID(readColorID());
    }

  private:
    struct RGBReading {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t c;
    };

    static constexpr uint8_t COLOR_ID_NEGRO = 0U;
    static constexpr uint8_t COLOR_ID_BLANCO = 1U;
    static constexpr uint8_t COLOR_ID_ROJO = 2U;
    static constexpr uint8_t COLOR_ID_VERDE = 3U;
    static constexpr uint8_t COLOR_ID_AZUL = 4U;
    static constexpr uint8_t COLOR_ID_AMARILLO = 5U;
    static constexpr uint16_t RGB_CACHE_MS = 20U;
    static constexpr uint16_t BLACK_CLEAR_ABS_MAX = 250U;
    static constexpr uint16_t BLACK_CLEAR_REFLECTIVE_MAX = 1200U;
    static constexpr uint16_t BLACK_MAX_RGB_MAX = 900U;
    static constexpr uint16_t BLACK_DELTA_MAX = 220U;
    static constexpr uint16_t BLACK_RELATIVE_RGB_LIMIT = 2U;
    static constexpr uint16_t BLACK_DARK_VALUE_LIMIT = 18U;
    static constexpr float BLACK_VALUE_MAX = 0.10f;
    static constexpr float BLACK_SATURATION_MAX = 0.45f;
    static constexpr uint16_t WHITE_CLEAR_MIN = 3000U;
    static constexpr float INV_U16_MAX = 1.0f / 65535.0f;

    Adafruit_TCS34725 _tcs;
    RGBReading _lastRGB{};
    uint32_t _lastReadMs;
    bool _hasLastRGB;

    RGBReading readRawRGB() {
        RGBReading rgb{};
        _tcs.getRawData(&rgb.r, &rgb.g, &rgb.b, &rgb.c);
        return rgb;
    }

    const RGBReading &readRGBCached() {
        const uint32_t nowMs = millis();
        const bool shouldUpdate = !_hasLastRGB || ((nowMs - _lastReadMs) >= RGB_CACHE_MS);

        if (shouldUpdate) {
            _lastRGB = readRawRGB();
            _lastReadMs = nowMs;
            _hasLastRGB = true;
        }

        return _lastRGB;
    }

    static void calculateHSV(const RGBReading &rgb, float &hue, float &saturation, float &value) {
        const float rf = static_cast<float>(rgb.r) * INV_U16_MAX;
        const float gf = static_cast<float>(rgb.g) * INV_U16_MAX;
        const float bf = static_cast<float>(rgb.b) * INV_U16_MAX;

        const float cmax = max(rf, max(gf, bf));
        const float cmin = min(rf, min(gf, bf));
        const float delta = cmax - cmin;

        value = cmax;
        saturation = (cmax <= 0.0f) ? 0.0f : (delta / cmax);

        if (delta <= 0.0f) {
            hue = 0.0f;
            return;
        }

        float hPrime = 0.0f;
        if (cmax == rf) {
            hPrime = (gf - bf) / delta;
            if (hPrime < 0.0f) {
                hPrime += 6.0f;
            }
        }
        else if (cmax == gf) {
            hPrime = ((bf - rf) / delta) + 2.0f;
        }
        else {
            hPrime = ((rf - gf) / delta) + 4.0f;
        }

        hue = 60.0f * hPrime;
    }

    static uint8_t classifyColorIDFromRGB(const RGBReading &rgb) {
        float hue = 0.0f;
        float saturation = 0.0f;
        float value = 0.0f;
        calculateHSV(rgb, hue, saturation, value);

        if (isBlackCandidate(rgb, saturation, value)) {
            return COLOR_ID_NEGRO;
        }

        if (isWhiteCandidate(rgb)) {
            return COLOR_ID_BLANCO;
        }

        return classifyNonWhiteColorID(hue, saturation, value);
    }

    static bool isBlackCandidate(const RGBReading &rgb, float saturation, float value) {
        // Si C ya esta en rango de blanco, nunca debe clasificarse como negro.
        if (rgb.c >= WHITE_CLEAR_MIN) {
            return false;
        }

        if (rgb.c < BLACK_CLEAR_ABS_MAX) {
            return true;
        }

        // Negro reflectante: aunque aparezca algun canal algo dominante, la luz total sigue siendo baja.
        if (rgb.c < BLACK_CLEAR_REFLECTIVE_MAX) {
            const uint16_t maxRGB = max(rgb.r, max(rgb.g, rgb.b));
            const uint16_t minRGB = min(rgb.r, min(rgb.g, rgb.b));
            const uint16_t deltaRGB = maxRGB - minRGB;

            if (maxRGB < BLACK_MAX_RGB_MAX && deltaRGB < BLACK_DELTA_MAX) {
                return true;
            }

            if (maxRGB * BLACK_RELATIVE_RGB_LIMIT < rgb.c) {
                return true;
            }
        }

        return rgb.c < BLACK_CLEAR_REFLECTIVE_MAX && value < BLACK_VALUE_MAX
            && saturation < BLACK_SATURATION_MAX;
    }

    static bool isWhiteCandidate(const RGBReading &rgb) {
        if (rgb.c < WHITE_CLEAR_MIN) {
            return false;
        }

        const uint16_t maxRGB = max(rgb.r, max(rgb.g, rgb.b));
        const uint16_t minRGB = min(rgb.r, min(rgb.g, rgb.b));
        const uint16_t deltaRGB = maxRGB - minRGB;

        // Blanco con sesgo de iluminacion: todos los canales deben seguir siendo relativamente altos
        // respecto al canal clear, aunque uno de ellos domine un poco.
        return (static_cast<uint32_t>(minRGB) * 5U >= static_cast<uint32_t>(rgb.c))
            && (static_cast<uint32_t>(deltaRGB) * 5U <= static_cast<uint32_t>(rgb.c));
    }

    static uint8_t classifyNonWhiteColorID(float hue, float saturation, float value) {
        // Oscuridad fuerte: si la luz total es muy baja, es negro aunque el tono apunte a verde.
        if (value <= static_cast<float>(BLACK_DARK_VALUE_LIMIT) / 65535.0f) {
            return COLOR_ID_NEGRO;
        }

        // Si no hay saturacion suficiente tras descartar blanco, lo tratamos como negro.
        if (saturation < 0.18f) {
            return COLOR_ID_NEGRO;
        }

        if (hue < 25.0f || hue >= 340.0f) {
            return COLOR_ID_ROJO;
        }
        else if (hue < 85.0f) {
            return COLOR_ID_AMARILLO;
        }
        else if (hue < 170.0f) {
            return COLOR_ID_VERDE;
        }
        else if (hue < 290.0f) {
            return COLOR_ID_AZUL;
        }

        return COLOR_ID_ROJO;
    }

    static const char *colorNameFromID(uint8_t colorID) {
        if (colorID == COLOR_ID_BLANCO) {
            return "Blanco";
        }
        if (colorID == COLOR_ID_ROJO) {
            return "Rojo";
        }
        if (colorID == COLOR_ID_VERDE) {
            return "Verde";
        }
        if (colorID == COLOR_ID_AZUL) {
            return "Azul";
        }
        if (colorID == COLOR_ID_AMARILLO) {
            return "Amarillo";
        }

        return "Negro";
    }
};