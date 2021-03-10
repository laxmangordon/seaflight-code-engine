#include <Servo.h>
#include "SAMD_AnalogCorrection.h"

// 3/10/21  from Jeremy: CCW is OUT, CW is IN from perspective of reservoir

#define DIRECTION_IN  0           //pumps INTO reservoir    
#define DIRECTION_OUT 1          //pumps OUT of reservoir
#define PWM_PUMP_PIN  3

#define PWM_ZERO        1000
#define PWM_MAX         2000
#define RAMP_DELAY      3000
#define PWM_RAMP_INCR   20

#define ADC_INTERVAL          500
#define ADC_READS_SHIFT       8
#define ADC_READS_COUNT       (1 << ADC_READS_SHIFT)
#define ADC_RESOLUTION_BITS   12
#define ADC_RANGE             (1 << ADC_RESOLUTION_BITS)
#define ADC_TOP_VALUE         (ADC_RANGE - 1)
#define MAX_TOP_VALUE_READS   10

#define ADC_CURRENT_PORT      A2  // current is from 0V-4V 0A-20A
#define ADC_CURRENT_AMP_UP    20
#define ADC_CURRENT_VOLT      4

#define ADC_RPM_PORT          A3  // speed is from 0V-4V, 0RPM-5000RPM
#define ADC_RPM_UP            5000
#define ADC_RPM_VOLT          4

#define PWM_PIN               3
#define CW_PIN                7
#define CCW_PIN               8
#define SOLENOID_PIN          6

#define PWM_MAX       2000
#define PWM_MIN       1000
#define PWM_SLOW      1200
#define PWM_MED       1300
#define PWM_FAST      1500


int currentPWMSpeed = 1000;
Servo myservo; 

void setupControlPump() {
  
  analogReadResolution(ADC_RESOLUTION_BITS);
  analogReadCorrection(12, 2055); //corrects gain
  
  pinMode(CW_PIN, OUTPUT);
  digitalWrite(CW_PIN, HIGH);

  pinMode(CCW_PIN, OUTPUT);
  digitalWrite(CCW_PIN, HIGH);

  pinMode(SOLENOID_PIN, OUTPUT);
  digitalWrite(SOLENOID_PIN, LOW);

  currentPWMSpeed = 1000;

  myservo.attach(PWM_PUMP_PIN);
}

void initControlPump() {

  //myservo.attach(3);  // attaches the servo on pin 3 to the servo object
}


void pumpMotorSpin(int dir, int pwm) {
  if (pwm >= PWM_MIN && pwm <= PWM_MAX) {
    if (dir == DIRECTION_IN) {
      Serial.printf("pumpMotorSpin DIRECTION_IN");
      digitalWrite(CCW_PIN, LOW);
      digitalWrite(CW_PIN, HIGH);
      myservo.write(pwm);
    }
    else if (dir == DIRECTION_OUT) {
      Serial.printf("pumpMotorSpin DIRECTION_OUT");
      digitalWrite(CCW_PIN, HIGH);
      digitalWrite(CW_PIN, LOW);
      myservo.write(pwm);
    }
  }
}

void pump_off() {
  digitalWrite(CCW_PIN, LOW);
  digitalWrite(CW_PIN, LOW);
  myservo.write(PWM_MIN);
}

// turns on/off the enable
void pumpMotorEnable(bool on) {


}

/*
  void runMotor() {
  //Serial.println("Simulate Dive Cycle");
  //Pressure = getPressure();
  //Serial.println(Pressure);

  for (int j = 0; j < 500; j++)
  {
    while (Pressure < Pset)
    {
      //Serial.println("Surface-Close Solenoid");
      //Serial.print("Pressure | ");
      //Serial.println(Pressure);
      lcd.setCursor(6, 0);
      lcd.print("S");
      digitalWrite(Solenoid, LOW);
      //myservo.write(180, 250);
      lcd.setCursor(11, 1);
      lcd.print(Pressure);
      //Pressure = getPressure();
    }
    //Serial.println("Hold - Stop Motor"); // Stop Pump
    lcd.setCursor(6, 0);
    lcd.print("H");
    //myservo.write(95, 250);
    Serial.print("Pressure | ");
    Serial.println(Pressure);
    delay(5000);                    // 5 Second Hold Prior to reversing

    while (Pressure > Pmin)
    {
      //Serial.println("Dive-Close Solenoid");
      //Serial.print("Pressure | ");
      //Serial.println(Pressure);
      lcd.setCursor(6, 0);
      lcd.print("D");
      digitalWrite(Solenoid, HIGH);
      //myservo.write(25, 250);
      lcd.setCursor(11, 1);
      lcd.print(Pressure);
      //Pressure = getPressure();
    }
    //Serial.println("Hold - Stop Motor"); // Stop Pump
    lcd.setCursor(6, 0);
    lcd.print("H");
    //myservo.write(95, 250);
    digitalWrite(Solenoid, LOW);
    Serial.print("Pressure | ");
    Serial.println(Pressure);
    delay(5000);                    // 5 Second Hold Prior to reversing

    lcd.setCursor(6, 1);
    int count = j + 1;
    lcd.print(count);
    Serial.print("cycle ");
    Serial.println(count);
  }

  Serial.println("exited");

  }
*/
