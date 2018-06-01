/*
 * Date Edited : 02-05-18 alfa 001
 Edited By:Naren 

 Previous Edited Date : 
 Previously Edited By:
 
#define OUTPIN_04 4 //output pin for relay one
#define OUTPIN_12 12 //output pin for relay two
#define OUTPIN_13 13//output pin for relay three
#define OUTPIN_14 14 //output pin for relay four
#define OUTLED 2  // output pin for led

#define INPIN 0  // input pin (push button)
#define INPIN_04 33 //Virtual Switch one for relay one
#define INPIN_12 32 //Virtual Switch two for relay two
#define INPIN_13 35 //Virtual Switch three for relay three
#define INPIN_14 34 //Virtual Switch four  for relay four
 
 *  This sketch is running a web server for configuring WiFI if can't connect or for controlling of one GPIO to switch a light/LED
 *  Also it supports to change the state of the light via MQTT message and gives back the state after change.
 *  The push button has to switch to ground. It has following functions: Normal press less than 1 sec but more than 50ms-> Switch light. Restart press: 3 sec -> Restart the module. Reset press: 20 sec -> Clear the settings in EEPROM
 *  While a WiFi config is not set or can't connect:
 *    http://server_ip will give a config page with 
 *  While a WiFi config is set:
 *    http://server_ip/gpio -> Will display the GIPIO state and a switch form for it
 *    http://server_ip/gpio?state04=x -> Will change the GPIO directly and display the above aswell x will be 0-for off 1-for on
 *    http://server_ip/gpio?state12=x -> Will change the GPIO directly and display the above aswell x will be 0-for off 1-for on
 *    http://server_ip/gpio?state13=x -> Will change the GPIO directly and display the above aswell x will be 0-for off 1-for on
 *    http://server_ip/gpio?state14=x -> Will change the GPIO directly and display the above aswell x will be 0-for off 1-for on
 *    http://server_ip/gpio?state05=x -> Will change the GPIO directly and display the above aswell x will be 0-for off 1-for on
 *    MQTT Commands 
 *    R4_ON ,R4_OFF for on and off
 *    R12_ON ,R12_OFF for on and off
 *    R13_ON ,R13_OFF for on and off
 *    R14_ON ,R14_OFF for on and off
 *    R5_ON ,R5_OFF for on and off
 *    
 *    http://server_ip/cleareeprom -> Will reset the WiFi setting and rest to configure mode as AP
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected. (most likly it will be 192.168.4.1)
 * To force AP config mode, press button 20 Secs!
 *  For several snippets used, the credit goes to:
 *  - https://github.com/esp8266
 *  - https://github.com/chriscook8/esp-arduino-apboot
 *  - https://github.com/knolleary/pubsubclient
 *  - https://github.com/vicatcu/pubsubclient <- Currently this needs to be used instead of the origin
 *  - https://gist.github.com/igrr/7f7e7973366fc01d6393
 *  - http://www.esp8266.com/viewforum.php?f=25
 *  - http://www.esp8266.com/viewtopic.php?f=29&t=2745
 *  - And the whole Arduino and ESP8266 comunity
 *  S2 -Button is connected to Reset
 *  S1 -Button is connected to GPIO0
 */
String VERSION=" V0.101";
#define DEBUG
//#define WEBOTA
//debug added for information, change this according your needs

#define ESP32
//#define ESP8266

#ifdef ESP8266
#define RECEIVE_ATTR ICACHE_RAM_ATTR
#elifdef ESP32
#define RECEIVE_ATTR IRAM_ATTR
#else
#define RECEIVE_ATTR
#endif

#ifdef DEBUG
  #define Debug(x)    Serial.print(x)
  #define Debugln(x)  Serial.println(x)
  #define Debugf(...) Serial.printf(__VA_ARGS__)
  #define Debugflush  Serial.flush
#else
  #define Debug(x)    {}
  #define Debugln(x)  {}
  #define Debugf(...) {}
  #define Debugflush  {}
#endif


#include <Espalexa.h>

#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "SPIFFS.h"
#include <Update.h>
#include "WiFiUdp.h"
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
extern "C" {
  #include "user_interface.h" //Needed for the reset command
}
#endif


#include <WiFiClient.h>
//#include <EEPROM.h>
//#include <Ticker.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "FS.h"

void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);
void fourthLightChanged(uint8_t brightness);

//***** Settings declare ********************************************************************************************************* 
String hostName ="Armtronix"; //The MQTT ID -> MAC adress will be added to make it kind of unique
int iotMode=0; //IOT mode: 0 = Web control, 1 = MQTT (No const since it can change during runtime)
//select GPIO's
#ifdef ARDUINO_ARCH_ESP32
#define OUTPIN_04 4 //output pin for relay 1
#define OUTPIN_12 12 //output pin for relay 2
#define OUTPIN_13 13//output pin for relay 3
#define OUTPIN_14 14 //output pin for relay 4
#define OUTLED 2  // output pin for led

#define INPIN 0  // input pin (push button)
#define INPIN_04 33 //Virtual Switch one for relay one
#define INPIN_12 32 //Virtual Switch two for relay two
#define INPIN_13 35 //Virtual Switch three for relay three
#define INPIN_14 34 //Virtual Switch four  for relay four

#else
#define OUTPIN_04 4 //output pin
#define OUTPIN_12 12 //output pin
#define OUTPIN_13 13//output pin
#define OUTPIN_14 14 //output pin
//#define INPIN 0  // input pin (push button)
#define OUTLED 5
#endif
 
#define RESTARTDELAY 3 //minimal time in sec for button press to reset
#define HUMANPRESSDELAY 50 // the delay in ms untill the press should be handled as a normal push by human. Button debounce. !!! Needs to be less than RESTARTDELAY & RESETDELAY!!!
#define RESETDELAY 20 //Minimal time in sec for button press to reset all settings and boot to config mode

//##### Object instances ##### 
MDNSResponder mdns;

#ifdef ARDUINO_ARCH_ESP32
WebServer server(80);
#else
ESP8266WebServer server(80);
#endif
Espalexa espalexa; 


WiFiClient wifiClient;
PubSubClient mqttClient;
//Ticker btn_timer;
//Ticker otaTickLoop;

//##### Flags ##### They are needed because the loop needs to continue and cant wait for long tasks!
int rstNeed=0;   // Restart needed to apply new settings
int toPub=0; // determine if state should be published.
int configToClear=0; // determine if config should be cleared.
int otaFlag=0;
boolean inApMode=0;

int switch_status_04; //Physical state of the switch for relay 1
int switch_status_12; //Physical state of the switch for relay 2
int switch_status_13; //Physical state of the switch for relay 3
int switch_status_14; //Physical state of the switch for relay 4

int wifi_input_status_04; //wifi command state of the switch for relay 1
int wifi_input_status_12; //wifi command state of the switch for relay 2
int wifi_input_status_13; //wifi command state of the switch for relay 3
int wifi_input_status_14; //wifi command state of the switch for relay 4

int send_status_04; // Mqtt switch status publish flag 
int send_status_12;
int send_status_13;
int send_status_14;
//##### Global vars ##### 
int webtypeGlob;
int otaCount=300; //imeout in sec for OTA mode
int current; //Current state of the button
unsigned long count = 0; //Button press time counter
String st; //WiFi Stations HTML list
String state; //State of light
String state1; //State of relay 1
String state2; //State of relay 2
String state3; //State of relay 3
String state4; //State of relay 4
char buf[40]; //For MQTT data recieve
char* host; //The DNS hostname
//To be read from Config file
String esid="";
String epass = "";
String pubTopic;
String subTopic;
String mqttServer = "";
const char* otaServerIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";

/*Alexa event names */
boolean wifiConnected = false;
String firstName;
String secondName;
String thirdName;
String fourthName;



#ifdef ARDUINO_ARCH_ESP32
#include "esp_system.h"
//String getMacAddress() {
//  uint8_t baseMac[6];
//  // Get MAC address for WiFi station
//  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
//  char baseMacChr[18] = {0};
//  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
//  return String(baseMacChr);
//}
#else 
#endif



#ifdef ARDUINO_ARCH_ESP32
static volatile uint16_t intTriggeredCount=0;
void IRAM_ATTR isr(){  //IRAM_ATTR tells the complier, that this code Must always be in the 
// ESP32's IRAM, the limited 128k IRAM.  use it sparingly.
intTriggeredCount++;
//input_int;
}
#else 
#endif


//-------------- void's -------------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  WiFi.printDiag(Serial);
  //WiFi.disconnect();
  delay(100);
#ifdef ARDUINO_ARCH_ESP32
   pinMode(INPIN_04, INPUT);
   pinMode(INPIN_12, INPUT);
   pinMode(INPIN_13, INPUT);
   pinMode(INPIN_14, INPUT);
#else 
#endif
  // prepare GPIO2
  pinMode(OUTPIN_04, OUTPUT);
  digitalWrite(OUTPIN_04, LOW);
  pinMode(OUTPIN_12, OUTPUT);
  digitalWrite(OUTPIN_12, LOW);
  pinMode(OUTPIN_13, OUTPUT);
  digitalWrite(OUTPIN_13, LOW);
  pinMode(OUTPIN_14, OUTPUT);
  digitalWrite(OUTPIN_14, LOW);
  delay(100);
  pinMode(OUTLED, OUTPUT);
  digitalWrite(OUTLED, LOW);
  digitalWrite(OUTLED, HIGH);
  
//  btn_timer.attach(0.05, btn_handle);
  Debugln("DEBUG: Entering loadConfig()");
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
  }
  
  uint8_t mac[6];
  #ifdef ARDUINO_ARCH_ESP32
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  #else
  WiFi.macAddress(mac);
  #endif
  hostName += "-";
  hostName += macToStr(mac);
  String hostTemp=hostName;
  hostTemp.replace(":","-");
  host = (char*) hostTemp.c_str();
  loadConfig();
  //loadConfigOld();
  Debugln("DEBUG: loadConfig() passed");
  
  // Connect to WiFi network
  Debugln("DEBUG: Entering initWiFi()");
  initWiFi();
  Debugln("DEBUG: initWiFi() passed");
  Debug("iotMode:");
  Debugln(iotMode);
  Debug("webtypeGlob:");
  Debugln(webtypeGlob);
  Debug("otaFlag:");
  Debugln(otaFlag);
  Debugln("DEBUG: Starting the main loop");
  //Alexa Part
   if(wifiConnected){
    server.on("/", HTTP_GET, [](){
    server.send(200, "text/plain", "This is an example index page your server may send.");
    });
    server.on("/test", HTTP_GET, [](){
    server.send(200, "text/plain", "This is a second subpage you may have.");
    });
    server.onNotFound([](){
      if (!espalexa.handleAlexaApiCall(server.uri(),server.arg(0))) //if you don't know the URI, ask espalexa whether it is an Alexa control request
      {
        //whatever you want to do with 404s
        server.send(404, "text/plain", "Not found");
      }
    });
  }
  // Define your devices here.
  espalexa.addDevice((char*)firstName.c_str(), firstLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)secondName.c_str(), secondLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)thirdName.c_str(), thirdLightChanged); //simplest definition, default state off
  espalexa.addDevice((char*)fourthName.c_str(), fourthLightChanged); //simplest definition, default state off

  espalexa.begin(&server); //give espalexa a pointer to your server object so it can use your server instead of creating its own
 
}


//void btn_handle()
//{
//  if(!digitalRead(INPIN)){
//    ++count; // one count is 50ms
//  } else {
//    if (count > 1 && count < HUMANPRESSDELAY/5) { //push between 50 ms and 1 sec      
//      Serial.print("button pressed "); 
//      Serial.print(count*0.05); 
//      Serial.println(" Sec."); 
//    
//      Serial.print("Light is ");
//      Serial.println(digitalRead(OUTPIN_04));
//      
//      Serial.print("Switching light to "); 
//      Serial.println(!digitalRead(OUTPIN_04));
//      digitalWrite(OUTPIN_04, !digitalRead(OUTPIN_04)); 
//      state = digitalRead(OUTPIN_04);
//      if(iotMode==1 && mqttClient.connected()){
//        toPub=1;        
//        Debugln("DEBUG: toPub set to 1");
//      }
//    } else if (count > (RESTARTDELAY/0.05) && count <= (RESETDELAY/0.05)){ //pressed 3 secs (60*0.05s)
//      Serial.print("button pressed "); 
//      Serial.print(count*0.05); 
//      Serial.println(" Sec. Restarting!"); 
//      setOtaFlag(!otaFlag);      
//     #ifdef ARDUINO_ARCH_ESP32
//           esp_restart();
//           #else
//           ESP.reset();
//           #endif 
//    } else if (count > (RESETDELAY/0.05)){ //pressed 20 secs
//      Serial.print("button pressed "); 
//      Serial.print(count*0.05); 
//      Serial.println(" Sec."); 
//      Serial.println(" Clear settings and resetting!");       
//      configToClear=1;
//      }
//    count=0; //reset since we are at high
//  }
//}



//-------------------------------- Main loop ---------------------------
void loop() 
{

#ifdef ARDUINO_ARCH_ESP32

/*##########  Virtual Switch One Status Update  ######################################################*/
  if(switch_status_04==(!digitalRead(INPIN_04)))// to read the status of physical switch
     {
        // send_status=0;
     }
  else
     {
      switch_status_04=(!digitalRead(INPIN_04));
      Serial.println("Status 04 ");
      send_status_04=1;
     }
     
/*##########  Virtual Switch Two Status Update  ######################################################*/
  if(switch_status_12==(!digitalRead(INPIN_12)))// to read the status of physical switch
     {
        // send_status=0;
     }
  else
     {
      switch_status_12=(!digitalRead(INPIN_12));
      Serial.println("Status 12 ");
      send_status_12=1;
     }

/*##########  Virtual Switch Three Status Update  ######################################################*/
  if(switch_status_13==(!digitalRead(INPIN_13)))// to read the status of physical switch
     {
        // send_status=0;
     }
  else
     {
      switch_status_13=(!digitalRead(INPIN_13));
       Serial.println("Status 13 ");
      send_status_13=1;
     }

/*##########  Virtual Switch Four Status Update  ######################################################*/
  if(switch_status_14==(!digitalRead(INPIN_14)))// to read the status of physical switch
     {
        // send_status=0;
     }
  else
     {
      switch_status_14=(!digitalRead(INPIN_14));
       Serial.println("Status 14 ");
      send_status_14=1;
     }


/*##########  Relay 1/Output 4  Status Update  ######################################################*/
  if(((wifi_input_status_04)&&(!switch_status_04))||((!wifi_input_status_04)&&(switch_status_04)))  //exor logic
      {
       digitalWrite(OUTPIN_04, HIGH);
       state1 = "R04isON";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, HIGH));   
      }
      else
      {
       digitalWrite(OUTPIN_04, LOW);
       state1 = "R04isOFF";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, LOW));
      }

/*##########  Relay 2/Output 12  Status Update  ######################################################*/
  if(((wifi_input_status_12)&&(!switch_status_12))||((!wifi_input_status_12)&&(switch_status_12)))  //exor logic
      {
       digitalWrite(OUTPIN_12, HIGH);
       state2 = "R12isON";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, HIGH));   
      }
      else
      {
       digitalWrite(OUTPIN_12, LOW);
       state2 = "R12isOFF";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, LOW));
      }

/*##########  Relay 3/Output 13  Status Update  ######################################################*/
  if(((wifi_input_status_13)&&(!switch_status_13))||((!wifi_input_status_13)&&(switch_status_13)))  //exor logic
      {
       digitalWrite(OUTPIN_13, HIGH);
       state3 = "R13isON";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, HIGH));   
      }
      else
      {
       digitalWrite(OUTPIN_13, LOW);
       state3 = "R13isOFF";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, LOW));
      }

/*##########  Relay 4/Output 14  Status Update  ######################################################*/
  if(((wifi_input_status_14)&&(!switch_status_14))||((!wifi_input_status_14)&&(switch_status_14)))  //exor logic
      {
       digitalWrite(OUTPIN_14, HIGH);
       state4 = "R14isON";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, HIGH));   
      }
      else
      {
       digitalWrite(OUTPIN_14, LOW);
       state4 = "R14isOFF";//added on 17/05/18
       //Serial.print("Light switched via web request to  ");
       //Serial.println(digitalWrite(OUTPIN, LOW));
      }

/*##########  Mqtt switch status publish   ######################################################*/ 
 
  if (send_status_04 == 1 || send_status_12 == 1 || send_status_13 == 1 || send_status_14 == 1)
  {
    //send_status_04 = 0;
    toPub = 1;
  }
  else
  {
    toPub = 0;
  }

  
#else
#endif







  
 //Debugln("DEBUG: loop() begin");
  if(configToClear==1)
  {
    //Debugln("DEBUG: loop() clear config flag set!");
    clearConfig()? Serial.println("Config cleared!") : Serial.println("Config could not be cleared");
    delay(1000);
   #ifdef ARDUINO_ARCH_ESP32
           esp_restart();
           #else
           ESP.reset();
           #endif 
  }
  //Debugln("DEBUG: config reset check passed");  
  if (WiFi.status() == WL_CONNECTED && otaFlag)
  {
    if(otaCount<=1) 
    {
     Serial.println("OTA mode time out. Reset!"); 
     setOtaFlag(0);
     #ifdef ARDUINO_ARCH_ESP32
     esp_restart();
     #else
     ESP.reset();
     #endif 
     delay(100);
    }
    server.handleClient();
    delay(1);   
  } 
  else if (WiFi.status() == WL_CONNECTED || webtypeGlob == 1)
  {
     //Debugln("DEBUG: loop - webtype =1"); 
    //Debugln("DEBUG: loop() wifi connected & webServer ");
    if (iotMode==0 || webtypeGlob == 1)
    {
      //Debugln("DEBUG: loop() Web mode requesthandling ");
      server.handleClient();
      delay(1);
      if(esid != "" && WiFi.status() != WL_CONNECTED) //wifi reconnect part
      {
        Scan_Wifi_Networks();
      }
    } 
    else if (iotMode==1 && webtypeGlob != 1 && otaFlag !=1)
    {
          //Debugln("DEBUG: loop() MQTT mode requesthandling ");
          if (!connectMQTT()){
              delay(200);          
          }                    
          if (mqttClient.connected()){            
              //Debugln("mqtt handler");
              mqtt_handler();
          } else {
              Debugln("mqtt Not connected!");
          }
    }
  } 
  else
  {
    Debugln("DEBUG: loop - WiFi not connected function initWifi()");  
    delay(1000);
    initWiFi(); //Try to connect again
  }
  espalexa.loop();
   delay(1);
 //Debugln("DEBUG: loop() end");
}

//our callback functions
void firstLightChanged(uint8_t brightness) 
{
    Serial.println("Device 1 changed to ");
    if (brightness == 255) 
     {
      Serial.println("High");
      if(switch_status_04==1)
       { 
        wifi_input_status_04=0;
       }
      else
       { 
        wifi_input_status_04=1;
       }
      send_status_04=1;
     }
    else if(brightness == 0) 
     {
      Serial.println("LOW");
      if(switch_status_04==0)
       { 
        wifi_input_status_04=0;
       }
      else
       { 
        wifi_input_status_04=1;
       }
     send_status_04=1;
    }


}

/*##################################Callback functions for Second Relay #########################*/
void secondLightChanged(uint8_t brightness) {
 Serial.println("Device 2 changed to ");
    if (brightness == 255) 
     {
      Serial.println("High");
      if(switch_status_12==1)
       { 
        wifi_input_status_12=0;
       }
      else
       { 
        wifi_input_status_12=1;
       }
      send_status_12=1;
     }
    else if(brightness == 0) 
     {
      Serial.println("LOW");
      if(switch_status_12==0)
       { 
        wifi_input_status_12=0;
       }
      else
       { 
        wifi_input_status_12=1;
       }
     send_status_12=1;
    }

}

/*##################################Callback functions for Third Relay #########################*/
void thirdLightChanged(uint8_t brightness) {
Serial.println("Device 3 changed to ");
    if (brightness == 255) 
     {
      Serial.println("High");
      if(switch_status_13==1)
       { 
        wifi_input_status_13=0;
       }
      else
       { 
        wifi_input_status_13=1;
       }
      send_status_13=1;
     }
    else if(brightness == 0) 
     {
      Serial.println("LOW");
      if(switch_status_13==0)
       { 
        wifi_input_status_13=0;
       }
      else
       { 
        wifi_input_status_13=1;
       }
     send_status_13=1;
    }

}

/*##################################Callback functions for Fourth Relay #########################*/
void fourthLightChanged(uint8_t brightness) 
{
Serial.println("Device 4 changed to ");
    if (brightness == 255) 
     {
      Serial.println("High");
      if(switch_status_14==1)
       { 
        wifi_input_status_14=0;
       }
      else
       { 
        wifi_input_status_14=1;
       }
      send_status_14=1;
     }
    else if(brightness == 0) 
     {
      Serial.println("LOW");
      if(switch_status_14==0)
       { 
        wifi_input_status_14=0;
       }
      else
       { 
        wifi_input_status_14=1;
       }
     send_status_14=1;
    }

}

