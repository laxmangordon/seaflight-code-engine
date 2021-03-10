/*  SeaFlight Glider
    Bouancy engine pump test state machine + control

    PUMP In means pumping INTO the reservoir                            i.e. DIVING
    PUMP OUT means pumping out of the reservoir (and into the bladder)  i.e. CLIMBING

    Todo:

*/
#define RUN_CYCLES  2                //number of complete cycles (i.e. dives) to go through
#define PRESSURE_UPPER_LIMIT 15   //Pressure Upper Limit
#define PRESSURE_LOWER_LIMIT -5   //Pressure Lower Limit
//#define SOLENOID_PIN LED_BUILTIN //9
#define ON  1
#define OFF 0

#define PUMP_INTERVAL_IN_POSTDELAY 5000  //amount of time between cycles.. 
#define PUMP_INTERVAL_OUT_POSTDELAY 5000  //amount of time between cycles.. 
#define CYCLE_START_WITH_PUMP_IN 1

#define KEY_START_PUMP_TEST_CYCLE 116   //letter 't' lowercase, kicks off pump cycle
#define KEY_PUMP_IN_TOGGLE    105     //letter 'i' lowercase
#define KEY_PUMP_OUT_TOGGLE   111     //letter 'o' lowercase
#define KEY_SOLENOID_TOGGLE   115     //letter 's' lowercase
#define KEY_NEXT_STATE        110     //letter 'n' lowercase



unsigned long previousMillisPUMP_IN = 0;
unsigned long previousMillisPUMP_OUT = 0;

bool actualPumpOnIn = OFF;
bool actualPumpOnOut = OFF;
bool actualSolenoidOn = OFF;

void setupPumpTest() {
   changePumpTestState(PUMP_INIT);
}

void loopPumpTest() {
  unsigned long currentMillis = millis();
  byte b;
  if (engineTestState == PUMP_TEST) {
    if (pumpTestState == PUMP_STANDBY) {
      loopPumpStandbyRespondToKeyPresses();
    }
    if (pumpTestState == PUMP_OFF) {  //pump_off is only to turn everything off, then jump to standby
      changePumpTestState(PUMP_STANDBY);
    }
    else if (pumpTestState == PUMP_IN_ON) {
      b = checkSerial();
      if (latestPressureM300_BLADDER >= PRESSURE_UPPER_LIMIT || b == KEY_NEXT_STATE) { //pressing 'n' will skip to next state
        changePumpTestState(PUMP_IN_HOLD);
      }
      else if (b == KEY_START_PUMP_TEST_CYCLE) { //press 't' again to stop test (toggle)
        changePumpTestState(PUMP_OFF);
      }
    }
    else if (pumpTestState == PUMP_OUT_ON) {
      b = checkSerial();
      if (latestPressureM300_BLADDER < PRESSURE_LOWER_LIMIT || b == KEY_NEXT_STATE) {
        changePumpTestState(PUMP_OUT_HOLD);
      }
      else if (b == KEY_START_PUMP_TEST_CYCLE) { //press 't' again to stop test (toggle)
        changePumpTestState(PUMP_OFF);
      }
    }
    else if (pumpTestState == PUMP_IN_HOLD) {
      b = checkSerial();
      if (currentMillis - previousMillisPUMP_IN > PUMP_INTERVAL_IN_POSTDELAY || b == KEY_NEXT_STATE) {
        changePumpTestState(PUMP_OUT_ON);
      }
      else if (b == KEY_START_PUMP_TEST_CYCLE) { //press 't' again to stop test (toggle)
        changePumpTestState(PUMP_OFF);
      }
    }
    else if (pumpTestState == PUMP_OUT_HOLD) {
      b = checkSerial();
      if (currentMillis - previousMillisPUMP_OUT > PUMP_INTERVAL_OUT_POSTDELAY || b == KEY_NEXT_STATE) {
        changePumpTestState(PUMP_IN_ON);
      }
      else if (b == KEY_START_PUMP_TEST_CYCLE) { //press 't' again to stop test (toggle)
        changePumpTestState(PUMP_OFF);
      }
    }
    else if (pumpTestState == PUMP_OFF) {
      digitalWrite(SOLENOID_PIN, LOW);
      //turn pumpoff
    }
    else if (pumpTestState == PUMP_INIT) {
      changePumpTestState(PUMP_STANDBY);
    }
  }

}

void changePumpTestState(enum PumpTestState newState) {
  if (newState == PUMP_INIT) {
    initControlPump();
  }
  else if (newState == PUMP_STANDBY) {
    Serial.println("PUMP_STANDBY accepting serial commands:");
    Serial.println("(t) to begin engine cycle test");
    Serial.println("(s) to toggle solenoid");
    Serial.println("(i) to toggle pump IN (into reservoir, out of bladder)");
    Serial.println("(o) to toggle pump OUT (out of reservoir, into bladder)");

  }
  else if (newState == PUMP_IN_ON) {
    controlSolenoid(ON);
    pumpIn(ON);
    Serial.println("State: PUMP_IN_ON press (n) to skip PUMP_IN_HOLD");

  }
  else if (newState == PUMP_OUT_ON) {
    controlSolenoid(OFF);
    pumpOut(ON);
    Serial.println("State: PUMP_OUT_ON press (n) to skip PUMP_OUT_HOLD");
  }
  else if (newState == PUMP_IN_HOLD) {
    controlSolenoid(OFF);
    pumpIn(OFF);
    previousMillisPUMP_IN = millis();       //mark time for timer for how long to be in 'hold' state
    Serial.print("State: PUMP_IN_HOLD seconds:");  Serial.println(PUMP_INTERVAL_IN_POSTDELAY);
  }
  else if (newState == PUMP_OUT_HOLD) {
    controlSolenoid(OFF);
    pumpOut(OFF);
    previousMillisPUMP_OUT = millis();      //mark time for timer for how long to be in 'hold' state
    Serial.print("State: PUMP_IN_HOLD seconds:");  Serial.println(PUMP_INTERVAL_IN_POSTDELAY);
  }
  else if (newState == PUMP_OFF) {
    controlSolenoid(OFF);
    pumpOut(OFF);
  }
  pumpTestState = newState;

#ifdef ENGINE_DEBUG_PRINT
  Serial.print("==============NEW PumpTestState = ");
  Serial.println(pumpTestStateStr[pumpTestState]);
#endif
}

void controlSolenoid(int turnON) {
  if (turnON) {
    digitalWrite(SOLENOID_PIN, LOW);
    actualSolenoidOn = ON;
#ifdef ENGINE_DEBUG_PRINT
    Serial.println("----Solenoid ON----");
#endif
  }
  else {
    digitalWrite(SOLENOID_PIN, HIGH);
    actualSolenoidOn = OFF;
#ifdef ENGINE_DEBUG_PRINT
    Serial.println("----Solenoid OFF----");
#endif
  }
}

void pumpIn(int turnON) {
  if (!actualPumpOnOut) {
    if (turnON) {
      //turn pump on
      pumpMotorSpin(DIRECTION_IN, PWM_SLOW);
      actualPumpOnIn = ON;
#ifdef ENGINE_DEBUG_PRINT
      Serial.println("----PumpIN ON----");
#endif
    }
    else {
      //turn pump off
      actualPumpOnIn = OFF;
#ifdef ENGINE_DEBUG_PRINT
      Serial.println("----PumpIN OFF----");
#endif
    }
  }
  else {
#ifdef ENGINE_DEBUG_PRINT
    Serial.println("----ERROR: pump is already ON OUT");
#endif
  }
}
void pumpOut(int turnON) {
  if (!actualPumpOnIn) {
    if (turnON) {
      //turn pump on
      pumpMotorSpin(DIRECTION_OUT, PWM_SLOW);
      actualPumpOnOut = ON;
#ifdef ENGINE_DEBUG_PRINT
      Serial.println("----PumpOUT ON----");
#endif
    }
    else {
      //turn pump off
      actualPumpOnOut = OFF;
#ifdef ENGINE_DEBUG_PRINT
      Serial.println("----PumpOUT OFF----");
#endif
    }
  }
  else {
#ifdef ENGINE_DEBUG_PRINT
    Serial.println("----ERROR: pump is already ON IN");
#endif
  }
}

void pumpTestProcessInput(byte incomingByte) {
  if (incomingByte == 49) {
    if (pumpTestState == PUMP_STANDBY) {
      changePumpTestState(PUMP_IN_ON);
    }
    else if (pumpTestState == PUMP_STANDBY) {
      //changePumpTestState(PUMP_IN_OFF);
    }

  }
}

void loopPumpStandbyRespondToKeyPresses() {
  byte b = checkSerial();
  if (b == KEY_START_PUMP_TEST_CYCLE) {
    changePumpTestState(PUMP_IN_ON);
  }
  else if (b == KEY_PUMP_IN_TOGGLE) {
    if (actualPumpOnIn) {
      pumpIn(OFF);
    } else {
      pumpIn(ON);
    }
#ifdef ENGINE_DEBUG_PRINT
    Serial.print("KEY_PUMP_IN_TOGGLE pumpval= ");
    Serial.println(actualPumpOnIn);
#endif
  }
  else if (b == KEY_PUMP_OUT_TOGGLE) {
    if (actualPumpOnOut) {
      pumpOut(OFF);
    } else {
      pumpOut(ON);
    }
#ifdef ENGINE_DEBUG_PRINT
    Serial.print("KEY_PUMP_OUT_TOGGLE pumpval= ");
    Serial.println(actualPumpOnOut);
#endif
  }
  else if (b == KEY_SOLENOID_TOGGLE) {
    if (actualSolenoidOn) {
      controlSolenoid(OFF);
    }
    else {
      controlSolenoid(ON);
    }
#ifdef ENGINE_DEBUG_PRINT
    Serial.print("KEY_SOLENOID_TOGGLE pumpval= ");
    Serial.println(actualSolenoidOn);
#endif
  }
}

byte checkSerial() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    byte incomingByte = Serial.read();

    // say what you got:
    //Serial.print("I received: ");
    //Serial.println(incomingByte, DEC);
    //pumpTestProcessInput(incomingByte);
    return incomingByte;
  }
  return -1;
}
