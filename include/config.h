#pragma once 

// Initial Default Values (Configured in EEPROM) //
const String DEFAULT_WIFI_SSID     = "WiFi SSID";
const String DEFAULT_WIFI_PASSWORD = "Password";
const String DEFAULT_SENSORNO      = "00";
const float  DEFAULT_TEMP_BIAS     = 0.0;
const float  DEFAULT_HUMI_BIAS     = 0.0;

// WiFi HotSpot Setup Password //
const String WIFI_SETUP_PASS       = "123456789";

// PHP API Server Endpoint Configuration //
const String PHPAPI_SERVER_URL     = "http://webserverurl.de/temperature/write_data.php";
const String PHPAPI_SERVER_TOKEN   = "tokenstring";

// Homebridge Server Webhook Configuration //
const String HOMEBRIDGE_WEBHOOK_URL = "http://192.168.1.42:51828/";
