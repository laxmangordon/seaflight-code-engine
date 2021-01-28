/*  SeaFlight Glider
    MSP300 Pressure Transducer


    More information:     https://bit.ly/39e2zJF
    Interfacing guide:    https://bit.ly/3c8NAT2

    Todo:
    - support for multiple pressure sensors using i2c mux
    - error handling - process the status bits returned
    - The MSP300 can return temperature - worth adding as another data point
*/

#define PRESSURE_M300_BLADDER 1
#define PRESSURE_M300_RESERVOIR 2
#define PRESSURE_SENSOR_REFRESH_RATE 500 //in milliseconds

long latestPressureM300_BLADDER = 0;
long latestPressureM300_RESERVOIR = 0;
unsigned long previousMillisPressureM300 = 0;
long intervalPressureM300 = 500;

struct PressureM300 {
  int pressureHigh;
  int pressureLow;
  int i2c;
  long latestPressure;
};

PressureM300 pressureM300_Bladder = { 255 , 0 , 2 , 0};
PressureM300 pressureM300_Reservoir = { 255 , 0 , 2, 0 };

void setupPressure_M300() {
  
}

void loopPressure_M300() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisPressureM300 > intervalPressureM300) {
    previousMillisPressureM300 = currentMillis;
    latestPressureM300_BLADDER = getPressure_M300();  //bladder for now until we get mux, it was all we had on initial testbed
    latestPressureM300_RESERVOIR = latestPressureM300_BLADDER;  //TEMP TEMP TEMP TEMP TEMP TEMP
#ifdef SENSOR_DEBUG_PRINT
    //Serial.print("pressure M300 = ");
    //Serial.println(latestPressureM300_BLADDER);
#endif
  }
}

long getPressure_M300()
{
  Wire.requestFrom(40, 2);

  while (Wire.available() == 0);  //consider collecting data inside this while loop  while(Wire.available()) {
  long Pval = Wire.read();
  long Pval2 = Wire.read();
  int pressureStatus = Pval & 0xC0;
  pressureStatus = pressureStatus >> 6;
  Pval = Pval & 0x3F;                   //mask out the status bits
  //int Pval3 = Wire.read();
  long Pvalshift = Pval << 8;
  long finalnum = Pvalshift | Pval2;
  long Pressure = (finalnum - 1000) / ((15000 - 1000) / 300);

  Wire.beginTransmission(40);
  Wire.write(0);
  Wire.endTransmission();

#ifdef SENSOR_DEBUG_PRINT
  Serial.print("Pressure M300=");
  Serial.print(Pressure);
  Serial.print("  pval:");
  Serial.print(Pval);
  Serial.print("  pval2:");
  Serial.print(Pval2);
  Serial.print("  status:");
  Serial.println(pressureStatus);
#endif

  return Pressure;
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
