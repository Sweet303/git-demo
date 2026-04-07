#include <WiFi.h>
#include <WebServer.h>

// WiFi配置
const char* ssid = "你的WiFi名称";
const char* password = "你的WiFi密码";

WebServer server(80);
const int touchPin = T0; // 触摸引脚T0(GPIO4)

// 处理根页面请求
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>触摸传感器仪表板</title>";
  html += "<style>body{font-family:Arial;text-align:center;margin-top:50px;}";
  html += "#value{font-size:80px;font-weight:bold;color:#3498db;}</style></head>";
  html += "<body><h1>触摸传感器数值</h1>";
  html += "<div id='value'>0</div>";
  // AJAX定时拉取数据
  html += "<script>setInterval(() => {";
  html += "fetch('/getTouchValue')";
  html += ".then(res => res.text())";
  html += ".then(data => document.getElementById('value').innerText = data);";
  html += "}, 100);</script>"; // 100ms刷新一次
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// 处理触摸数值请求
void handleTouchValue() {
  int touchVal = touchRead(touchPin); // 读取触摸模拟量
  server.send(200, "text/plain", String(touchVal));
}

void setup() {
  Serial.begin(115200);
  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi已连接，IP地址：" + WiFi.localIP().toString());

  // 注册路由
  server.on("/", handleRoot);
  server.on("/getTouchValue", handleTouchValue);
  server.begin();
}

void loop() {
  server.handleClient(); // 处理客户端请求
}
