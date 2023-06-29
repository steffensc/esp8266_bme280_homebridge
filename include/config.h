#pragma once 

// WiFi Configuration //
const char *WIFI_SSID     = "WiFi SSid";
const char *WIFI_PASSWORD = "Password";


// PHP API Server Endpoint Configuration //
const String PHPAPI_SERVER_URL   = "http://webserverurl.de/temperature/write_data.php";
const String PHPAPI_SERVER_TOKEN = "tokenstring";

// Homebridge Server Webhook Configuration //
const String HOMEBRIDGE_WEBHOOK_URL   = "http://192.168.1.42:51828/";


// Device Configuration
const String SENSOR_NO = "10"; // Watch out for keeping this unique
