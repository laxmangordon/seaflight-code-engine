uint8_t reservoirDistanceTOF = 0;
bool VL6180_connected = false;
unsigned long previousMillisTOF_VL6180 = 0;
long intervalTOF_VL6180 = 50;

Adafruit_VL6180X vl = Adafruit_VL6180X();

void setupTOF_VL6180() {
  if (! vl.begin()) {
    Serial.println("VL6180 Failed to find sensor");
  } else {
    Serial.println("VL6180 Sensor Found");
    VL6180_connected = true;
  }
}

void loopTOF_VL6180() {
  if (VL6180_connected) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisTOF_VL6180 > intervalTOF_VL6180) {
      previousMillisTOF_VL6180 = currentMillis;
      float lux = vl.readLux(VL6180X_ALS_GAIN_5);
      if (SENSOR_DEBUG_PRINT) {
        Serial.print("VL6180X Lux: "); Serial.println(lux);
      }
      uint8_t range = vl.readRange();
      uint8_t status = vl.readRangeStatus();
      reservoirDistanceTOF = range;
      if (status == VL6180X_ERROR_NONE && SENSOR_DEBUG_PRINT) {
        Serial.print("VL6180X Range: "); Serial.println(range);
      }

      // Some error occurred, print it out!

      if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5) && SENSOR_DEBUG_PRINT) {
        Serial.println("VL6180X_ERROR_SYSERR_1");
      }
      else if (status == VL6180X_ERROR_ECEFAIL && SENSOR_DEBUG_PRINT) {
        Serial.println("VL6180X ECE failure");
      }
      else if (status == VL6180X_ERROR_NOCONVERGE && SENSOR_DEBUG_PRINT) {
        //Serial.println("VL6180X No convergence");
      }
      else if (status == VL6180X_ERROR_RANGEIGNORE && SENSOR_DEBUG_PRINT) {
        //Serial.println("VL6180X Ignoring range");
      }
      else if (status == VL6180X_ERROR_SNR && SENSOR_DEBUG_PRINT) {
        //Serial.println("VL6180X Signal/Noise error");
      }
      else if (status == VL6180X_ERROR_RAWUFLOW && SENSOR_DEBUG_PRINT) {
        //Serial.println("VL6180X Raw reading underflow");
      }
      else if (status == VL6180X_ERROR_RAWOFLOW && SENSOR_DEBUG_PRINT) {
        Serial.println("VL6180X Raw reading overflow");
      }
      else if (status == VL6180X_ERROR_RANGEUFLOW && SENSOR_DEBUG_PRINT) {
        Serial.println("VL6180X Range reading underflow");
      }
      else if (status == VL6180X_ERROR_RANGEOFLOW && SENSOR_DEBUG_PRINT) {
        Serial.println("VL6180X Range reading overflow");
      }
    }
  }
}
