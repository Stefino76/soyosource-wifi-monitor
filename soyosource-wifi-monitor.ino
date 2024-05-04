/*
  Stefano Baccetti
  With this sketch you can read the SoyoSource data via internet.
  The sketch use event for update data in browser without need to reload the page.
  The sketch is tested with an ESP32 board
  Last update 04/2024
*/

#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <esp_wifi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "html.h"

int data[16];
int byteIdx = 0;

// Timer variables
unsigned long now ;
unsigned long lastSendTime = 0;  
unsigned long lastReadTime = 0;  
unsigned long timerSendDelay = 3000;

JsonDocument dataJson;

// Replace with your network credentials
const char* ssid = "WIFI-NAME";
const char* password = "WIFI-PASSWORD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

//Initialize Serial(D2) read only 
HardwareSerial LCD(2);

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void saveToDataJson(int data[]) {
  dataJson["pw_req"] = static_cast<uint16_t>(data[1]) * 256 + data[2];
  dataJson["mode"] = data[3];
  dataJson["error"] = data[4];
  dataJson["bt_v"] = (static_cast<uint16_t>(data[5]) * 256 + data[6])*0.1;
  dataJson["bt_a"] = (static_cast<uint16_t>(data[7]) * 256 + data[8])*0.1;
  dataJson["ac_v"] = static_cast<uint16_t>(data[9]) * 256 + data[10];
  dataJson["ac_hz"] = (int)data[11]/2;
  dataJson["temp"] = ((static_cast<uint16_t>(data[12]) * 256 + data[13])-300)*0.1;
}

void setup() {

  Serial.begin(9600);
  Serial.println("configuring serial:\n");
  LCD.begin(9600);    // Read display

  initWiFi();

   // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();
}

void loop() {
  now = millis();

  //Lettura Seriale LCD
  if (LCD.available()) {
    int byte = LCD.read();  // Read the byte
    if (now - lastReadTime < 100 && byteIdx < 16) {
      data[byteIdx] = byte;
      byteIdx++;
    } else {
      data[0] = byte;
      if (byteIdx == 15) { //The entire message was readed
        saveToDataJson(data);
      }
      byteIdx = 1;
    } 
    lastReadTime = millis();
  }

  // Send the dataJson string to the Client by Events 
  if ((millis() - lastSendTime) > timerSendDelay) {
    String dataToSend;
    serializeJson(dataJson, dataToSend);
    Serial.println(dataToSend);
    events.send(dataToSend.c_str(),"data",millis());
    lastSendTime = millis();
  }

}
