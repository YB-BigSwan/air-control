#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <IRremote.h>
#include <HTTPClient.h>


const uint16_t kIrLedPin = 21; 

const char* ssid = "( ͡° ͜ʖ ͡°)";
const char* password = "OnePingToRuleThemAll";

AsyncWebServer server(80);

#define POWER_ON  0x118C0000800040AA
#define POWER_OFF 0x118C0000800000AA
#define FAN_LOW   0x1880000800040AA
#define FAN_MED   0x1840000800040AA
#define FAN_HIGH  0x18C0000800040AA
#define TEMP_20C  0x18C0000900042AA
#define TEMP_18C  0x18C0000A00044AA
#define TEMP_16C  0x18C0000800040AA
#define SWING_ON  0x18C00008000C0AA
#define SWING_OFF 0x18C0000800040AA

void registerWithServer() {
  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi not connected!");
      return;
  }

  WiFiClient client;
  HTTPClient http;

  http.setTimeout(30000); 
  client.setTimeout(30000); 
  // Set your Heroku server's URL
  String serverUrl = "http://air-control-25b7322984ae.herokuapp.com/register";
  http.begin(client, serverUrl);

  // Create JSON data with the ESP32's IP address
  String jsonData = "{\"ip\":\"" + WiFi.localIP().toString() + "\"}";
  http.addHeader("Content-Type", "application/json");

  delay(2000);

  // Send the POST request
  int httpResponseCode = http.sendRequest("POST", jsonData);

  // Handle the response
  if (httpResponseCode > 0) {
    Serial.printf("ESP32 registered with server: %d\n", httpResponseCode);
  } else {
    Serial.printf("Failed to register with server: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();  // Close the connection
}

void setup() {
  Serial.begin(115200);

 // Initialize the IR sender
  IrSender.begin(kIrLedPin);
  Serial.println("Initialized IR emitter");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Register with the Heroku server after connecting to WiFi
  Serial.println("Server registration attempted!");
  registerWithServer();
  

  // Set up API routes
  server.on("/poweron", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {POWER_ON, 0x88};
    IrSender.sendPulseDistanceWidthFromArray(38, 11950, 5150, 600, 2000, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Power ON sent");
    Serial.println("Signal sent!");
  });

  server.on("/poweroff", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {POWER_OFF, 0xF0};
    IrSender.sendPulseDistanceWidthFromArray(38, 12100, 5050, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Power OFF sent");
    Serial.println("Signal sent!");
  });

  server.on("/fanLow", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {FAN_LOW, 0x97};
    IrSender.sendPulseDistanceWidthFromArray(38, 12100, 5050, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Fan low sent");
    Serial.println("Signal sent!");
  });

    server.on("/fanMed", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {FAN_MED, 0x9F};
    IrSender.sendPulseDistanceWidthFromArray(38, 12050, 5050, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Fan med sent");
    Serial.println("Signal sent!");
  });


  server.on("/fanHigh", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {FAN_HIGH, 0x90};
    IrSender.sendPulseDistanceWidthFromArray(38, 12100, 5050, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Fan high sent");
    Serial.println("Signal sent!");
  });


  server.on("/20c", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {TEMP_20C, 0x8A};
    IrSender.sendPulseDistanceWidthFromArray(38, 12100, 5100, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Temperature set to 20°C");
    Serial.println("Signal sent!");
  });

  server.on("/18c", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {TEMP_18C, 0xB4};
    IrSender.sendPulseDistanceWidthFromArray(38, 12100, 5050, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Temperature set to 18°C");
    Serial.println("Signal sent!");
  });

  server.on("/16c", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {TEMP_16C, 0x90};
    IrSender.sendPulseDistanceWidthFromArray(38, 12050, 5100, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Temperature set to 16°C");
    Serial.println("Signal sent!");
  });

  server.on("/swingon", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {SWING_ON, 0x50};
    IrSender.sendPulseDistanceWidthFromArray(38, 12000, 5150, 600, 1950, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Swing ON sent");
    Serial.println("Signal sent!");
  });

  server.on("/swingoff", HTTP_GET, [](AsyncWebServerRequest *request){
    uint64_t tRawData[] = {SWING_OFF, 0x90};
    IrSender.sendPulseDistanceWidthFromArray(38, 12050, 5100, 600, 1900, 600, 450, &tRawData[0], 72, PROTOCOL_IS_LSB_FIRST, 1000, 0);
    request->send(200, "text/plain", "Swing OFF sent");
    Serial.println("Signal sent!");
  });

  // Start the server
  server.begin();
  Serial.println("Web server started!");
}

void loop() {
  
}