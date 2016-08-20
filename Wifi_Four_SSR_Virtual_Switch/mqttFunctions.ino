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

//--------------------------------------------------------------------"R4_ON")
if ((msgString == "R4_ON"))
  {
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_04));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      if(switch_status1==1)
      { 
      state_04=1;
      }
      else
      { 
      state_04=0;
      
      }
      send_status_04=0;
      
  } 
    else if ((msgString == "R4_OFF")){
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_04));    
      Serial.print("Switching light to "); 
      Serial.println("low");
       if(switch_status1==0)
      { 
      state_04=1;
      }
      else
      { 
      state_04=0;   
      }
      send_status_04=0;
      send_status_04=0;
      //digitalWrite(OUTPIN, 0); 
  }
//-----------------------------------------------------------------------------"R12_ON"

  if ((msgString == "R12_ON"))
  {
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_12));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      if(switch_status2==1)
      { 
      state_12=0;
      }
      else
      { 
      state_12=1;
      
      }
      send_status_12=1;
      
  } 
    else if ((msgString == "R12_OFF")){
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_12));    
      Serial.print("Switching light to "); 
      Serial.println("low");
       if(switch_status2==0)
      { 
      state_12=0;
      }
      else
      { 
      state_12=1;   
      }
      send_status_12=1;
      send_status_12=1;
      //digitalWrite(OUTPIN, 0); 
  }

//---------------------------------------------------------------------------"R13_ON")
if ((msgString == "R13_ON"))
  {
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_13));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      if(switch_status3==1)
      { 
      state_13=0;
      }
      else
      { 
      state_13=1;
      
      }
      send_status_13=1;
      
  } 
    else if ((msgString == "R13_OFF")){
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_13));    
      Serial.print("Switching light to "); 
      Serial.println("low");
       if(switch_status3==0)
      { 
      state_13=0;
      }
      else
      { 
      state_13=1;   
      }
      send_status_13=1;
      send_status_13=1;
      //digitalWrite(OUTPIN, 0); 
  }

//-------------------------------------------------------------------------------"R14_ON")

if ((msgString == "R14_ON"))
  {
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_14));      
      Serial.print("Switching light to "); 
      Serial.println("high");
      if(switch_status4==1)
      { 
      state_14=1;
      }
      else
      { 
      state_14=0;
      
      }
      send_status_14=0;
      
  } 
    else if ((msgString == "R14_OFF")){
      Serial.print("Light is ");
      Serial.println(digitalRead(OUTPIN_14));    
      Serial.print("Switching light to "); 
      Serial.println("low");
       if(switch_status4==0)
      { 
      state_14=1;
      }
      else
      { 
      state_14=0;   
      }
      send_status_14=0;
      send_status_14=0;
      //digitalWrite(OUTPIN, 0); 
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
