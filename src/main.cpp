#include <Arduino.h>

#include <EEPROM.h>

// WiFi
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>


// BME 280 Sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Configuration
#include <config.h>
#include <constants.h>

#define DEBUG_PRINT true

// Globals
ESP8266WebServer server(80);
WiFiClient wifiClient;
Adafruit_BME280 bme;

String wifi_ssid;
String wifi_password;
String sensorNo;

float glob_temperature;
float glob_pressure;
float glob_humidity;
bool setup_mode = false;



IRAM_ATTR void ISR_onTimer()
{
  ESP.reset();
}



/* - - - - - - METHODS - - - - - - */
#include "configsite_html.h"
#include "eeprom_methods.cpp"
#include "methods.cpp"


/* - - - - - - - SETUP - - - - - - - */
void setup() {
  #if (DEBUG_PRINT) 
    Serial.begin(115200);
  #endif

  // Read Data from EEPROM //
  #if (DEBUG_PRINT) 
    Serial.println("Reading from EEPROM");
  #endif
  EEPROM.begin(EEPROM_SIZE);
  delay(10);
  wifi_ssid = "";
  eeprom_read_string(EEPROM_SSID_START_IDX, EEPROM_SSID_BYTES_LEN, wifi_ssid);
  wifi_password = "";
  eeprom_read_string(EEPROM_WIFIPASS_START_IDX, EEPROM_WIFIPASS_BYTES_LEN, wifi_password);
  sensorNo = "";
  eeprom_read_string(EEPROM_SENSORNO_START_IDX, EEPROM_SENSORNO_BYTES_LEN, sensorNo);

  // fallback to defaults if EEPROM is empty, initialize EEPROM with default values
  #if (DEBUG_PRINT) 
    Serial.println("EEPROM fallback to defaults");
  #endif
  if(wifi_ssid == ""){
    wifi_ssid = DEFAULT_WIFI_SSID;
    eeprom_write_string(EEPROM_SSID_START_IDX, EEPROM_SSID_BYTES_LEN, wifi_ssid); 
    EEPROM.commit();
  }
  if(wifi_password == ""){
    wifi_password = DEFAULT_WIFI_PASSWORD;
    eeprom_write_string(EEPROM_WIFIPASS_START_IDX, EEPROM_WIFIPASS_BYTES_LEN, wifi_password);
    EEPROM.commit();
  }
  if(sensorNo == ""){
    sensorNo = DEFAULT_SENSORNO;
    eeprom_write_string(EEPROM_SENSORNO_START_IDX, EEPROM_SENSORNO_BYTES_LEN, sensorNo);
    EEPROM.commit();
  }


  // Init BME 280 //
  int bme_status = bme.begin(BME_ADDRESS);
  if (!bme_status){
    while (1)
      ;
  } 

  // Start WiFi //
  #if (DEBUG_PRINT) 
    Serial.println("Connecting to WiFi:" + wifi_ssid + " PASS:" + wifi_password);
  #endif
  if(! connectWifi(WIFI_CONN_CHECK_INTERVAL, WIFI_CONN_MAX_CHECK_COUNT)){
    // Could not connect to WiFi
    #if (DEBUG_PRINT) 
      Serial.println("WiFi connection failed. Starting HotSpot");
    #endif

    setup_mode = true;
    setupHotSpot();
  }
  else{
    #if (DEBUG_PRINT) 
      Serial.println("WiFi successfully connected");
    #endif
  }
  

}


/* - - - - - MAIN LOOP - - - - - */
void loop() {

  if(!setup_mode){
    environment_measurement(3, 10);

    // Format data as json
    String get_content = "?accessoryId=temperature"+sensorNo+"&value=" + String(glob_temperature);
    GET_SensorData(HOMEBRIDGE_WEBHOOK_URL, get_content);

    get_content = "?accessoryId=humidity"+sensorNo+"&value=" + String(glob_humidity);
    GET_SensorData(HOMEBRIDGE_WEBHOOK_URL, get_content);

    String post_content = "token=" + PHPAPI_SERVER_TOKEN + "&";
    post_content += "sensor=" + sensorNo + "&";
    post_content += "temperature=" + String(glob_temperature) + "&";
    post_content += "humidity="    + String(glob_humidity) + "&";
    post_content += "pressure="    + String(glob_pressure);
    POST_SensorData(PHPAPI_SERVER_URL, post_content);
    
    ESP.deepSleep(TIME_TO_SLEEP * uS_TO_S_FACTOR); // Set deepsleep for specified time
  }

  // SETUP MODE
  else{
    server.handleClient();
  }
  
}

