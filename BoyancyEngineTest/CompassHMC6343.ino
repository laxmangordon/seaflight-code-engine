
#define UTF8_HEX_DEGREE_SYMBOL                  "\xC2\xB0"

const unsigned int COMPASS_HMC6343_REFRESH_DELAY     200
SFE_HMC6343 compass; // Declare the sensor object
bool validCompassHMC6343 = false;
long previousMillisCompassHMC6343 = 0;

void setupCompassHMC6343() {
  if (!compass.init())
  {
    validCompassHMC6343 = false;
    Serial.println("Sensor Initialization Failed\n\r"); // Report failure, is the sensor wiring correct?
  }
  else {
    validCompassHMC6343 = true;
  }

}

void loopCompassHMC6343() {
  if (validCompassHMC6343) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisCompassHMC6343 > CLOCK_RV1805_REFRESH_DELAY) {
      previousMillisClockRV1805 = millis();
      // Read, calculate, and print the heading, pitch, and roll from the sensor
      compass.readHeading();
      compass.readMag();
      compass.readTilt();
      // Read, calculate, and print the acceleration on the x, y, and z axis of the sensor
      compass.readAccel();

    }
  }
}

void vt100DashDisplayCompassHMC6343(bool refreshAll, int x, int y) {
  if (validCompassHMC6343) {
    if (refreshAll) {

    }

    term.position(y, x);
    term.print(F("Compass"));
    int y_off = 1;
    term.position(y+y_off, x);
    term.print(F("Heading:"));
    term.print((float) compass.heading/10.0);
    Serial.print(UTF8_HEX_DEGREE_SYMBOL);
    
    y_off++;
    x = x+2;
    term.position(y+y_off, x);
    term.print(F("Magnetometer:"));
    term.position(y+y_off, x + 14);
    term.print(F("x="));
    term.print(compass.magX);
    term.position(y+y_off, x + 24);
    term.print(F("y="));
    term.print(compass.magY);
    term.position(y+y_off, x + 34);
    term.print(F("z="));
    term.print(compass.magZ);

    y_off++;
    term.position(y+y_off, x);
    term.print(F("Raw Tilt Data:"));
    term.position(y+y_off, x + 14);
    term.print(F("pitch="));
    term.print(compass.pitch);
    term.position(y+y_off, x + 27);
    term.print(F("roll="));
    term.print(compass.roll);
    term.position(y+y_off, x + 40);
    term.print(F("temp="));
    term.print(compass.temperature);

    y_off++;
    term.position(y+y_off, x);
    term.print(F("Acceleration:"));
    term.position(y+y_off, x + 14);
    term.print(F("x="));
    term.print((float) compass.accelX/1024.0);
    term.print(F("g")); 
    term.position(y+y_off, x + 24);
    term.print(F("y="));
    term.print((float) compass.accelY/1024.0);
    term.print(F("g")); 
    term.position(y+y_off, x + 34);
    term.print(F("z="));
    term.print((float) compass.accelZ/1024.0);
    term.print(F("g")); 
  }
}

//   compass.enterStandby();
//   compass.exitStandby();
//    compass.enterSleep();
//    compass.exitSleep();
//    compass.setOrientation(LEVEL);

//    byte opmode1 = compass.readOPMode1();
//    Serial.print("OP Mode 1: ");
//    Serial.println(opmode1,BIN);

//    int sn = compass.readEEPROM(SN_LSB);
//    delay(10);
//    sn |= (compass.readEEPROM(SN_MSB) << 8);
