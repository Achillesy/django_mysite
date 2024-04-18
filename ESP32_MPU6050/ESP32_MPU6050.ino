#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// WiFi credentials
const char* apSSID = "MPU6050_AP"; // 默认的接入点名称
const char* apPassword = "mpu6050password"; // 接入点密码，可以根据需要更改

IPAddress localIP(192, 168, 10, 1); // 接入点的IP地址
IPAddress gateway(192, 168, 10, 1); // 网关
IPAddress subnet(255, 255, 255, 0); // 子网掩码

// Web server port
const int port = 80; // 默认的Web服务器端口

// Web server instance
WebServer server(port);

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  // 初始化MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  // 配置ESP32为AP
  WiFi.softAP(apSSID, apPassword);
  Serial.println("ESP32 configured as Access Point");

  // 设置ESP32作为接入点的IP配置
  WiFi.softAPConfig(localIP, gateway, subnet);

  // 获取本地IP地址
  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  // Web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);

  // 启动Web服务器
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // 处理Web服务器请求
  server.handleClient();
}

void handleRoot() {
  // 发送一个简单的HTML页面
  String html = "<!DOCTYPE html><html><head><title>MPU6050 Data</title></head><body><h1>MPU6050 Data</h1><p><a href=\"/data\">Get data</a></p></body></html>";
  server.send(200, "text/html", html);
}

void handleData() {
  // 读取传感器数据
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // 创建JSON对象并填充传感器数据
  String json = "{";
  json += "\"acc_x\": " + String(a.acceleration.x);
  json += ", \"acc_y\": " + String(a.acceleration.y);
  json += ", \"acc_z\": " + String(a.acceleration.z);
  json += ", \"gyro_x\": " + String(g.gyro.x);
  json += ", \"gyro_y\": " + String(g.gyro.y);
  json += ", \"gyro_z\": " + String(g.gyro.z);
  json += "}";

  // 发送JSON数据
  server.send(200, "application/json", json);
  Serial.println(json);
}
