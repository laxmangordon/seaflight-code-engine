#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_u-blox_GNSS

const unsigned int GPS_8MU_REFRESH_DELAY = 1000;
bool validGPS_NeoM8U = false;
SFE_UBLOX_GPS gpsNeoM8U;
long previousMillisGPSNeoM8Urefresh = 0;

struct gpsNeoM8UStruct {
  long latitude;
  long longitude;
  long altitude;
  byte SIV;
};

void setupGPS_NeoM8U() {

  if (gpsNeoM8U.begin() == false) //Connect to the Ublox module using Wire port
  {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring. Freezing."));
    validGPS_NeoM8U = false;
  }
  else {
    validGPS_NeoM8U = true;
    gpsNeoM8U.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    gpsNeoM8U.saveConfiguration(); //Save the current settings to flash and BBR

  }
}

void loopGPS_NeoM8U() {
  if (validGPS_NeoM8U) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisGPSNeoM8Urefresh > GPS_8MU_REFRESH_DELAY) {
      previousMillisGPSNeoM8Urefresh = millis();
      latitude = gpsNeoM8U.getLatitude();
      Serial.print(F("Lat: "));
      Serial.print(latitude);

      longitude = gpsNeoM8U.getLongitude();
      Serial.print(F(" Long: "));
      Serial.print(longitude);
      Serial.print(F(" (degrees * 10^-7)"));

      altitude = gpsNeoM8U.getAltitude();
      Serial.print(F(" Alt: "));
      Serial.print(altitude);
      Serial.print(F(" (mm)"));

      SIV = gpsNeoM8U.getSIV();
      Serial.print(F(" SIV: "));
      Serial.print(SIV);

    }
  }
}

void vt100DashDisplaygpsNeoM8U(bool refreshAll, int x, int y) {
  if (validGPS_NeoM8U) {
    
    if (rtc.setTime(0, gpsNeoM8U.getSecond(), gpsNeoM8U.getMinute(), gpsNeoM8U.getHour(), date, gpsNeoM8U.getMonth(), gpsNeoM8U.getYear(), gpsNeoM8U.getDay()) == false) {
      Serial.println("Something went wrong setting the time");
    }
  }
}
