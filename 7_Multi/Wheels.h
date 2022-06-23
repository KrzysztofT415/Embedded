/* 
 * prosta implementacja klasy obsĹ‚ugujÄ…cej 
 * silniki pojazdu za poĹ›rednictwem moduĹ‚u L298
 *
 * Sterowanie odbywa siÄ™ przez:
 * 1)  powiÄ…zanie odpowiednich pinĂłw I/O Arduino metodÄ… attach() 
 * 2)  ustalenie prÄ™dkoĹ›ci setSpeed*()
 * 3)  wywoĹ‚anie funkcji ruchu
 *
 * TODO:
 *  - zabezpieczenie przed ruchem bez attach()
 *  - ustawienie domyĹ›lnej prÄ™dkoĹ›ci != 0
 */


#include <Arduino.h>

#ifndef Wheels_h
#define Wheels_h

#include "Wheels.h"

class Wheels {
    public: 
        Wheels();
        /*
         *  pinForward - wejĹ›cie "naprzĂłd" L298
         *  pinBack    - wejĹ›cie "wstecz" L298
         *  pinSpeed   - wejĹ›cie "enable/PWM" L298
         */
        void attachRight(int pinForward, int pinBack, int pinSpeed);
        void attachLeft(int pinForward, int pinBack, int pinSpeed);
        void attach(int pinRightForward, int pinRightBack, int pinRightSpeed,
                    int pinLeftForward, int pinLeftBack, int pinLeftSpeed);
        /*
         *  funkcje ruchu
         */
        void forward();
        void forwardLeft();
        void forwardRight();
        void back();
        void backLeft();
        void backRight();
        void stop();
        void stopLeft();
        void stopRight();
        /*
         *  ustawienie prÄ™dkoĹ›ci obrotowej (przez PWM)
         *   - minimalna efektywna wartoĹ›Ä‡ 60
         *      moĹĽe zaleĹĽeÄ‡ od stanu naĹ‚adowania baterii
         */
        void setSpeed(uint8_t);
        void setSpeedRight(uint8_t);
        void setSpeedLeft(uint8_t);

    private: 
        int pinsRight[3];
        int pinsLeft[3];
};



#endif
