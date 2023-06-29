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


bool GET_SensorData(String http_endpoint, String get_payload) {
  bool success = false;

  if ((WiFi.status() == WL_CONNECTED)){

    // Start new http connection
    HTTPClient http_client;

    String endpoint_url = http_endpoint + get_payload;
    if(http_client.begin(wifiClient, endpoint_url)){

      // GET 
      int httpCode = http_client.GET();

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


bool POST_SensorData(String http_endpoint, String post_payload) {
  bool success = false;

  if ((WiFi.status() == WL_CONNECTED)){

    // Start new http connection
    HTTPClient http_client;
    if(http_client.begin(wifiClient, http_endpoint)){

      // POST 
      http_client.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int httpCode = http_client.POST(post_payload);

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