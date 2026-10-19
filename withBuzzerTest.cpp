#include <RTClib.h>

RTC_DS3231 rtc;

const int buttonPin = 2;
const int pumpPin = 9;
const int buzzerPin = 8;

const int onHour = 19;
const int onMinute = 38;
const int runDuration = 300;

bool pumpRunning = false;
unsigned long startTime = 0;
unsigned long lastBeep = 0;
bool beepState = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  rtc.begin();
}

void loop() {
  DateTime now = rtc.now();

  // Trigger at specified time
  if (now.hour() == onHour && now.minute() == onMinute && now.second() == 0) {
    pumpRunning = true;
    startTime = millis();
  }

  // Button stops everything
  if (digitalRead(buttonPin) == LOW) {
    pumpRunning = false;
    noTone(buzzerPin);
  }

  // Stop after duration
  if (pumpRunning && millis() - startTime >= (unsigned long)runDuration * 1000) {
    pumpRunning = false;
    noTone(buzzerPin);
  }

  // Control pump and buzzer
  if (pumpRunning) {
    digitalWrite(pumpPin, HIGH);
    // Non-blocking beep every 1000ms
    if (millis() - lastBeep >= 1000) {
      lastBeep = millis();
      beepState = !beepState;
      if (beepState) {
        tone(buzzerPin, 1000, 500);
      }
    }
  } else {
    digitalWrite(pumpPin, LOW);
    noTone(buzzerPin);
  }
}