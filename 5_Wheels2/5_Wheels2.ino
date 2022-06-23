#include "Wheels.h"
Wheels w;
volatile char cmd;

#include <IRremote.h>
#define RECV_PIN 12

void setup() {
  w.attach(5, 6, 3, 10, 9, 11);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  Serial.begin(9600);
  w.setSpeed(200);
}

void loop() {
  if (IrReceiver.decode()) {
    switch(IrReceiver.decodedIRData.command) {
      case 24: w.forward(); Serial.println("fwd"); break;
      case 21: w.back(); Serial.println("back"); break;
      case 68: w.forwardLeft(); w.backRight(); break;
      case 67: w.forwardRight(); w.backLeft(); break;
      case 64: w.stop(); break;
    }
    IrReceiver.resume();
  }
}