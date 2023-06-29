void environment_measurement(int measurements, int delay_time) {
  float avg_temperature = 0;
  float avg_pressure = 0;
  float avg_humidity = 0;

  for (int i = 0; i < measurements; i++){
    avg_temperature = avg_temperature + bme.readTemperature();
    avg_pressure = avg_pressure + bme.readPressure() / 100;
    avg_humidity = avg_humidity + bme.readHumidity();
    delay(delay_time);
  }

  glob_temperature = (avg_temperature / measurements);
  glob_pressure = (avg_pressure / measurements);
  glob_humidity = (avg_humidity / measurements);
}

bool connectWifi(const char *ssid , const char *password, int interval, int max_con_count) {
  // Try to connect
  WiFi.hostname("ESP-host-" + String(SENSOR_NO));
  WiFi.begin(ssid, password);

  int wifiConnCheckCount = 0;
  while (WiFi.status() != WL_CONNECTED && wifiConnCheckCount < max_con_count){
    delay(interval);
    wifiConnCheckCount++;
  }

  // Check if connected
  if (WiFi.status() != WL_CONNECTED){
    WiFi.disconnect();
    return false;
  }
  else{
    return true;
  }
}

/*
bool sendSensorData_php_api() {
  bool success = false;

  if ((WiFi.status() == WL_CONNECTED)){

    // Start new http connection
    HTTPClient http_client;
    if(http_client.begin(wifiClient, php_api_Endpoint)){
      
      // Format data as json
      String htmlapirequest = "token=" + php_api_Token + "&";
      htmlapirequest += "sensor=" + php_api_sensorNo + "&";
      htmlapirequest += "temperature=" + String(glob_temperature) + "&";
      htmlapirequest += "humidity="    + String(glob_humidity) + "&";
      htmlapirequest += "pressure="    + String(glob_pressure);

      // Post 
      http_client.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http_client.POST(htmlapirequest);

      // Check if request was successful
      if (httpCode == 200){
        success = true;
      }
      else{
        success = false;
      }

      // Close connection
      http_client.end();

      return success;
    }    
  }

  return success;
}
*/

