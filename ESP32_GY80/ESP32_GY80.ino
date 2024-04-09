#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// WiFi credentials
const char* ssid = "Metronet1150";
const char* password = "1318995CX";
// const char* ssid = "Achilles' iPhone SE3";
// const char* password = "12345678";

// Web server port
const int port = 80;

// ADXL345 sensor
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Web server instance
WebServer server(port);

void setup() {
  // Serial port for debugging
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize ADXL345 sensor
  if (!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  // Set the range to whatever is appropriate for your project
  accel.setRange(ADXL345_RANGE_16_G);

  // Web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle web server requests
  server.handleClient();
}

void handleRoot() {
  // Send a simple HTML page with a link to the data endpoint
  String html = "<!DOCTYPE html><html><head><title>GY80 Data</title></head><body><h1>GY80 Data</h1><p><a href=\"/data\">Get data</a></p></body></html>";
  server.send(200, "text/html", html);
}

void handleData() {
  // Get sensor data
  sensors_event_t event;
  accel.getEvent(&event);

  // Create a JSON object with the data
  String json = "{";
  json += "\"x\": " + String(event.acceleration.x);
  json += ", \"y\": " + String(event.acceleration.y);
  json += ", \"z\": " + String(event.acceleration.z);
  json += "}";

  // Send the JSON data to the client
  server.send(200, "application/json", json);
  Serial.println(json);
}
