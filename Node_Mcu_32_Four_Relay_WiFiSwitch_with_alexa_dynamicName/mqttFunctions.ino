boolean connectMQTT(){
  if (mqttClient.connected()){
    return true;
  }  
  
  Serial.print("Connecting to MQTT server ");
  Serial.print(mqttServer);
  Serial.print(" as ");
  Serial.println(host);
  
  if (mqttClient.connect(host)) {
    Serial.println("Connected to MQTT broker");
    if(mqttClient.subscribe((char*)subTopic.c_str())){
      Serial.println("Subsribed to topic.");
    } else {
      Serial.println("NOT subsribed to topic!");      
    }
    return true;
  }
  else {
    Serial.println("MQTT connect failed! ");
    return false;
  }
}

void disconnectMQTT(){
  mqttClient.disconnect();
}

void mqtt_handler(){
  if (toPub==1){
    Debugln("DEBUG: Publishing state via MWTT");
    if(pubState()){
     toPub=0; 
    }
  }
  mqttClient.loop();
  delay(100); //let things happen in background
}

void mqtt_arrived(char* subTopic, byte* payload, unsigned int length) { // handle messages arrived 
  int i = 0;
  Serial.print("MQTT message arrived:  topic: " + String(subTopic));
    // create character buffer with ending null terminator (string)
    
  for(i=0; i<length; i++) {    
    buf[i] = payload[i];
  }
  buf[i] = '\0';
  String msgString = String(buf);
  Serial.println(" message: " + msgString);
  
  if (msgString == "R4_ON")
  {
#ifdef ARDUINO_ARCH_ESP32
      if(switch_status_04==1)
      { 
       wifi_input_status_04=0;
      }
      else
      { 
       wifi_input_status_04=1;
      }
      send_status_04=1;
#else
    digitalWrite(OUTPIN_04, HIGH);
#endif 
  } 
  else if (msgString == "R4_OFF")
  {
#ifdef ARDUINO_ARCH_ESP32
      if(switch_status_04==0)
      { 
       wifi_input_status_04=0;
      }
      else
      { 
       wifi_input_status_04=1;
      }
      send_status_04=1;
#else
    digitalWrite(OUTPIN_04, LOW);
#endif
  }

  
  if (msgString == "R12_ON")
  {
#ifdef ARDUINO_ARCH_ESP32
      if(switch_status_12==1)
      { 
       wifi_input_status_12=0;
      }
      else
      { 
       wifi_input_status_12=1;
      }
      send_status_12=1;
#else
    digitalWrite(OUTPIN_12, HIGH);
#endif
  } 
  else if (msgString == "R12_OFF")
  {
#ifdef ARDUINO_ARCH_ESP32
      if(switch_status_12==0)
      { 
       wifi_input_status_12=0;
      }
      else
      { 
       wifi_input_status_12=1;
      }
      send_status_12=1;
#else
    digitalWrite(OUTPIN_12, LOW);
#endif
  }   

       
  if (msgString == "R13_ON")
  {
#ifdef ARDUINO_ARCH_ESP32
       if(switch_status_13==1)
      { 
       wifi_input_status_13=0;
      }
      else
      { 
       wifi_input_status_13=1;
      }
      send_status_13=1;
#else
    digitalWrite(OUTPIN_13, HIGH);
#endif
  } 
  else if (msgString == "R13_OFF")
  {
#ifdef ARDUINO_ARCH_ESP32
      if(switch_status_13==0)
      { 
       wifi_input_status_13=0;
      }
      else
      { 
       wifi_input_status_13=1;
      }
      send_status_13=1;
#else
    digitalWrite(OUTPIN_13, LOW);
#endif
  }  

  
 if (msgString == "R14_ON")
    {
#ifdef ARDUINO_ARCH_ESP32

     if(switch_status_14==1)
      { 
       wifi_input_status_14=0;
      }
      else
      { 
       wifi_input_status_14=1;
      }
      send_status_14=1;
#else
    digitalWrite(OUTPIN_14, HIGH);
#endif
    } 
  else if (msgString == "R14_OFF")
    {
#ifdef ARDUINO_ARCH_ESP32

      if(switch_status_14==0)
      { 
       wifi_input_status_14=0;
      }
      else
      { 
       wifi_input_status_14=1;
      }
      send_status_14=1;
#else
    digitalWrite(OUTPIN_14, LOW);
#endif
    }

    
   if (msgString == "R5_ON")
   {
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTLED));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      digitalWrite(OUTLED, 1); 
  } 
  else if (msgString == "R5_OFF")
  {
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTLED));    
      Serial.print("Switching light to "); 
      Serial.println("low");
      digitalWrite(OUTLED, 0); 
  } 
   if (msgString == "Reset")
   {
     clearConfig();
     delay(10);
     Serial.println("Done, restarting!");
#ifdef ARDUINO_ARCH_ESP32
     esp_restart();
#else
     ESP.reset();
#endif 
   }
}

boolean pubState(){ //Publish the current state of the light   
  boolean publish_ok; 
  if (!connectMQTT()){
      delay(100);
      if (!connectMQTT){                            
        Serial.println("Could not connect MQTT.");
        Serial.println("Publish state NOK");
        publish_ok=false;
        return publish_ok;
      }
    }
    if (mqttClient.connected()){      
        //String state = (digitalRead(OUTPIN))?"1":"0";
        
#ifdef ARDUINO_ARCH_ESP32
      if ( send_status_04 == 1) {
        mqttClient.publish((char*)pubTopic.c_str(), (char*) state1.c_str()); 
        send_status_04=0;
        Serial.println("Publish state1 OK");        
       publish_ok=true;
      } 
      if ( send_status_12 == 1) {
        mqttClient.publish((char*)pubTopic.c_str(), (char*) state2.c_str()); 
        send_status_12=0;
        Serial.println("Publish state2 OK");        
        publish_ok=true;
      } 
      if ( send_status_13 == 1) {
        mqttClient.publish((char*)pubTopic.c_str(), (char*) state3.c_str()) ;
        send_status_13=0;
        Serial.println("Publish state3 OK");        
        publish_ok=true;
      } 
      if ( send_status_14 == 1) {
        mqttClient.publish((char*)pubTopic.c_str(), (char*) state4.c_str()) ;
        send_status_14=0;
        Serial.println("Publish state4 OK");        
         publish_ok=true;
      } 
//      else {
//        Serial.println("Publish state4 NOK");        
//         publish_ok=false;
//      }
      send_status_04=0;
      send_status_12=0;
      send_status_13=0;
      send_status_14=0;
      return  publish_ok;
 #else
      if (mqttClient.publish((char*)pubTopic.c_str(), (char*) state.c_str())) {
        Serial.println("Publish state OK");        
        return true;
      }
     else {
        Serial.println("Publish state NOK");        
        return false;
      }
 #endif     
        
     } else {
         Serial.println("Publish state NOK");
         Serial.println("No MQTT connection.");        
     } 
    
}
