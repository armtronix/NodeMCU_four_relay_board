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
  if (msgString == "R4_ON"){
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_04));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      digitalWrite(OUTPIN_04, 1); 
  } else if (msgString == "R4_OFF"){
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_04));    
      Serial.print("Switching light to "); 
      Serial.println("low");
      digitalWrite(OUTPIN_04, 0); 
  }
  if (msgString == "R12_ON"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTPIN_12));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      digitalWrite(OUTPIN_12, 1); 
  } else if (msgString == "R12_OFF"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTPIN_12));    
      Serial.print("Switching light to "); 
      Serial.println("low");
      digitalWrite(OUTPIN_12, 0); 
  }        
  if (msgString == "R13_ON"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTPIN_13));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      digitalWrite(OUTPIN_13, 1); 
  } else if (msgString == "R13_OFF"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTPIN_13));    
      Serial.print("Switching light to "); 
      Serial.println("low");
      digitalWrite(OUTPIN_13, 0); 
  }  
    if (msgString == "R14_ON"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTPIN_14));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      digitalWrite(OUTPIN_14, 1); 
  } else if (msgString == "R14_OFF"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTPIN_14));    
      Serial.print("Switching light to "); 
      Serial.println("low");
      digitalWrite(OUTPIN_14, 0); 
  }
    if (msgString == "R5_ON"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTLED));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      digitalWrite(OUTLED, 1); 
  } else if (msgString == "R5_OFF"){
      Serial.print("Led is ");
      Serial.println(digitalRead(OUTLED));    
      Serial.print("Switching light to "); 
      Serial.println("low");
      digitalWrite(OUTLED, 0); 
  }        
}

boolean pubState(){ //Publish the current state of the light    
  if (!connectMQTT()){
      delay(100);
      if (!connectMQTT){                            
        Serial.println("Could not connect MQTT.");
        Serial.println("Publish state NOK");
        return false;
      }
    }
    if (mqttClient.connected()){      
        //String state = (digitalRead(OUTPIN))?"1":"0";
        Serial.println("To publish state " + state );  
      if (mqttClient.publish((char*)pubTopic.c_str(), (char*) state.c_str())) {
        Serial.println("Publish state OK");        
        return true;
      } else {
        Serial.println("Publish state NOK");        
        return false;
      }
     } else {
         Serial.println("Publish state NOK");
         Serial.println("No MQTT connection.");        
     }    
}
