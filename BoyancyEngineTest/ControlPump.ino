#include <LiquidCrystal_I2C.h>
#define DIRECTION_IN 0           //pumps INTO reservoir
#define DIRECTION_OUT 1          //pumps OUT of reservoir
//VarSpeedServo myservo;
int testRunMotor = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setupControlPump() {
  //Solenoid Pin Setup

}

void initControlPump() {

  //myservo.attach(3);  // attaches the servo on pin 3 to the servo object
}


void pumpMotorON(int dir) {
  if (dir == DIRECTION_IN) {

  }
  else if (dir == DIRECTION_OUT) {

  }

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
