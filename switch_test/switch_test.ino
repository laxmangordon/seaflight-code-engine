#define vbatt A6

void setup(){
  Serial.begin(115200);
}

void loop(){
  if (Serial.available()>0){
    test_mode();
  }
}

float get_voltage(){
  float measuredvbat = analogRead(vbatt);
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage  
  return (measuredvbat);
}

void test_mode(){
  char cmd = Serial.read();
  switch(cmd){
    case 'H':{
      Serial.println("Hello!");
      break;
    }
    case 'B':{
      Serial.println("Bye!");
      break;
    }
    case 'V':{
      float voltage = get_voltage();
      Serial.println(voltage);
      break;
    }
  }
}
