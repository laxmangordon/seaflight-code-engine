#define LCD_REFRESH_DELAY     10

long previousMillisLCDRefresh = 0;
int displayCNT = 0;

void setupLCDDisplay() {

  lcd.begin(Wire); //Set up the LCD for I2C communication

  lcd.setBacklight(200, 255, 255); //Set backlight to bright white
  lcd.setContrast(3); //Set contrast. Lower to 0 for higher contrast.

  lcd.clear(); //Clear the display - this moves the cursor to home position as well
  lcd.print("SeaFlightGlider 0.01");
  //disableMuxPort(LCD_MUX);
}

void loopLCDDisplay() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisLCDRefresh > LCD_REFRESH_DELAY) {
    previousMillisLCDRefresh = millis();
    if (displayCNT++ > 10) {
      lcdWritePressureReservoir();
      lcdWritePressureBladder();
      lcdWritePumpTestState();
      lcdWriteTOF();
      lcdWriteInput();
      displayCNT = 0;
    }
    lcdWriteTimeDate();
  }
}

void lcdWriteTimeDate() {
  //lcd.setCursor(0, 0);
  //lcd.print("                 ");
  lcd.setCursor(0, 0);
  lcd.print(currentDate);
  lcd.print(currentTime);
}
void lcdWritePressureReservoir() {
  //setCursor(byte col, byte row);
  //enableMuxPort(LCD_MUX);
  lcd.setCursor(0, 4);
  lcd.print("      ");
  lcd.setCursor(0, 4);
  lcd.print(latestPressureM300_RESERVOIR);
  //disableMuxPort(LCD_MUX);
}

void lcdWritePressureBladder() {
  //setCursor(byte col, byte row);
  //enableMuxPort(LCD_MUX);
  lcd.setCursor(6, 3);
  lcd.print("   ");
  lcd.setCursor(6, 3);
  lcd.print(latestPressureM300_BLADDER);
  //disableMuxPort(LCD_MUX);
}

void lcdWriteInput() {
  if (stopButtonON) {
    lcd.setCursor(18, 4);
    lcd.print("S");
  }
  else {
    lcd.setCursor(18, 4);
    lcd.print(" ");
  }
}
void lcdWriteTOF() {
  lcd.setCursor(10, 3);
  lcd.print("         ");
  lcd.setCursor(10, 3);
  if (reservoirDistanceTOF == 255) {
    lcd.print("TOF=inf");
  } else {
    lcd.print("TOF=");
    lcd.print(reservoirDistanceTOF);
    lcd.print("cm");
  }
}

void lcdWritePumpTestState() {
  //enableMuxPort(LCD_MUX);
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("State=");
  //lcd.setCursor(7, 2);
  lcd.print(pumpTestStateStr[pumpTestState]);
  //lcd.print("     ");
  //disableMuxPort(LCD_MUX);
}
