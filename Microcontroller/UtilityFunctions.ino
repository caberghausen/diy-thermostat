// Button press detection
unsigned long timeOfLastButtonPush = 0;
const uint delayForButtonRelease = 300;
bool buttonPushed(int buttonPin) {
  if (digitalRead(buttonPin) == HIGH && timeOfLastButtonPush + delayForButtonRelease < currentClockTime) {
    timeOfLastButtonPush = currentClockTime;
    return true;
  }
  return false;
}

// LED
void flashLED() {
  delay(50);
  digitalWrite(INDICATOR_LED, HIGH);
  delay(50);
  digitalWrite(INDICATOR_LED, LOW);
  delay(50);
  digitalWrite(INDICATOR_LED, HIGH);
  delay(50);
  digitalWrite(INDICATOR_LED, LOW);
}

bool isLEDOn = false;
void toggleLED() {
  digitalWrite(INDICATOR_LED, !isLEDOn);
  isLEDOn = !isLEDOn;
}

char* acIsOnToString() {
  if (acIsOn) {
    return "on";
  } else {
    return "off";
  }
}

char* currentModeToString() {
  if(currentMode == onUntilStopped) {
    return "onUntilStopped";
  } else {
    return "scheduled";
  }
}