/*  SeaFlight Glider
    MSP300 Pressure Transducer


    More information:     https://bit.ly/39e2zJF
    Interfacing guide:    https://bit.ly/3c8NAT2

    Todo:
    - support for multiple pressure sensors using i2c mux
    - error handling - process the status bits returned
    - The MSP300 can return temperature - worth adding as another data point
*/

#define PRESSURE_M300_BLADDER 0
#define PRESSURE_M300_RESERVOIR 1
#define PRESSURE_SENSOR_REFRESH_RATE 500 //in milliseconds

long latestPressureM300_BLADDER = 0;
long latestPressureM300_RESERVOIR = 0;
unsigned long previousMillisPressureM300 = 0;
unsigned long intervalPressureM300 = 500;
bool validPressureSensors = false;

struct PressureM300 {
  int pressureHigh;
  int pressureLow;
  int muxPort;
  long latestPressure;
};

PressureM300 pressureM300_Bladder = { 255 , 0 , 0 , 0};
PressureM300 pressureM300_Reservoir = { 255 , 0 , 1, 0 };

void setupPressure_M300() {
  validPressureSensors = true;
}

void loopPressure_M300() {
  if (validPressureSensors) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisPressureM300 > intervalPressureM300) {
      previousMillisPressureM300 = currentMillis;
      latestPressureM300_BLADDER = getPressure_M300(PRESSURE_M300_BLADDER);  //bladder for now until we get mux, it was all we had on initial testbed
      latestPressureM300_RESERVOIR = getPressure_M300(PRESSURE_M300_RESERVOIR);


      if (SENSOR_DEBUG_PRINT) {
        //Serial.print("pressure M300 = ");
        //Serial.println(latestPressureM300_BLADDER);
      }

    }
  }
}

long getPressure_M300(int mux)
{
  enableMuxPort(mux);
  Wire.requestFrom(40, 2);  //40 is address of M300, 112 is address of
  while (Wire.available() == 0);  //consider collecting data inside this while loop  while(Wire.available()) {
  long Pval = Wire.read();              //MSB
  long Pval2 = Wire.read();             //LSB
  int pressureStatus = Pval & 0xC0;     // 0xC0 = 1100 0000
  pressureStatus = pressureStatus >> 6;
  Pval = Pval & 0x3F;                   //mask out the status bits 0x3F = 0011 1111
  //int Pval3 = Wire.read();
  long Pvalshift = Pval << 8;
  long finalnum = Pvalshift | Pval2;

  //  Pressure Sensor info:
  //  Manufacturer: TE  Part number: M30J1-000105-300PG
  //  i2c, 17-4PH Stainless Steel, Sleep Mode, i2c add=0X28H, 1/4-18 NPT, psi std=300, bar std=020B, pressure ref = guage
  //  Pmax = 300
  //  Pmin = 0
  // output(decimal counts) = (15000-1000)/(Pmax-Pmin) x (Papplied - Pmin) + 1000

  long Pressure = (finalnum - 1000) / ((15000 - 1000) / 300);

  Wire.beginTransmission(40);
  Wire.write(0);
  Wire.endTransmission();
  disableMuxPort(mux);
  if (SENSOR_DEBUG_PRINT) {
    Serial.print("Pressure M300=");
    Serial.print(Pressure);
    Serial.print(" Fin=");
    Serial.print(finalnum);
    Serial.print(" MUX=");
    Serial.print(mux);
    Serial.print(" pval:");
    Serial.print(Pval);
    Serial.print("  pval2:");
    Serial.print(Pval2);
    Serial.print("  status:");
    Serial.println(pressureStatus);
  }

  return Pressure;
}

void vt100DashDisplayPressure(bool refreshAll, int x, int y) {
  if (refreshAll) {
    //term.position(0, 0);
    //term.print(F("SeaFlight Glider "));
    //term.print(FIRMWARE_VERSION);
  }
  term.show_cursor(false);
  //if (validPressureSensors) {
  term.position(y, x + 4);
  term.print(F("Pressure"));
  term.position(y + 1, x);
  term.print(F("Reservoir:"));
  term.position(y + 1, x + 11);
  term.print(latestPressureM300_RESERVOIR);
  term.print(F(" Psi"));
  term.position(y + 2, x);
  term.print(F("Bladder: "));
  term.position(y+2, x + 11);
  term.print(latestPressureM300_BLADDER);
  term.print(F(" Psi"));
  //}

}

// ORIGINAL = FLAKY
//int getPressure()
//{
//  //start the communication with IC with the address xx
//  Wire.beginTransmission(40);
//  //send a bit and ask for register zero
//  Wire.write(0);
//  //end transmission
//  Wire.endTransmission();
//  //request 2 byte from address xx
//  Wire.requestFrom(40, 2);
//  //wait for response
//  while (Wire.available() == 0);
//  int Pval = Wire.read();
//  int Pvalshift = Pval << 8;
//  int Pval2 = Wire.read();
//  int Pval3 = Wire.read();
//  long finalnum = Pvalshift | Pval2;
//  long Pressure = (finalnum - 1000) / ((15000 - 1000) / 300);
//  //Serial.print("getPressure | ");
//  //Serial.println(Pressure);
//  return Pressure;
//}
