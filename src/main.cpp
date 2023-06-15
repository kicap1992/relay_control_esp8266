#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <Arduino_JSON.h>
#include <ESP8266WebServer.h>

const char *ssid = "KARAN";
const char *password = "12345679";

ESP8266WebServer server(80);

int GPIO_pin = D5;

bool relayState = false;

void handleRoot() {
  server.send(200, "text/plain", "Hello from ESP8266!");
}

void handleRelayOn() {
  digitalWrite(GPIO_pin, HIGH);
  relayState = true;
  JSONVar response;
  response["status"] = "success";
  response["message"] = "Relay turned on";
  String jsonString = JSON.stringify(response);
  server.send(200, "application/json", jsonString);

}

void handleRelayOff() {
  digitalWrite(GPIO_pin, LOW);
  relayState = false;
  JSONVar response;
  response["status"] = "success";
  response["message"] = "Relay turned off";
  String jsonString = JSON.stringify(response);
  server.send(200, "application/json", jsonString);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin as an output
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  pinMode(GPIO_pin, OUTPUT); // Set the GPIO pin as an output
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  // check the IP address
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, LOW); // Turn the LED off
  server.on("/", handleRoot);
  server.on("/relay/on", handleRelayOn);
  server.on("/relay/off", handleRelayOff);
  server.begin();
}

void loop() {
  server.handleClient();
}