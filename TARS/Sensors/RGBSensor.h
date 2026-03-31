#pragma once

#include <Adafruit_TCS34725.h>
#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Sensor RGB (TCS34725) con API minima para uso educativo.
 */
class TARS_RGBSensor {
  public:
    explicit TARS_RGBSensor(
        tcs34725IntegrationTime integrationTime = TCS34725_INTEGRATIONTIME_614MS,
        tcs34725Gain gain = TCS34725_GAIN_1X)
        : _tcs(integrationTime, gain), _hue(0.0f) {}

    bool begin() {
        return _tcs.begin();
    }

    void readRaw(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
        _tcs.getRawData(&r, &g, &b, &c);
    }

    float readHue() {
        uint16_t r, g, b, c;
        _tcs.getRawData(&r, &g, &b, &c);
        (void)c;
        _hue = calculateHue(r, g, b);
        return _hue;
    }

    float getHue() const {
        return _hue;
    }

    const char *readColorName() {
        const float h = readHue();

        if (h < 15.0f) {
            return "Rojo";
        }
        if (h < 45.0f) {
            return "Naranja";
        }
        if (h < 90.0f) {
            return "Amarillo";
        }
        if (h < 150.0f) {
            return "Verde";
        }
        if (h < 210.0f) {
            return "Cyan";
        }
        if (h < 270.0f) {
            return "Azul";
        }
        if (h < 330.0f) {
            return "Magenta";
        }
        return "Rojo";
    }

  private:
    Adafruit_TCS34725 _tcs;
    float _hue;

    static float calculateHue(uint16_t r, uint16_t g, uint16_t b) {
        const float rf = static_cast<float>(r);
        const float gf = static_cast<float>(g);
        const float bf = static_cast<float>(b);

        const float maxRgb = max(rf, max(gf, bf));
        if (maxRgb <= 0.0f) {
            return 0.0f;
        }

        const float rn = rf / maxRgb;
        const float gn = gf / maxRgb;
        const float bn = bf / maxRgb;

        const float cmax = max(rn, max(gn, bn));
        const float cmin = min(rn, min(gn, bn));
        const float delta = cmax - cmin;
        if (delta <= 0.0f) {
            return 0.0f;
        }

        float hPrime = 0.0f;
        if (rn >= gn && rn >= bn) {
            hPrime = (gn - bn) / delta;
            if (hPrime < 0.0f) {
                hPrime += 6.0f;
            }
        } else if (gn >= rn && gn >= bn) {
            hPrime = ((bn - rn) / delta) + 2.0f;
        } else {
            hPrime = ((rn - gn) / delta) + 4.0f;
        }

        return 60.0f * hPrime;
    }
};