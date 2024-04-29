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

// Web server port
const int port = 80;

// Define DEBUG macro for debugging
#define DEBUG

// Sensor instances
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
L3G gyro;
Adafruit_BMP085 bmp;

// Web server instance
WebServer server(port);

// Function to get sensor data as an array of floats
void getSensorData(float data[12]) {
  sensors_event_t accel_event;
  accel.getEvent(&accel_event);

  data[0] = accel_event.acceleration.x;
  data[1] = accel_event.acceleration.y;
  data[2] = accel_event.acceleration.z;

  gyro.read();
  data[3] = static_cast<float>(gyro.g.x);
  data[4] = static_cast<float>(gyro.g.y);
  data[5] = static_cast<float>(gyro.g.z);

  sensors_event_t mag_event;
  mag.getEvent(&mag_event);

  data[6] = mag_event.magnetic.x;
  data[7] = mag_event.magnetic.y;
  data[8] = mag_event.magnetic.z;

  data[9] = bmp.readTemperature();
  data[10] = bmp.readPressure();
  data[11] = bmp.readAltitude();
}

void handleData() {
  float data[12];
  getSensorData(data);

  server.setContentLength(sizeof(data));
  server.send(200, "application/octet-stream", (const char*)data);

#ifdef DEBUG
  Serial.print("Sent data: ");
  const uint8_t* bytePtr = (const uint8_t*)data;
  for (size_t i = 0; i < sizeof(data); i++) {
    Serial.print(bytePtr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#else
  Serial.println("Data sent");
#endif
}

// Setup and loop functions
void setup() {
  Serial.begin(115200);

  // WiFi setup
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected, IP address: " + WiFi.localIP().toString());

  // Sensor initialization
  if (!accel.begin() || !gyro.init() || !bmp.begin() || !mag.begin()) {
    Serial.println("Failed to initialize sensors");
    while (1);
  }

  // Web server setup
  server.on("/data", handleData);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
