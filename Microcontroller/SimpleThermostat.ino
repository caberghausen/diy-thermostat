#include <Servo.h>

// DELTA TIMING
unsigned long currentClockTime = 0;
unsigned long acRestTime = 2400e3;     // default: 40 min
const unsigned long acOnTime = 600e3;  // default: 10 min
unsigned long nextACToggleTime = 15000;
bool acIsOn = false;
bool shouldToggleAc = false;

// PINS
Servo acControlKnobServo;
const int SERVO_PIN = D1;
const int MANUAL_TOGGLE_SWITCH = D8;
const int DEFAULT_SCHEDULE_SWITCH = D7;
const int INDICATOR_LED = D6;
const int AC_OFF = 8;   // Servo angle
const int AC_ON = 175;  // Servo angle

// AC Control:
enum ACMode {
  onUntilStopped,
  scheduled
};
ACMode currentMode = onUntilStopped;

void turnACOff() {
  acControlKnobServo.write(AC_OFF);
  acIsOn = false;
  Serial.println("AC turned off");
}

void turnACOn() {
  acControlKnobServo.write(AC_ON);
  acIsOn = true;
  Serial.println("AC turned on");
}

void toggleACWithSchedule() {
  if (acIsOn) {
    turnACOff();
    nextACToggleTime = currentClockTime + acRestTime;
  } else {
    turnACOn();
    nextACToggleTime = currentClockTime + acOnTime;
  }
}

void turnACOnWithSchedule() {
  turnACOn();
  nextACToggleTime = currentClockTime + acOnTime;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  acControlKnobServo.attach(SERVO_PIN);
  pinMode(MANUAL_TOGGLE_SWITCH, INPUT);
  pinMode(DEFAULT_SCHEDULE_SWITCH, INPUT);
  pinMode(INDICATOR_LED, OUTPUT);
  turnACOff();

  setupWifi();
}

void loop() {
  currentClockTime = millis();

  if (buttonPushed(MANUAL_TOGGLE_SWITCH)) {
    currentMode = onUntilStopped;
    if (acIsOn) {
      turnACOff();
    } else {
      turnACOn();
    }
  }
  if (buttonPushed(DEFAULT_SCHEDULE_SWITCH)) {
    currentMode = scheduled;
    flashLED();
    if (acIsOn) {
      nextACToggleTime = currentClockTime + acOnTime;
    } else {
      nextACToggleTime = currentClockTime + acRestTime;
    }
  }
  if (currentClockTime > nextACToggleTime && currentMode == scheduled) {
    toggleACWithSchedule();
  }

  listenForNetworkUpdates(currentClockTime);
  updateNTPTimeIfNecessary(currentClockTime);
}
