# Modified ESP WiFiSwitch code for node mcu four relay board 
#credit https://github.com/biohazardxxx/ESP_WiFiSwitch
## General info
This sketch is for a WiFi enabled wall light switch with focus to reliable pushbutton switch.
In the beginning or (if no WiFi connection) it is running a web server to configure WiFI (and MQTT if desired). 
Each second start up switch will load into OTA mode. By this you can upload a new firmware (compiled *.bin file) via web browser. The OTA mode will end after set timeout and restart into desired mode.
The operation mode can be web server or MQTT to change the state of the light.
The push button have to switch to ground.
## Button functions
* Normal press less than 1 sec but more than 50ms-> Switch light.
* Restart press: 3 sec -> Restart the module.
* Reset press: 20 sec -> Clear the settings in EEPROM

## URL adresses
* While a WiFi config is not set or can't connect:
   * http://server_ip
   *-> Gives a WiFi config page 
  
* While a WiFi config is set and in Web control mode (iotMode==0):
 * http://server_ip/gpio
  * Will display the GIPIO state and a switch form for it
  
 * http://server_ip/gpio?state=0

  * Will change the GPIO directly and display the above aswell
  
 * http://server_ip/cleareeprom 
  * Will reset the WiFi setting and rest to configure mode as AP
  * 
* While in OTA mode each second start:
   * http://server_ip
   *-> Gives a WiFi config page 
  
<b>server_ip</b> is the IP address of the ESP8266 module, will be printed to Serial when the module is connected.

## Hardware
http://armtronix.net/wifi%20nodemcu%20four%20relay%20board.html
## Setup and other instructions 
http://www.instructables.com/id/NodeMCU-ESP8266-Based-Switch-Board-with-Raw-TCPIP-/



## Usage
<b>For default usage you can use the pre build firmware and flash it with NodeMcu flasher.</b>

After fresh flash please restart the module manualy (power Off & On) otherwise software restart will not work and sometimes WiFi connect does not work.

When manually compile and flash with Arduino IDE please make sure to have this flash settings: ![fLashSetting](https://github.com/biohazardxxx/ESP_WiFiSwitch/blob/master/other/flashSettings.png)

Open the modules page after entering config mode (Press button >20secs if you want to enter again) via http://server_ip there you can setup to be MQTT controled or Web controled.

## Credits
For several snippets used the credit goes to:
 - https://github.com/esp8266
 - https://github.com/chriscook8/esp-arduino-apboot
 - https://github.com/knolleary/pubsubclient
 - https://github.com/vicatcu/pubsubclient <- Currently this needs to be used instead of the origin
 - https://gist.github.com/igrr/7f7e7973366fc01d6393
 - http://www.esp8266.com/viewforum.php?f=25
 - http://www.esp8266.com/viewtopic.php?f=29&t=2745
 - And the whole Arduino and ESP8266 comunity
