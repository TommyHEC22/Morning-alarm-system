//this turns on the pump at the specified time and it turned off by the button.

#include <RTClib.h>

RTC_DS3231 rtc;

const int buttonPin = 2;
const int pumpPin = 9;

// Set your trigger time here
const int onHour = 7;
const int onMinute = 30;

bool pumpRunning = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT);
  rtc.begin();
}

void loop() {
  DateTime now = rtc.now();

  // Trigger pump at specified time
  if (now.hour() == onHour && now.minute() == onMinute && now.second() == 0) {
    pumpRunning = true;
  }

  // Button stops the pump
  if (digitalRead(buttonPin) == LOW) {
    pumpRunning = false;
  }

  // Control pump
  if (pumpRunning) {
    digitalWrite(pumpPin, HIGH);
  } else {
    digitalWrite(pumpPin, LOW);
  }
}