#include <WiFi.h>
#include <WebServer.h>
const int touchPin = T0; // 触摸引脚T0(GPIO4)
const int ledPin = 2;
WebServer server(80);
bool isArmed = false;
bool isAlarming = false;

void handleState() {
  String cmd = server.arg("cmd");
  if (cmd == "arm") {
    isArmed = true;
    isAlarming = false;
  } else if (cmd == "disarm") {
    isArmed = false;
    isAlarming = false;
    digitalWrite(ledPin, LOW);
  }
  server.send(200, "text/plain", "OK");
}

void checkTouch() {
  if (isArmed && !isAlarming && touchRead(touchPin) < 40) { // 触摸阈值
    isAlarming = true;
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  // WiFi连接与路由注册
  server.on("/setState", handleState);
  server.begin();
}

void loop() {
  server.handleClient();
  checkTouch();
  if (isAlarming) {
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1); // LED狂闪
    delay(100);
  }
}