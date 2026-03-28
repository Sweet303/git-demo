#include <Arduino.h>

const int ledPin = 13;
unsigned long previousMillis = 0;
int stateIndex = 0; // 记录当前SOS状态的索引

// 定义SOS闪烁序列：0=灭，1=短亮，2=长亮；数字为持续时间(ms)
// 格式：状态, 持续时间, 状态, 持续时间, ...
int sssSequence[40] = {
    1, 500,  // 短亮
    0, 500,  // 灭
    1, 500,  // 短亮
    0, 500,  // 灭
    1, 500,  // 短亮（完成S）
    0, 500,  // 灭（字母间隔）
    2, 1500, // 长亮（O）
    0, 500,  // 灭
    2, 1500, // 长亮（O）
    0, 500,  // 灭
    2, 1500, // 长亮（O）
    0, 500,  // 灭（字母间隔）
    1, 500,  // 短亮（S）
    0, 500,  // 灭
    1, 500,  // 短亮
    0, 500,  // 灭
    1, 500,  // 短亮
    0, 3000  // SOS结束后长停顿
};

int sequenceLength = 36; // 序列总长度（状态+持续时间对的数量 × 2）

void setup() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW); // 初始状态为灭
}

void loop() {
    unsigned long currentMillis = millis();
    
    // 检查是否到达下一个状态的切换时间
    if (currentMillis - previousMillis >= sssSequence[stateIndex + 1]) {
        previousMillis = currentMillis;
        
        // 根据序列设置LED状态
        int state = sssSequence[stateIndex];
        if (state == 1 || state == 2) {
            digitalWrite(ledPin, HIGH);  // 亮
        } else {
            digitalWrite(ledPin, LOW);   // 灭
        }
        
        // 移动到下一个状态
        stateIndex += 2;
        
        // 如果到达序列末尾，重置索引
        if (stateIndex >= sequenceLength) {
            stateIndex = 0;
        }
    }
}