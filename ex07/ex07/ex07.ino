#include <WiFi.h>
#include <WebServer.h>
const int ledPin = 2;
WebServer server(80);
void handleBrightness() {
  int val = server.arg("value").toInt();
  ledcWrite(0, val); // PWM通道0输出对应亮度
  server.send(200, "text/plain", "OK");
}
void setup() {
  ledcSetup(0, 5000, 8); // PWM通道0，频率5kHz，分辨率8位
  ledcAttachPin(ledPin, 0);
  // WiFi连接与路由注册
  server.on("/setBrightness", handleBrightness);
  server.begin();
}
void loop() {
  server.handleClient();
}