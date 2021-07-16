#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

#include <Wire.h>
#include <SerLCD.h>                         //Click here to get the library: http://librarymanager/All#SparkFun_SerLCD
#include <SPI.h>
#include <SD.h>                             //
#include "Adafruit_VL6180X.h"               // library: https://github.com/adafruit/Adafruit_VL6180X
#include "SparkFun_RV1805.h"                // library: https://github.com/sparkfun/SparkFun_RV-1805_Arduino_Library/archive/master.zip  https://github.com/sparkfun/SparkFun_RV-1805_Arduino_Library
#include "SFE_HMC6343.h"                    // library: https://github.com/sparkfun/SparkFun_HMC6343_Arduino_Library

#include <BasicTerm.h>

#define ENGINE_DEBUG_PRINT 1

#define PRESSURE_RES        0
#define LCD_MUX             4
#define MUX_ADDR            0x70             //7-bit unshifted default I2C Address

#define FIRMWARE_VERSION    0.1

BasicTerm term(&Serial);

class DualPrint : public Print
{
  public:
    DualPrint() : use_Serial(false), use_Serial1(false) {}
    virtual size_t write(uint8_t c) {
      if (use_Serial) {
        Serial.write(c);
      }
      if (use_Serial1) {
        term.position(23, 2);
        term.print(c);
        //Serial1.write(c);

      }
      return 1;
    }
    bool use_Serial, use_Serial1;
} out;

enum GliderState {
  EMERGENCY,
  MISSION,
  STANDBY,
  BENCH,
  POWER_ON
};
static GliderState gliderState;

enum BenchState {
  INIT,
  DATA,
  TESTING,
  BURN_IN,
  WAITING,
  INIT_DEPLOY
};
enum BenchState benchState;

enum TestingState {
  ENGINE_TEST,
  SENSOR_TEST,
  POWER_TEST,
  COMMS_TEST
};
enum TestingState testingState;

enum EngineTestState {
  ENGINETEST_STANDBY,
  ENGINETEST_DIVING,
  ENGINETEST_CLIMBING,
  PUMP_TEST
};
enum EngineTestState engineTestState;

enum PumpTestState {
  PUMP_INIT,
  PUMP_STANDBY,
  PUMP_IN_ON,
  PUMP_OUT_ON,
  PUMP_OUT_HOLD,    //Simulated climbing time
  PUMP_IN_HOLD,     //Simulated Diving time
  PUMP_OFF,
  PUMP_HOLD_FOR
};
enum PumpTestState pumpTestState;
const char* pumpTestStateStr[] = {"INIT", "STANDBY", "PUMP_IN_ON", "PUMP_OUT_ON", "PUMP_OUT_HOLD", "PUMP_IN_HOLD", "PUMP_OFF", "PUMP_HOLD_FOR"};
bool displayVT100Dash = true;
String currentDate = "";
String currentTime = "";
long Pressure;
int Pval;
int Pval2;
long finalnumber;
int Pset = 75;   //Pressure Upper Limit
int Pmin = 10;   //Pressure Lower Limit

SerLCD lcd; // Initialize the library with default I2C address 0x72
RV1805 rtc;

bool usingVT100 = true;

bool SENSOR_DEBUG_PRINT = true;  //To see real time readouts on the Serial console set this to TRUE

void setup() {
  Serial.begin(115200);

  int serialDelay = 0;

  while (!Serial || serialDelay < 3) {
    ; // wait for serial port to connect. Needed for native USB port only
    delay(1000);
    serialDelay++;
  }
  out.use_Serial = false;
  Wire.begin();        //begin I2C Comm.
  setupLCDDisplay();
  //Serial.println("SeaFlight Glider v0.01 ENGINE TEST CODE");
  out.println("SeaFlight Glider v0.01 ENGINE TEST CODE");

  gliderState = BENCH;
  benchState = TESTING;
  testingState = ENGINE_TEST;
  engineTestState = PUMP_TEST;
  pumpTestState = PUMP_INIT;

  setupClockRV1805();
  setupPressure_M300();
  setupPumpTest();
  setupControlPump();
  setupTOF_VL6180();
  //setupInputs();
  //setupStorage();
  setupCompassHMC6343();
  vt100DashSetup();
  //changePumpTestState(PUMP_STANDBY);
}

void loop() {
  loopClockRV1805();
  loopPressure_M300();
  loopPumpTest();
  loopTOF_VL6180();
  loopInputs();
  //loopLCDDisplay();
  loopCompassHMC6343();
  vt100DashLoop();
  loopStorage();
}

//Enables a specific port number
void enableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Set the wanted bit to enable the port
  settings |= (1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
}

//Disables a specific port number
void disableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Clear the wanted bit to disable the port
  settings &= ~(1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
}
