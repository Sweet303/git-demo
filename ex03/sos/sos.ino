#include <Arduino.h>
const int ledPin = 13;
unsigned long previousMillis = 0;
int stateIndex = 0;  // 记录当前SOS状态的索引

// 定义SOS闪烁序列：0=灭，1=短亮，2=长亮；数字为持续时间(ms)
int sosSequence[40] = {1,500, 0,500, 1,500, 0,500, 1,500,  // 三次短闪
                       0,500, 2,1500,0,500, 2,1500,0,500,
                       2,1500,0,500, 1,500,0,500,1,500,0,500,1,500,  // 三次长闪+三次短闪
                       0,3000};  // SOS结束后长停顿
int sequenceLength = 38;  // 序列总长度

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= sosSequence[stateIndex+1]) {
    previousMillis = currentMillis;
    
    // 根据序列设置LED状态
    if (sosSequence[stateIndex] == 1 || sosSequence[stateIndex] == 2) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
    
    // 切换到下一个状态，循环执行SOS
    stateIndex = (stateIndex + 2) % sequenceLength;
  }
}