#include "Wheels.h"
#include "config.h"
#include "lib.h"
Wheels w;
volatile char cmd;
int sp = 0;
int prevTime = 0;
int timeThreshold = 200;
bool stopped = true;

#include <IRremote.h>

void setup() {
  w.attach(RF, RB, RS, LF, LB, LS);
  IrReceiver.begin(PILOT, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
}

int lookAndTellDistance() {
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  auto distance = pulseIn(ECHO, HIGH) / 58;

  return distance;
}

void loop() {
  if (IrReceiver.decode()) {
    prevTime = millis();
    Serial.println(IrReceiver.decodedIRData.command);
    switch(IrReceiver.decodedIRData.command) {
      case 24:
        if (lookAndTellDistance() < 40) {
          w.stop(); stopped = true; break;
        } else {
          w.setSpeed(sp); w.forward(); stopped = false; break;
        }
      case 82: w.setSpeed(sp); w.back(); stopped = false; break;
      case 8: w.setSpeed(180); w.forwardLeft(); w.backRight(); stopped = false; break;
      case 90: w.setSpeed(180); w.forwardRight(); w.backLeft(); stopped = false; break;
      case 28: w.stop(); stopped = true; break;
      case 69: w.setSpeed(100); sp = 100; break;
      case 70: w.setSpeed(133); sp = 133; break;
      case 71: w.setSpeed(166); sp = 166; break;
      case 68: w.setSpeed(200); sp = 200; break;
      case 64: w.setSpeed(233); sp = 233; break;
      case 67: w.setSpeed(255); sp = 255; break;
      case 25: w.setSpeed(0); sp = 0; break;
      default: Serial.println(IrReceiver.decodedIRData.command);
    }
    IrReceiver.resume();
  }
  if (millis() - prevTime > timeThreshold && !stopped) w.stop();
}
