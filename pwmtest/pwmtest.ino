
/*
  Controlling a servo position using a potentiometer (variable resistor)
  by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

  modified on 8 Nov 2013
  by Scott Fitzgerald
  http://www.ardui8no.cc/en/Tutorial/Knob
*/
//#include "SAMD_AnalogCorrection.h"  // https://github.com/arduino/ArduinoCore-samd
#include <Servo.h>

#define PWM_NORMALSPEED 1200
#define PWM_ZERO        1000
#define PWM_MAX         2000
#define RAMP_DELAY      3000
#define PWM_RAMP_INCR   20

//#define ADC_INTERVAL          500
//#define ADC_READS_SHIFT      8
//#define ADC_READS_COUNT      (1 << ADC_READS_SHIFT)
//#define ADC_RESOLUTION_BITS   12
//#define ADC_RANGE             (1 << ADC_RESOLUTION_BITS)
//#define ADC_TOP_VALUE         (ADC_RANGE - 1)
//#define MAX_TOP_VALUE_READS   10
//
//#define ADC_CURRENT_PORT      A2  // current is from 0V-4V 0A-20A
//#define ADC_CURRENT_AMP_UP    20
//#define ADC_CURRENT_VOLT      4
//
//#define ADC_RPM_PORT          A3  // speed is from 0V-4V, 0RPM-5000RPM
//#define ADC_RPM_UP            5000
//#define ADC_RPM_VOLT          4

#define PWM_PIN               3
#define CW_PIN                7
#define CCW_PIN               8
#define SOLENOID_PIN          6

bool ramp = false;
bool rampUp = false;
bool rampDn = false;
bool readAnalog = true;
bool solenoidON = false;

bool pumpCW = false;
bool pumpCCW = false;
Servo myservo;  // create servo object to control a servo
unsigned long previousMillis = 0;
unsigned long previousAnalogMillis = 0;
int potpin = 0;  // analog pin used to connect the potentiometer
int valu = 100;  // variable to read the value from the analog pin

// analogReadCorrection(12, 2055);

void setup() {
  //analogReadResolution(ADC_RESOLUTION_BITS);
  //////analogReadCorrection(12, 2055); //corrects gain
  pinMode(CW_PIN, OUTPUT);
  digitalWrite(CW_PIN, HIGH);

  pinMode(CCW_PIN, OUTPUT);
  digitalWrite(CCW_PIN, HIGH);

  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, LOW);

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("pwm test code");
  Serial.println("1 1000 means set pwm to 1000");
  Serial.println("2 means ramp up from 1000 (which is 0 rpm)");
  Serial.println("3 means ramp dn to 1000 (which is 0 rpm)");
  Serial.println("4 means SOLENOID = ON");
  Serial.println("5 means SOLENOID = OFF");
  Serial.println("6 means CW PUMP TOGGLE");
  Serial.println("7 means CCW PUMP TOGGLE");
  Serial.println("8 means CW ENABLED");
  Serial.println("9 means CCW ENABLED");
  Serial.println("0 means OFF");

  previousAnalogMillis = millis();
  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
}
// 1000 = 20ms second freq, 50hz, 2ms pulse width
int pwmCounter = 0;
void loop() {
  if (Serial.available() > 0)  {
    int incomingData = Serial.parseInt();
    Serial.println(incomingData);
    //int incomingData = Serial.read(); // can be -1 if read error
    switch (incomingData) {
      case 1:
        Serial.print("myservo.write = ");
        Serial.println(valu);
        myservo.writeMicroseconds(valu);
        delay(15);
        break;
      case 2:     //ramp up
        if (rampUp) {
          rampUp = false;
        }
        else {
          rampUp = true;
          pwmCounter = PWM_ZERO + 200;
          Serial.println("starting rampUp...");
        }
        break;
      case 3:     //ramp dn
        if (rampDn) {
          rampDn = false;
        }
        else {
          rampDn = true;
          Serial.println("starting rampDn...");
        }
        break;
      case 4:
        digitalWrite(SOLENOID_PIN, HIGH);       // 0.83amps @ 12.2v = 10.13watts
        Serial.println("SOLENOID_PIN, HIGH");
        break;
      case 5:
        digitalWrite(SOLENOID_PIN, LOW);
        Serial.println("SOLENOID_PIN, LOW");
        break;
      case 6:
        if (pumpCW) {
          Serial.println("pumpCW OFF");
          pumpCW = false;
          controlPumpCW(false);
        }
        else {
          Serial.println("pumpCW ON");
          pumpCW = true;
          controlPumpCW(true);
        }
        break;
      case 7:
        if (pumpCCW) {
          Serial.println("pumpCCW OFF");
          pumpCCW = false;
          controlPumpCCW(false);
        }
        else {
          Serial.println("pumpCCW ON");
          pumpCCW = true;
          controlPumpCCW(true);
        }
        break;
      case 8:     //CCW
        Serial.println("CW_PIN, LOW");
        digitalWrite(CCW_PIN, HIGH);
        digitalWrite(CW_PIN, LOW);
        break;
      case 9:     //CW
        Serial.println("CCW_PIN, LOW");
        digitalWrite(CCW_PIN, LOW);
        digitalWrite(CW_PIN, HIGH);
        break;
      case 0:
        Serial.println("OFFFFFF");
        rampDn = false;
        rampUp = false;
        pwmCounter = PWM_ZERO;
        digitalWrite(CCW_PIN, HIGH);
        digitalWrite(CW_PIN, HIGH);
        myservo.writeMicroseconds(PWM_ZERO);
        break;
      default:
        valu = incomingData;
        Serial.print("incoming data = ");
        Serial.println(valu);
        break;
    }
  }
  if (rampUp == true) {
    if (pwmCounter >= PWM_MAX) {
      rampUp = false;
      Serial.print("rampUp ended at ");
      Serial.println(pwmCounter);
    } else if (millis() - previousMillis > RAMP_DELAY) {
      pwmCounter = pwmCounter + PWM_RAMP_INCR;
      myservo.write(pwmCounter);
      Serial.print("rampUp = ");
      Serial.println(pwmCounter);
      previousMillis = millis();
    }
  } else if (rampDn == true) {
    if (pwmCounter <= PWM_ZERO) {
      rampDn = false;
      Serial.print("rampDn ended at ");
      Serial.println(pwmCounter);
    } else if (millis() - previousMillis > RAMP_DELAY) {
      pwmCounter = pwmCounter - PWM_RAMP_INCR;
      myservo.write(pwmCounter);
      Serial.print("rampUp = ");
      Serial.println(pwmCounter);
      previousMillis = millis();
    }


  }
  /*
    if((millis() - previousAnalogMillis > ADC_INTERVAL) && readAnalog == true) {
      previousAnalogMillis = millis();
      //uint16_t pumpCurrentRaw = readAnalogPort(ADC_CURRENT_PORT);
      //uint16_t pumpRPMRaw = readAnalogPort(ADC_RPM_PORT);
      //uint16_t pumpCurrent = map(pumpCurrentRaw, 0, ADC_CURRENT_VOLT, 0, ADC_CURRENT_AMP_UP);
      //uint16_t pumpRPM = map(pumpRPMRaw, 0, 1023, 0, 4);
      int pumpCurrentRaw = analogRead(A2);
      int pumpRPMRaw = analogRead(A3);
      float pumpCurrentVoltageIn = (pumpCurrentRaw * 3.3) / ADC_TOP_VALUE;
      float pumpRPMVoltageIn = (pumpRPMRaw * 3.3) / ADC_TOP_VALUE;
      uint16_t pumpCurrent = map((pumpCurrentRaw * 3.3) / 65536, 0, ADC_CURRENT_VOLT, 0, ADC_CURRENT_AMP_UP);
      uint16_t pumpRPM = map(pumpRPMVoltageIn, 0, ADC_RPM_VOLT, 0, ADC_RPM_UP);
      Serial.print("rawCurr=");
      Serial.print(pumpCurrentRaw);
      Serial.print("  cur= ");
      Serial.print(pumpCurrent);

      Serial.print("  rawRPM=");
      Serial.print(pumpRPMRaw);
      Serial.print(" rpm=");
      Serial.println(pumpRPM);

    }
  */
}


void controlPumpCW(bool on) {     // going INTO pressure housing
  if (on) {
    myservo.write(PWM_NORMALSPEED);
    digitalWrite(CCW_PIN, HIGH);
    digitalWrite(CW_PIN, LOW);
  } else {
    digitalWrite(CCW_PIN, HIGH);
    digitalWrite(CW_PIN, HIGH);
    myservo.writeMicroseconds(PWM_ZERO);
  }
}

void controlPumpCCW(bool on) {      // going OUT pressure housing
  if (on) {
    myservo.write(PWM_NORMALSPEED);
    digitalWrite(CCW_PIN, LOW);
    digitalWrite(CW_PIN, HIGH);
  } else {
    digitalWrite(CCW_PIN, HIGH);
    digitalWrite(CW_PIN, HIGH);
    myservo.writeMicroseconds(PWM_ZERO);
  }
}
/*
  uint16_t readAnalogPort(int port)
  {
  uint32_t readAccumulator = 0;

  for (int i = 0; i < ADC_READS_COUNT; ++i)
    readAccumulator += analogRead(port);

  uint16_t readValue = analogRead(port); //readAccumulator >> ADC_READS_SHIFT;

  //Serial.print("ADC(GND) = ");
  //Serial.println(readValue);

  return readValue;
  }
*/
