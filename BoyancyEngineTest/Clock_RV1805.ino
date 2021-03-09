
#define CLOCK_RV1805_REFRESH_DELAY     20

RV1805 rtc;
bool validClockRV1805 = false;
String currentDate = "";
String currentTime = "";
long previousMillisClockRV1805 = 0;

void setupClockRV1805() {
  if (rtc.begin() == false) {
    validClockRV1805 = false;
    Serial.println("setupClockRV1805: Something went wrong, check wiring");
  }
  else {
    validClockRV1805 = true;
  }
}

void loopClockRV1805() {
  if (validClockRV1805) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisClockRV1805 > CLOCK_RV1805_REFRESH_DELAY) {
      previousMillisClockRV1805 = millis();
      if (rtc.updateTime() == false) //Updates the time variables from RTC
      {
        Serial.print("RTC failed to update");
      }
      else {
        currentDate = rtc.stringDateUSA(); //Get the current date in mm/dd/yyyy format (we're weird)
        //String currentDate = rtc.stringDate()); //Get the current date in dd/mm/yyyy format
        currentTime = rtc.stringTime(); //Get the time

      }
    }
  }
}
