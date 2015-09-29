/*
Working code of basic Tcp server by me presently connecting to router
 */

#include <ESP8266WiFi.h>


const char* ssid = "";
const char* password = "";
const int Relay_Pin_04 = 4;
const int Relay_Pin_05 = 5;
const int Relay_Pin_12 = 12;
const int Relay_Pin_13 = 13;
const int Relay_Pin_14 = 14;
 
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO5
  pinMode(Relay_Pin_04, OUTPUT);
  pinMode(Relay_Pin_05, OUTPUT);
  pinMode(Relay_Pin_12, OUTPUT);
  pinMode(Relay_Pin_13, OUTPUT);
  pinMode(Relay_Pin_14, OUTPUT);
  
  digitalWrite(Relay_Pin_05, HIGH);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  //WiFi.begin(ssid, password);
  
 // while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }

  //if(WiFi.status() != WL_CONNECTED) 
  //{
  //delay(500);
  //Serial.println("Connecting");
  //if(WiFi.status() != WL_CONNECTED) 
  // {
  //   Serial.println("Failed");
  // }
 // }
 // else
  //{
  //  Serial.print("Connected");
 // }
  
  delay(500);
  //Serial.println("");
 // Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  //Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
 int waittime=0; 
 int over=50;

 WiFiClient client = server.available();
 if (!client) {
    return;
 }
  
  // Wait until the client sends some data
  Serial.println("new client");
  
while(!client.available())
{
   delay(1);
}  
 while(client.connected())
 {
  delay(1);
    
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("on01")!=-1)
  {
    val = 0;
    Serial.println("Relay01 is on ");
    client.write("01 is on\r\n");
     digitalWrite(Relay_Pin_04, HIGH);
    //client.write("invalid request");
    waittime=0; 
  }
  else if (req.indexOf("off01")!=-1)
  {
    val = 1;
    Serial.println("Relay01 is off ");
    client.write("01 is off\r\n");
    digitalWrite(Relay_Pin_04, LOW);
    waittime=0; 
  }
  if (req.indexOf("on02")!=-1)
  {
    val = 0;
    Serial.println("Relay02 is on ");
    client.write("02 is on\r\n");
     digitalWrite(Relay_Pin_12, HIGH);
    //client.write("invalid request");
    waittime=0; 
  }
  else if (req.indexOf("off02")!=-1)
  {
    val = 1;
    Serial.println("Relay02 is off ");
    client.write("02 is off\r\n");
    digitalWrite(Relay_Pin_12, LOW);
    waittime=0; 
  }
  if (req.indexOf("on03")!=-1)
  {
    val = 0;
    Serial.println("Relay03 is on ");
    client.write("03 is on\r\n");
     digitalWrite(Relay_Pin_13, HIGH);
    //client.write("invalid request");
    waittime=0; 
  }
  else if (req.indexOf("off03")!=-1)
  {
    val = 1;
    Serial.println("Relay03 is off ");
    client.write("03 is off\r\n");
    digitalWrite(Relay_Pin_13, LOW);
    waittime=0; 
  }
    if (req.indexOf("on04")!=-1)
  {
    val = 0;
    Serial.println("Relay04 is on ");
    client.write("03 is on\r\n");
     digitalWrite(Relay_Pin_14, HIGH);
    //client.write("invalid request");
    waittime=0; 
  }
  else if (req.indexOf("off04")!=-1)
  {
    val = 1;
    Serial.println("Relay04 is off ");
    client.write("04 is off\r\n");
    waittime=0; 
  }
    if (req.indexOf("ledon")!=-1)
  {
    val = 0;
    Serial.println("Led is on ");
    client.write("Led is on\r\n");
    digitalWrite(Relay_Pin_05, HIGH);
    //client.write("invalid request");
    waittime=0; 
  }
  else if (req.indexOf("ledoff")!=-1)
  {
    val = 1;
    Serial.println("Led is off ");
    client.write("Led is off\r\n");
    digitalWrite(Relay_Pin_05, LOW);
    waittime=0; 
  }
  else if (req.indexOf("status")!=-1)
  {
    Serial.println("Status Unknown ");
    client.write("Unknown\r\n");
   // client.stop();
    //return; 
    waittime=0; 
  }
  if(req.indexOf("SSID:")!=-1) //string format to connect it to your router sent "SSID:xxxx:yyyy:"
  { 
    int ssidlength=req.indexOf(":");
    int passlength=req.indexOf(":",ssidlength + 1);
    int passendlength=req.indexOf(":",passlength+ 2);
    int temppasswdlength=passendlength-passlength;
    int tempssidlength=passlength-ssidlength;
    String ssid_try=req.substring(ssidlength + 1,passlength);
    String passwd_try=req.substring(passlength + 1,passendlength);
    char ssid_new[30];
    char password_new[30];
    //Serial.println(ssid_try);
    //Serial.println(passwd_try);
    ssid_try.toCharArray(ssid_new,tempssidlength);
    passwd_try.toCharArray(password_new,temppasswdlength);
    delay(10);
    Serial.println(ssid_new);
    Serial.println(password_new);
     if(WiFi.status() != WL_CONNECTED) 
   {
     Serial.println("Trying to Connect");
     WiFi.begin(ssid_new, password_new);
   }
    Serial.println(ssid_new);
    Serial.println(password_new);
    delay(100);
    Serial.println(WiFi.localIP());
    client.write(WiFi.localIP());
    break;
   // client.stop();
    //return; 
    waittime=0; 
  }
  else 
  {
    //Serial.println("invalid request");
    //client.print("invalid request");
   // client.stop();
   //Serial.println("Doing Nothing");
   waittime++;
    //return;  
   if(waittime>=over)
   {
    client.write("Client Timeout Plz Reconnect\r\n");
    Serial.println("Client Timeout Plz Reconnect");
    break;
   }
   Serial.println(waittime);
   Serial.println("Doing Nothing");
  }
  
  // Set GPIO2 according to the request
  digitalWrite(5, val);
  client.flush();
  //server.available();
  if(server.available())
  {
    Serial.println("Server Available");
   client.stop();
   //return;
   break;
  }

}
 
  // Prepare the response
  //String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  //s += (val)?"high":"low";
  //s += "</html>\n";
  //String j;
  //j="ok";
  // Send the response to the client
  //client.print(j);
  //delay(1);
  if(WiFi.status() != WL_CONNECTED) 
   {
     Serial.println("Failed");
     //WiFi.begin(ssid_new, password_new);
   }
  //WiFi.begin(ssid_new, password_new);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

