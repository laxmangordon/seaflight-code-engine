#include <Adafruit_PWMServoDriver.h>
#include "MillisTimer.h"
#include <Wire.h>
#include <Servo.h>

//#define SENSOR_DEBUG_PRINT 
#define ENGINE_DEBUG_PRINT 1

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

long Pressure;
int Pval;
int Pval2;
long finalnumber;
int Pset = 75;   //Pressure Upper Limit
int Pmin = 10;   //Pressure Lower Limit

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Wire.begin();        //begin I2C Comm.
  Serial.println("SeaFlight Glider v0.01 ENGINE TEST CODE");

  //servo.attach(9);
  //servo.writeMicroseconds(1500); // send "stop" signal to ESC.
  
  gliderState = BENCH;
  benchState = TESTING;
  testingState = ENGINE_TEST;
  engineTestState = PUMP_TEST;
  pumpTestState = PUMP_INIT;

  setupPressure_M300();
  setupPumpTest();
  setupControlPump();
  
  
}

void loop() {
  loopPressure_M300();
  loopPumpTest();
  
}
