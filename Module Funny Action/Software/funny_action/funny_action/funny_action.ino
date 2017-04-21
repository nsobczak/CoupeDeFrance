/**
 *    \file funny_action.ino
 *    \brief code de la funny action
 *    \author Olivier Jombart
 *    \date Avril 2017
 */
//_______________________________________________________________________________________________________
#include <Servo.h>
#include <Arduino.h>

#define _DEBUG_ true
#define _TEST_ARRET_URGENCE_ET_CARTE_MERE_ false

unsigned long interval = 12000L; //Au bout de 90s, lancement funny action
const int PIN_D2 = 2; //Pin pour le bouton arret d'urgence, arret funny action si = HIGH
const int PIN_D3 = 3; //Pin pour la lancé de la funny action, ordre venant de la carte mère : HIGH
const int PIN_D5 = 5; // Pin test sortant un état HIGH
const int PIN_D6 = 6; // Pin test sortant un état HIGH

/**
 * \struct myMillis
 * \brief permet le reset de la millis
 */
struct myMillis
{
        unsigned long offset = 0;
        void set(unsigned long current) {
                offset = millis() - current;
        }
        void reset() {
                offset = millis();
        }
        unsigned long get() {
                return millis() - offset;
        }
};

bool flag = false;    //arret de la funny une fois lancée

Servo servo;
myMillis MM;


//_______________________________________________________________________________________________________
/**
 * \fn void servoMoteur()
 * \brief fonction d'interruption à la pin D2, reset la milis au lancement de l'ordre
 */
void servoMoteur()
{
        servo.write(60);
        if (_DEBUG_) Serial.println("Fusee lancee");
}


/**
 * \fn void interruption()
 * \brief fonction d'interruption à la pin D2, reset la milis au lancement de l'ordre
 */
void interruption()
{
        MM.reset();
        if (_DEBUG_) Serial.println("reset millis");
}


/**
 * \fn void setup()
 * \brief fonction d'initialisation
 */
void setup()
{
        pinMode(PIN_D2,INPUT);
        if (_TEST_ARRET_URGENCE_ET_CARTE_MERE_)
        {
                pinMode(PIN_D5,OUTPUT); //Pin test
                pinMode(PIN_D6,OUTPUT); //Pin test
        }

        attachInterrupt(digitalPinToInterrupt(PIN_D3),interruption,HIGH);
        Serial.begin(9600);
        servo.attach(10); // Servo moteur sur la PIN 10

        if (_TEST_ARRET_URGENCE_ET_CARTE_MERE_)
        {
                digitalWrite(PIN_D5,HIGH); //Pin test
                digitalWrite(PIN_D6,HIGH); //Pin test
        }

        servo.write(160);
}


/**
 * \fn void interruption()
 * \brief fonction de boucle principale
 */
void loop()
{
        if (digitalRead(PIN_D3)==HIGH && flag==false && digitalRead(PIN_D2)==LOW)
        {
                Serial.println(MM.get());
                if(MM.get() >= interval)
                {
                        servoMoteur();
                        flag = true;
                        delay(2000);
                        servo.write(160);
                }
        }
}
