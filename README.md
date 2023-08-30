# esp8266_bme280_homekit

## Usage / Setup / Configuration
To enter the setup-mode, press and hold the flash-button for approx 3 seconds during startup (replugging USB-powercable). The ESP will switch into hotspot-mode and open a WiFi-Network (SSDI: e.g. *ESPclimatesensor-00*). Connect to the WiFi, the default Password is 123456789 (see *WIFI_SETUP_PASS* in `include/config.h`). When connectod, go to `192.168.4.1` in your browser. Configure the SSID and password of the WiFi network, you want the ESP to connect to.

Hardware:
- ESP8266 Node MCU
- I2C BME280 (temperature, pressure, humidity)

### Wiring
| ESP8266  | BME280 |
| -------- | ------- |
| January  | $250    |
| February | $80     |
| March    | $420    |
