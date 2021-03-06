/*
	AlexaTVRemoteForESP8266
	By Richard Audette

	A merger of the Esp8266 Websockets Client
	By Gil Maimon
	https://github.com/gilmaimon/ArduinoWebsockets

	And:
	The IRSend Pronto code demo:
	https://github.com/crankyoldgit/IRremoteESP8266

	This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server running AlexaTVRemoteServer
        3. Sends the websockets server a message ("Hello Server")
        4. Sends IR commands when certain messages are sent back from the servers

	Hardware:
        For this sketch you need an ESP8266 board with an infrared LED connected to Pin 4.

	My TV happens to be a Toshiba.  Get codes for your TV from: http://irdb.tk/

*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

//wifi and web socket stuff
const char* ssid = "wifissid"; //Enter SSID
const char* password = "wifipassword"; //Enter Password
const char* websockets_server_host = "serverhostingAlexaRemoteService.js"; //Enter server adress
const uint16_t websockets_server_port = 8080; // Enter server port

//IR stuff
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

//from http://irdb.tk/
uint16_t toshibaPwrProntoCode[104] = {
    0x0000, 0x006C, 0x0022, 0x0002, 0x015B, 0x00AD, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016,
    0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0016,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x05F7,
    0x015B, 0x0057, 0x0016, 0x0E6C};

uint16_t toshibaVolUpProntoCode[104] = {
    0x0000, 0x006C, 0x0022, 0x0002, 0x015B, 0x00AD, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016,
    0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0041, 
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0041, 
    0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0016, 
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x05F7, 
    0x015B, 0x0057, 0x0016, 0x0E6C};

uint16_t toshibaVolDownProntoCode[104] = {
    0x0000, 0x006C, 0x0022, 0x0002, 0x015B, 0x00AD, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x05F7,
    0x015B, 0x0057, 0x0016, 0x0E6C
};

uint16_t toshibaMuteProntoCode[104] = {
    0x0000, 0x006C, 0x0022, 0x0002, 0x015B, 0x00AD, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0016, 0x0016, 0x0041, 0x0016, 0x0016,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0016, 0x0041,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0016,
    0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x0041, 0x0016, 0x05F7,
    0x015B, 0x0057, 0x0016, 0x0E6C
};


using namespace websockets;

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    irsend.begin();
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    // try to connect to Websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/");
    if(connected) {
        Serial.println("Connected!");
        client.send("Hello Server");
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message) {
        Serial.print("Got Message: ");
        Serial.println(message.data());
        String strMessage = message.data();
        Serial.println(strMessage);
        if (strMessage.indexOf("volup")>=0) {
          Serial.println("got volup");
          irsend.sendPronto(toshibaVolUpProntoCode, 104);
          }
        if (strMessage.indexOf("voldown")>=0) {
          Serial.println("got voldown");
          irsend.sendPronto(toshibaVolDownProntoCode, 104);
          }
        if (strMessage.indexOf("power")>=0) {
          Serial.println("got power");
          irsend.sendPronto(toshibaPwrProntoCode, 104);
          }
        if (strMessage.indexOf("mute")>=0) {
          Serial.println("got mute");
          irsend.sendPronto(toshibaMuteProntoCode, 104);
          }
    });
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }
    delay(500);
}
