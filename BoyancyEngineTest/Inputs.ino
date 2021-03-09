#include <SparkFun_Qwiic_Button.h>
QwiicButton redStopButton;

bool stopButtonON = false;
void setupInputs() {
  if (redStopButton.begin() == false) {
    Serial.println("Red Button did not acknowledge! Freezing.");
    //while (1);
  }
}

void loopInputs() {
  if (redStopButton.isPressed() == true) {
    stopButtonON = true;
    Serial.println("redStopButton PRESSED");
    changePumpTestState(PUMP_OFF);
  } 
  else {  
    stopButtonON = false;
  }
}
