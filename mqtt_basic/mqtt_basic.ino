/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
char* topic = "Relay_Control";
const char *ssid =  "xxxx";   // cannot be longer than 32 characters!
const char *pass =  "xxxx";   //


// Update these with values suitable for your network.
//IPAddress server(192, 168, 1, 1);example
IPAddress server(192, 168, 3, 1);///need to change this asper ur broker

WiFiClient wclient;
PubSubClient client(wclient, server,1883);
const int Relay_Pin_04 = 4;
const int Relay_Pin_05 = 5;
const int Relay_Pin_12 = 12;
const int Relay_Pin_13 = 13;
const int Relay_Pin_14 = 14;

#define BUFFER_SIZE 100

void callback(const MQTT::Publish& pub) {
  //Serial.print(pub.topic());
  //Serial.print(" => ");
  Serial.println(pub.payload_string());
  String command = pub.payload_string();

  if(command.equals("on01")){
    digitalWrite(Relay_Pin_04, HIGH);
    Serial.println("Relay 1 is on");
    client.publish(topic, "Relay 1 is on");
  }
  if(command.equals("off01")){
    digitalWrite(Relay_Pin_04, LOW);
    Serial.println("Relay 1 is off");
    client.publish(topic, "Relay 1 is off");
  }
  if(command.equals("on02")){
    digitalWrite(Relay_Pin_12, HIGH);
    Serial.println("Relay 2 is on");
    client.publish(topic, "Relay 2 is on");
  }
  if(command.equals("off02")){
    digitalWrite(Relay_Pin_12, LOW);
    Serial.println("Relay 2 is off");
    client.publish(topic, "Relay 2 is off");
  }
  if(command.equals("on03")){
    digitalWrite(Relay_Pin_13, HIGH);
    Serial.println("Relay 3 is on");
    client.publish(topic, "Relay 3 is on");
  }
  if(command.equals("off03")){
    digitalWrite(Relay_Pin_13, LOW);
    Serial.println("Relay 3 is off");
    client.publish(topic, "Relay 3 is off");
  }
  if(command.equals("on04")){
    digitalWrite(Relay_Pin_14, HIGH);
    Serial.println("Relay 4 is on");
    client.publish(topic, "Relay 4 is on");
  }
  if(command.equals("off04")){
    digitalWrite(Relay_Pin_14, LOW);
    Serial.println("Relay 4 is off");
    client.publish(topic, "Relay 4 is off");
  }
  if(command.equals("ledon")){
    digitalWrite(Relay_Pin_05, HIGH);
    Serial.println("Led is on");
    client.publish(topic, "Led is on");
  }
  if(command.equals("ledoff")){
    digitalWrite(Relay_Pin_05, LOW);
    Serial.println("Led is off");
    client.publish(topic, "Led is off");
  }
  if(command.equals("PIR HIGH\n\r")){
    digitalWrite(Relay_Pin_14, HIGH);
    Serial.println("Relay 4 is on");
    client.publish(topic, "Relay 4 is on");
  }
  if(command.equals("PIR LOW\n\r")){
    digitalWrite(Relay_Pin_14, LOW);
    Serial.println("Relay 4 is off");
    client.publish(topic, "Relay 4 is off");
  }
  Serial.println("End of callback");
}

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  pinMode(Relay_Pin_04, OUTPUT);
  pinMode(Relay_Pin_05, OUTPUT);
  pinMode(Relay_Pin_12, OUTPUT);
  pinMode(Relay_Pin_13, OUTPUT);
  pinMode(Relay_Pin_14, OUTPUT);
  
  digitalWrite(Relay_Pin_05, HIGH);
  //client.set_callback(callback);
  //WiFi.status()
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
   
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      return;
    }
      
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("Wifi Four Relay Board")) {
        client.set_callback(callback);
        client.subscribe(topic);
        client.publish(topic, "Control your relays by sending onxx or offxx commands xx can be 01,02,03,04");
	// if (client.publish(topic, "hello from ESP8266")) {
   //   Serial.println("Publish ok");
   // }
    //else {
    //  Serial.println("Publish failed");
   // }
      }
    }

    if (client.connected())
      client.loop();
  }
}

