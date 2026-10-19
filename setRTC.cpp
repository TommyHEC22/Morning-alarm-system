//set the time on the RTC

#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("Time set!");
}

void loop() {
}