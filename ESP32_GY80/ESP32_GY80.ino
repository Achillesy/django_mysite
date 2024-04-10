#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085.h>
#include <L3G.h>

// WiFi credentials
const char* ssid = "Metronet1150";
const char* password = "1318995CX";
// const char* ssid = "Achilles' iPhone SE3";
// const char* password = "12345678";

// Web server port
const int port = 80;

// ADXL345 sensor
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

L3G gyro;

Adafruit_BMP085 bmp;

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

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }
  gyro.enableDefault();

  if (!bmp.begin())
  {
      Serial.println("BMP085 sensor not found, check connections!");
      while (1);
  }

  /* Initialise the sensor */
  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

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
  gyro.read();
  /* Get a new sensor event */ 
  sensors_event_t mag_event; 
  mag.getEvent(&mag_event);

  // Create a JSON object with the data
  String json = "{";
  json += "\"accX\": " + String(event.acceleration.x);
  json += ", \"accY\": " + String(event.acceleration.y);
  json += ", \"accZ\": " + String(event.acceleration.z);
  json += ", \"gyroX\": " + String((int)gyro.g.x);
  json += ", \"gyroY\": " + String((int)gyro.g.y);
  json += ", \"gyroZ\": " + String((int)gyro.g.z);
  json += ", \"magX\": " + String(mag_event.magnetic.x);
  json += ", \"magY\": " + String(mag_event.magnetic.y);
  json += ", \"magZ\": " + String(mag_event.magnetic.z);
  json += ", \"Temperature\": " + String(bmp.readTemperature());
  json += ", \"Pressure\": " + String(bmp.readPressure());
  json += ", \"Altitude\": " + String(bmp.readAltitude());
  json += "}";

  // Send the JSON data to the client
  server.send(200, "application/json", json);
  Serial.println(json);
}
