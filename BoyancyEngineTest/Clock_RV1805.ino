
const unsigned int CLOCK_RV1805_REFRESH_DELAY     20
bool validClockRV1805 = false;
long previousMillisClockRV1805 = 0;

struct gliderTimeStruct {
  int hund = 42;
  int sec = 42;
  int minute = 42;
  int hour = 4;
  int date = 1;
  int month = 4;
  int year = 1974;
  int day = 26;

};

gliderTimeStruct currentTime;

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
/*
int setClockRV1805(gliderTimeStruct newTime) {
  currentTime.hund = newTime.hund;
  currentTime.sec = newTime.sec;
  currentTime.minute = newTime.minute;
  currentTime.hour = newTime.hour;
  currentTime.date = newTime.date;
  currentTime.month = newTime.month;
  currentTime.year = newTime.year;
  currentTime.day = newTime.day;
  
  return 1;
}
*/
