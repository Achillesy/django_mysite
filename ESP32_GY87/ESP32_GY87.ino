#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085.h>

// WiFi credentials
const char* ssid = "Metronet1150";
const char* password = "1318995CX";

// Web server port
const int port = 80;

Adafruit_MPU6050 mpu;

/* Assign a unique ID to this sensor at the same time */
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

Adafruit_BMP085 bmp;

// Web server instance
WebServer server(port);

void setup() {
  // Serial port for debugging
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());


  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  // Initialize the HMC5883L sensor
  if(!mag.begin()) {
    Serial.println("HMC5883L initialization failed!");
    while(1);
  }
  
  // Initialize the BMP180 sensor
  if(!bmp.begin()) {
    Serial.println("BMP180 initialization failed!");
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
  String html = "<!DOCTYPE html><html><head><title>GY87 Data</title></head><body><h1>GY87 Data</h1><p><a href=\"/data\">Get data</a></p></body></html>";
  server.send(200, "text/html", html);
}

void handleData() {
  // Get sensor data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  /* Get a new sensor event */ 
  sensors_event_t mag_event; 
  mag.getEvent(&mag_event);

  // Get temperature from BMP180
  float temperature = bmp.readTemperature();
    // Get pressure from BMP180
  float pressure = bmp.readPressure();
  
  // Create a JSON object with the data
  String json = "{";
  json += "\"acc_x\": " + String(a.acceleration.x);
  json += ", \"acc_y\": " + String(a.acceleration.y);
  json += ", \"acc_z\": " + String(a.acceleration.z);
  json += ", \"gyro_x\": " + String(g.gyro.x);
  json += ", \"gyro_y\": " + String(g.gyro.y);
  json += ", \"gyro_z\": " + String(g.gyro.z);
  json += ", \"mag_x\": " + String(mag_event.magnetic.x);
  json += ", \"mag_y\": " + String(mag_event.magnetic.y);
  json += ", \"mag_z\": " + String(mag_event.magnetic.z);
  json += ", \"temperature\": " + String(temperature);
  json += ", \"pressure\": " + String(pressure);
  json += "}";

  // Send the JSON data to the client
  server.send(200, "application/json", json);
  Serial.println(json);
}
