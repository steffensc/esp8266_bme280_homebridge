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


String avail_networks_html = "";

void scanWiFiNetworks()
{
  // Disconnect WiFi to scan for networks
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  int available_wifi_networks = WiFi.scanNetworks();

  avail_networks_html = "";
  for (int i = 0; i < available_wifi_networks && i < 5; ++i)
  {
    // Print SSID and RSSI for each network found
    avail_networks_html += "<li id='";
    avail_networks_html += WiFi.SSID(i);
    avail_networks_html += "'>";
    avail_networks_html += WiFi.SSID(i);
    avail_networks_html += " (";
    avail_networks_html += WiFi.RSSI(i);
    avail_networks_html += " db) ";
    avail_networks_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    avail_networks_html += "</li>";
  }

  delay(100);
}


void createWebServer()
{
  {   
    server.on("/", []() {
      #if (ENABLE_HOTSPOT_TIMEOUT)
        timer1_detachInterrupt(); // Disable timer / reset of ESP since someone connected to the Hostpot
      #endif

      IPAddress hotspot_ip = WiFi.softAPIP();
      String ipStr = String(hotspot_ip[0]) + '.' + String(hotspot_ip[1]) + '.' + String(hotspot_ip[2]) + '.' + String(hotspot_ip[3]);
      
      char html_buffer[1600];
      snprintf(html_buffer, 1600, configsite_config_html.c_str(), "ESPclimatesensor", ipStr.c_str(), avail_networks_html.c_str(), wifi_ssid.c_str(), wifi_password.c_str(), sensorNo.c_str());

      server.send(200, "text/html", html_buffer);
    });
  
    server.on("/setting", []() {
      String qssid = server.arg("ssid");
      String qpass = server.arg("pass");
      String qsensorno = server.arg("sensorno");

      String html_code = "";
      if (qssid.length() > 0 && qpass.length() > 0 && qsensorno.length() > 0) {
        
        eeprom_write_string(EEPROM_SSID_START_IDX, EEPROM_SSID_BYTES_LEN, qssid); 
        eeprom_write_string(EEPROM_WIFIPASS_START_IDX, EEPROM_WIFIPASS_BYTES_LEN, qpass);
        eeprom_write_string(EEPROM_SENSORNO_START_IDX, EEPROM_SENSORNO_BYTES_LEN, qsensorno);

        EEPROM.commit();

        server.send(200, "text/html", configsite_success_html);
        ESP.reset();
      } 
      else {
        server.send(200, "text/html", configsite_error_html);
      }
 
    });

    server.on("/rescan", []() {
      scanWiFiNetworks();

      //"Page refresh" by redirecting back to the main / basedir
      server.sendHeader("Location", String("/"), true);
      server.send(302, "text/plain", "");
    });
  } 
}


void setupHotSpot(){  
  scanWiFiNetworks();

  // Re-enable WiFi and soft-AP
  WiFi.softAP("ESPclimatesensor"+String(sensorNo), WIFI_SETUP_PASS);
  #if (DEBUG_PRINT) 
    Serial.println("HotSpot is started");
  #endif

  #if (ENABLE_HOTSPOT_TIMEOUT)
    // TIMER INTERRUPT //
    // Use Timer1, Timer0 already used by WiFi functionality
    // Timer triggers reset of ESP when noone has connected to the WiFi AP in the timer's configured timespan
    timer1_isr_init();
    timer1_attachInterrupt(ISR_onTimer);
    timer1_write(8388607); // maximum ticks for timer: 8388607 // ESP8266 has 80MHz clock, division by 256 = 312.5Khz (1 tick = 3.2us - 26.8435424 sec max), maximum ticks 8388607 
  #endif

  createWebServer();
  server.begin();
}


bool connectWifi(int interval, int max_con_count) {
  // Try to connect
  WiFi.hostname("ESPclimatesensor"+String(sensorNo));
  WiFi.begin(wifi_ssid, wifi_password);

  int wifiConnCheckCount = 0;
  while (WiFi.status() != WL_CONNECTED && wifiConnCheckCount < max_con_count){
    delay(interval);
    wifiConnCheckCount++;

    #if (DEBUG_PRINT) 
      Serial.print(".");
    #endif
  }

  // Check if connected
  if (WiFi.status() == WL_CONNECTED){
    return true;
  }

  return false;
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