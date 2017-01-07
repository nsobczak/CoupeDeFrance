/* |=====================|
 * | Gestion de la pince |
 * |=====================|
 */
/**
 *    \file pince.cpp
 *    \brief Gestion de la pince
 *    \author Olivier JOMBART
 *    \date Janvier 2016
*/
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

#include <Servo.h>


// I2C
#include <Wire.h>
#include "i2cCommunication.h"


//____________________________________________________________________________________________________
// I2C
#define _RECEIVEADRESS_ 10
#define _SENDADRESS_ 11


// Shield - For RAMPS 1.4
#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING



//_____________________________________________________________________________________________
//_____________________________________________________________________________________________


Servo myservo;        // create servo object to control a servo
int interrupt_pin;

/**
 * \fn void mouvement_pince(const int pin_in)
 * \brief pour un pin, la pince se ferme si elle reçoit l'état l'état low et s'ouvre si elle reçoit l'état high
 * \param const int pin_in
 */
 
void mouvement_pince(const int pin_in){   // Selon ce qu'elle reçoit (LOW/HIGH) elle effectuera le mouvement prévu
  if(digitalRead(pin_in)==LOW) {
    myservo.write(100);                   // fermée
  }
  if (digitalRead(pin_in)==HIGH) { 
    myservo.write(140);                   // ouverte
  }
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup() {
  myservo.attach(2);                      // attaches the servo on pin 2 to the servo object
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop() 
{
  mouvement_pince(interrupt_pin);
}
