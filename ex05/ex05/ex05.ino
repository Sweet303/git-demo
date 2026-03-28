#include <Arduino.h>

// 引脚定义（选择ESP32支持analogWrite的引脚，如GPIO2、GPIO13）
#define LED_PIN     2   
#define TOUCH_PIN   4   
// 触摸与档位配置
#define TOUCH_THRESHOLD 40  
#define DEBOUNCE_DELAY  20  
#define GEAR1_STEP 1     
#define GEAR2_STEP 3     
#define GEAR3_STEP 5     

// 全局变量
bool lastTouchState = false;    
unsigned long touchDebounceTime = 0; 
int currentGear = 1;            
int pwmValue = 0;               
int pwmStep = GEAR1_STEP;       
bool fadeUp = true;             

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("ESP32触摸调速呼吸灯（模拟PWM版）");
}

void loop() {
  // 触摸检测与档位切换
  unsigned long currentTime = millis();
  if (currentTime - touchDebounceTime >= DEBOUNCE_DELAY) {
    bool currentTouch = touchRead(TOUCH_PIN) < TOUCH_THRESHOLD;
    if (currentTouch && !lastTouchState) {
      currentGear = (currentGear % 3) + 1;
      switch (currentGear) {
        case 1: pwmStep = GEAR1_STEP; break;
        case 2: pwmStep = GEAR2_STEP; break;
        case 3: pwmStep = GEAR3_STEP; break;
      }
      Serial.print("当前档位：");
      Serial.println(currentGear);
    }
    lastTouchState = currentTouch;
    touchDebounceTime = currentTime;
  }

  // 呼吸灯逻辑（analogWrite实现）
  if (fadeUp) {
    pwmValue += pwmStep;
    pwmValue = min(pwmValue, 255);
    if (pwmValue == 255) fadeUp = false;
  } else {
    pwmValue -= pwmStep;
    pwmValue = max(pwmValue, 0);
    if (pwmValue == 0) fadeUp = true;
  }
  analogWrite(LED_PIN, pwmValue);
  delay(10);
}