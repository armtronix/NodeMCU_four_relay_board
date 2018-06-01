void Scan_Wifi_Networks()
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  // Need to be in dicsonected mode to Run network Scan!
 // WiFi.mode(WIFI_STA);
 // WiFi.disconnect();
//  delay(100);
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
 // Serial.println("Scaning Networks Complete..");
 // Serial.print(n); Serial.println(" Networks have been Found");

  if (n == 0)
  {
   // Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      if (esid == WiFi.SSID(i))

      {
        Serial.print("My network has been Found! ");
        Do_Connect();
        //Fl_MyNetwork = true;
      }
      else
      {
        Serial.print("Not my network... ");
      }
      // Print SSID and RSSI for each network found
      //Serial.print(i + 1);
     // Serial.print(": ");
     // Serial.print(WiFi.SSID(i));
     // Serial.print(" (");
     // Serial.print(WiFi.RSSI(i));
     // Serial.print(")");
     // Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      //delay(10);
    }
  }
  Serial.println("");
}


void Do_Connect()                  // Try to connect to the Found WIFI Network!
{


 #ifdef ARDUINO_ARCH_ESP32
           esp_restart();
           #else
           ESP.reset();
           #endif 
  
}

