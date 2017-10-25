/*
 Basic MQTT example 
 
  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
char* topic = "esp8266_arduino_out";
const char *ssid =	"DLink";		// cannot be longer than 32 characters!
const char *pass =	"1234567890";		//

// Update these with values suitable for your network.
IPAddress server(192, 168, 1, 9);

void callback(const MQTT::Publish& pub) {
  // handle message arrived
}

WiFiClient wclient;
PubSubClient client(wclient, server,1883);

void setup() {
  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  client.set_callback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("arduinoClient")) {
	 if (client.publish(topic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
      }
    }

    if (client.connected())
      client.loop();
  }
}

