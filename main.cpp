#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Encoder myEncoder(3, 4);

const int buttonPin = 6;
const int pumpPin = 9;
const int buzzerPin = 8;
const int swPin = 5;

int onHour = 7;
int onMinute = 30;
const int runDuration = 300;

bool pumpRunning = false;
bool manualRun = false;
bool setting = false;
bool settingMinute = false;
unsigned long startTime = 0;
unsigned long lastBeep = 0;
bool beepState = false;

long lastEncPos = 0;

bool lastButtonState = HIGH;
bool lastSwState = HIGH;
unsigned long lastButtonTime = 0;
unsigned long lastSwTime = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  rtc.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Water Alarm");
  lcd.setCursor(0, 1);
  lcd.print("Ready");
  delay(2000);
}

void loop() {
  DateTime now = rtc.now();
  unsigned long currentTime = millis();

  // Read encoder
  long encPos = myEncoder.read();
  if (encPos != lastEncPos) {
    long diff = encPos - lastEncPos;
    lastEncPos = encPos;
    if (setting) {
      if (abs(diff) >= 1) {
        int steps = -(diff / 3);
        if (!settingMinute) {
          onHour = (onHour + steps + 24) % 24;
        } else {
          onMinute = (onMinute + steps + 60) % 60;
        }
      }
    }
  }

  // Encoder press with debounce
  bool swState = digitalRead(swPin);
  if (swState == LOW && lastSwState == HIGH && currentTime - lastSwTime > 200) {
    lastSwTime = currentTime;
    if (!setting) {
      setting = true;
      settingMinute = false;
    } else if (!settingMinute) {
      settingMinute = true;
    } else {
      setting = false;
      settingMinute = false;
    }
  }
  lastSwState = swState;

  // Button — hold to manual run, press to stop scheduled run
  bool buttonState = digitalRead(buttonPin);
  if (pumpRunning) {
    if (buttonState == LOW && lastButtonState == HIGH && currentTime - lastButtonTime > 200) {
      lastButtonTime = currentTime;
      pumpRunning = false;
      noTone(buzzerPin);
    }
  } else {
    manualRun = (buttonState == LOW);
  }
  lastButtonState = buttonState;

  // Trigger at specified time
  if (!pumpRunning && now.hour() == onHour && now.minute() == onMinute && now.second() == 0) {
    pumpRunning = true;
    startTime = millis();
  }

  // Stop after duration
  if (pumpRunning && millis() - startTime >= (unsigned long)runDuration * 1000) {
    pumpRunning = false;
    noTone(buzzerPin);
  }

  // Control pump and buzzer
  if (pumpRunning || manualRun) {
    digitalWrite(pumpPin, HIGH);
    if (pumpRunning) {
      if (millis() - lastBeep >= 200) {
        lastBeep = millis();
        beepState = !beepState;
        if (beepState) tone(buzzerPin, 300, 100);
      }
    }
  } else {
    digitalWrite(pumpPin, LOW);
    noTone(buzzerPin);
  }

  // Update LCD
  lcd.setCursor(0, 0);
  if (setting) {
    if (!settingMinute) {
      lcd.print("Set hour:  ");
      lcd.print(onHour < 10 ? "0" : "");
      lcd.print(onHour);
      lcd.print("  ");
    } else {
      lcd.print("Set min:   ");
      lcd.print(onMinute < 10 ? "0" : "");
      lcd.print(onMinute);
      lcd.print("  ");
    }
  } else if (pumpRunning) {
    lcd.print("PUMP ON!        ");
  } else if (manualRun) {
    lcd.print("MANUAL ON!      ");
  } else {
    lcd.print("Alarm: ");
    lcd.print(onHour < 10 ? "0" : "");
    lcd.print(onHour);
    lcd.print(":");
    lcd.print(onMinute < 10 ? "0" : "");
    lcd.print(onMinute);
    lcd.print("   ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(now.hour() < 10 ? "0" : "");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute() < 10 ? "0" : "");
  lcd.print(now.minute());
  lcd.print(":");
  lcd.print(now.second() < 10 ? "0" : "");
  lcd.print(now.second());
  lcd.print("  ");
}