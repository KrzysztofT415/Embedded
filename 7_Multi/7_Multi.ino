// Biblioteki: Wheels z zajęć, Dabble dla aplikacji bluetooth i Servo dla sensora odległości
#include "Wheels.h"
#include <Dabble.h>
#include <Servo.h>


// Piny sterowania autkiem (# oznacza numer kabelka licząc od zewnątrz płytki):
#define RF 4 // # 3
#define RB 7 // # 2
#define RS 6 // # 1
#define LF 8 // # 4
#define LB 12 // # 5
#define LS 10 // # 6

// Piny sensora odległości:
#define TRIG A4
#define ECHO A5

// Piny modułu bluetooth (w bibliotece Dabble):
// TXD 2
// HXD 3


Wheels w;
Servo serwo;

// funkcja zczytywania odległości z zajęć
int lookAndTellDistance() {
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  auto distance = pulseIn(ECHO, HIGH) / 58;

  return distance;
}

// funkcje poruszania się
void goForward() {
  int space = lookAndTellDistance();
  if (space < 30) {
    Serial.print(" Nie jadę, bo ściana : ");
    Serial.print(space);
    Serial.println(";");
    w.stop();
    return;
  }
  w.forwardLeft();
  w.forwardRight();
}

void goBackward() {
  w.backLeft();
  w.backRight();
}

void goRotLeft() {
  w.forwardRight();
  w.backLeft();
}

void goRotRight() {
  w.forwardLeft();
  w.backRight();
}

// funkcje dla dwóch trybów w aplikacji
void handleJoystickMode(int angle, int radius) {
  int speed = 100 + radius * 20; //  dobieram prędkość r:[0-7/+1] -> speed:[100-240/+20]
  w.setSpeed(speed);

  // jeżeli nie mam wychylonego joystick'a to się nie ruszam
  if (radius == 0) {
    w.setSpeed(0);
    w.stop();
  }

  // działanie specyficzne dla joystick'a (w tym przypadku dodatkowe 4 kierunki)
  else if (angle == 45) { // NE
    w.stop();
    w.forwardLeft();
  } else if (angle == 135) { // NW
    w.stop();
    w.forwardRight();
  } else if (angle == 225) { // SW
    w.stop();
    w.backLeft();
  } else if (angle == 315) { // SE
    w.stop();
    w.backRight();
  }

  // działanie wspólne z digital, czyli podstawowe kierunki
  else if (45 < angle && angle < 135) { // N
    goForward(); 
  } else if (135 < angle && angle < 225) { // W
    goRotLeft();
  } else if (225 < angle && angle < 315) { // S
    goBackward();
  } else { // E
    goRotRight();
  }
}

void handleDigitalMode() {
  // jak wcisnę X dodaję gazu
  if (GamePad.isCrossPressed()) {
    w.setSpeed(255);
  } else {
    w.setSpeed(180);
  }

  // działanie specyficzne dla digital'a
  if (GamePad.isUpPressed()) { // N
    goForward();
  } else if (GamePad.isLeftPressed()) { // W
    goRotLeft();
  } else if (GamePad.isDownPressed()) { // S
    goBackward();
  } else if (GamePad.isRightPressed()) { // E
    goRotRight();
  } else {
    w.stop();
  }
}


void setup() {
  w.attach(RF, RB, RS, LF, LB, LS);
  Dabble.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(250000);
  delay(1000);
}

void loop() {
  Dabble.processInput(); // Czytanie danych z aplikacji przez moduł, jest to ciągłe połączenie, tz. nawet gdy nie jest nic wciśnięte

  auto mode = GamePad.mode;
  /*
     Tryb działania aplikacji [0 - digital, 1 - joystick/accelerometer]
     Jest to zmienna prywatna w klasie GamePadModule, więc trzeba zmienić jej dostęp w pobranej bibliotece
     ~/.../Dabble/src/GamePadModule.h zmienna w linii 65:
          bool mode;   //mode=0 for gamepad , mode = 1 for joystick
     Trzeba ją przenieść do zmiennych publicznych, czyli np. linii 57.

     Nie jest to konieczne, ponieważ można dostosować implementację do wybranego trybu, lub do obu naraz,
     lecz może być przydatne w taki sposób, jak ja poniżej zastosowałem, czyli do osobnych trybów specyficzny kod
  */

  // W zależności od trybu działania, odpowiednio są definiowane ruchy autka
  if (mode == 0) {
    handleDigitalMode();

  } else if (mode == 1) {
    int radius = GamePad.getRadius(); // radius przyjmuje wartości całkowite z przedziału 0-7
    int angle = GamePad.getAngle(); // angle przyjmuje wartości 0-360 ze skokiem 15
    handleJoystickMode(angle, radius);
  }
}
