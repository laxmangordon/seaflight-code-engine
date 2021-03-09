

#define STORAGE_REFRESH_DELAY     1000
const int chipSelect = SDCARD_SS_PIN;
bool validSDCard = false;
long previousMillisStorage = 0;

void setupStorage() {
  if (!SD.begin(chipSelect)) {
    validSDCard = false;
    Serial.println("SD Card failed, or not present");
    // don't do anything more:
    // while (1);
  }
  else {
    validSDCard = true;
  }
}

void loopStorage() {
  if (validSDCard) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisStorage > STORAGE_REFRESH_DELAY) {
      previousMillisStorage = millis();
      String dataString = "";
      dataString += String(currentDate);
      dataString += ",";
      dataString += String(currentTime);
      dataString += ",";
      dataString += String(pumpTestState);
      dataString += ",";
      dataString += String(latestPressureM300_RESERVOIR);
      dataString += ",";
      dataString += String(latestPressureM300_BLADDER);
      dataString += ",";
      dataString += String(reservoirDistanceTOF);

      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
        Serial.println(dataString);
      }
      // if the file isn't open, pop up an error:
      else {
        Serial.println("SDCARD: error opening datalog.txt");
      }
    }
  }
}
