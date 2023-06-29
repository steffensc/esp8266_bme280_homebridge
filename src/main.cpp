#include <Arduino.h>

// WiFi
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// BME 280 Sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Configuration
#include <config.h>


#define BME_ADDRESS 0x76

#define WIFI_CONN_CHECK_INTERVAL 1000
#define WIFI_CONN_MAX_CHECK_COUNT 5

#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 300       // Time ESP32 will go to sleep (in seconds)


// Constants



// Globals
WiFiClient wifiClient;
Adafruit_BME280 bme;

float glob_temperature;
float glob_pressure;
float glob_humidity;

/* - - - - - - METHODS - - - - - - */
#include "methods.cpp"





/* - - - - - - - MAIN - - - - - - - */
void setup() {
  // Init BME 280
  int bme_status = bme.begin(BME_ADDRESS);
  if (!bme_status){
    while (1)
      ;
  }  
  // Start WiFi
  connectWifi(WIFI_SSID, WIFI_PASSWORD, WIFI_CONN_CHECK_INTERVAL, WIFI_CONN_MAX_CHECK_COUNT);
}

void loop() {
  environment_measurement(3, 10);

  // Format data as json
  String get_content = "?accessoryId=temperature"+SENSOR_NO+"&value=" + String(glob_temperature);
  GET_SensorData(HOMEBRIDGE_WEBHOOK_URL, get_content);

  get_content = "?accessoryId=humidity"+SENSOR_NO+"&value=" + String(glob_humidity);
  GET_SensorData(HOMEBRIDGE_WEBHOOK_URL, get_content);

  String post_content = "token=" + PHPAPI_SERVER_TOKEN + "&";
  post_content += "sensor=" + SENSOR_NO + "&";
  post_content += "temperature=" + String(glob_temperature) + "&";
  post_content += "humidity="    + String(glob_humidity) + "&";
  post_content += "pressure="    + String(glob_pressure);
  POST_SensorData(PHPAPI_SERVER_URL, post_content);
  
  ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // Set deepsleep for specified time
}

