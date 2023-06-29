#pragma once 

// EEPROM Memory indexes / adresses
#define EEPROM_SIZE 256 // max 512 Bytes!
#define EEPROM_SSID_START_IDX 0
#define EEPROM_SSID_BYTES_LEN 32
#define EEPROM_WIFIPASS_START_IDX 32
#define EEPROM_WIFIPASS_BYTES_LEN 64
#define EEPROM_SENSORNO_START_IDX 96
#define EEPROM_SENSORNO_BYTES_LEN 3

#define BME_ADDRESS 0x76

#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 300       // Time ESP32 will go to sleep (in seconds)

#define WIFI_CONN_CHECK_INTERVAL 1000
#define WIFI_CONN_MAX_CHECK_COUNT 10


