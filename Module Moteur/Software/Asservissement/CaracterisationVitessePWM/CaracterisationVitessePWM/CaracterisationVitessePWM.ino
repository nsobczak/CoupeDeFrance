/**
 *    \file caraterisationVitesseMoteur.ino
 *    \brief test des drivers, test des encodeurs
 *
 *    \author Arthur Duytschaever et Nicolas Sobczak
 *    \date Février 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include <Arduino.h>

/* ======================================================================================================
 *      Variables globales
 * ======================================================================================================
 */
#define encoder0PinA_L 13   //encodeur gauche A
#define encoder0PinB_L 12   //encodeur gauche B
#define encoder0PinA_R 11   //encodeur droit A
#define encoder0PinB_R 10   //encodeur droit B

#define MotorR 6 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define MotorL 3
#define IN1MotorR 52
#define IN2MotorR 53
#define IN1MotorL 22
#define IN2MotorL 23

#define diametreRoueCodeuse 0.05228 // 52,28mm
#define nombreTicksPour1TourDeRoue 2500

const float Pi = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*Pi;

unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse

// unsigned long pulseDuration;
// unsigned long totalDuration;
unsigned long testDuration;
unsigned long testStart;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        Serial.begin(115200);     // Initialisation port COM
        pinMode(MotorR,OUTPUT);
        pinMode(MotorL,OUTPUT);
        pinMode(IN1MotorR,OUTPUT);
        pinMode(IN2MotorR,OUTPUT);
        pinMode(IN1MotorL,OUTPUT);
        pinMode(IN2MotorL,OUTPUT);

        pinMode(encoder0PinA_L, INPUT);
        pinMode(encoder0PinB_L, INPUT);
        pinMode(encoder0PinA_R, INPUT);
        pinMode(encoder0PinB_R, INPUT);

        attachInterrupt(encoder0PinA_R, compteur_tick_R, CHANGE);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)

        /*
           === Remarques ===
           Pour changer le pwm: 0 -> 255
           100% = 0
           75% = 63
           50% = 127
           25% = 191
           0% = 255
         */
        //Moteur droit
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        digitalWrite(IN1MotorR, LOW);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);

        testDuration = millis();
        testStart = millis();
        
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop()
{
        
        //fréquence de mesure de 50Hz
        if (millis() - testDuration >= 20) {
                //Attente de 2" avant le début
                if ((millis() - testStart > 5000) && (millis() - testStart < 15000)) {
                          //Moteur droit
                        digitalWrite(IN1MotorR, HIGH);
                        digitalWrite(IN2MotorR, LOW);
                        digitalWrite(IN1MotorL, LOW);
                        digitalWrite(IN2MotorL, LOW);
                        analogWrite(MotorL,255);
                        analogWrite(MotorR,63);
                        //Serial.print("\t testDuration : \t " );
                        Serial.println(millis() - testDuration);
                        // //Serial.print("\t tick_codeuse_L : \t");
                        // Serial.println(tick_codeuse_L);
                        // // Serial.print("\t testDuration : \t");
                        // Serial.println(testDuration);

                        printDouble(calculVitesse(tick_codeuse_R, millis() - testDuration), 1000000);
                }
                else if (millis() > 15000) {
                        endTest();
                }
                testDuration = millis();
                tick_codeuse_R = 0;
        }
}


/**
 * \fn void compteur_tick_R()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_R()
{
        tick_codeuse_R++; // On incrémente le nombre de tick de la codeuse
}


/**
 * \fn void compteur_tick_L()
 * \brief Interruption sur tick de la codeuse left
 */
void compteur_tick_L()
{
        tick_codeuse_L++; // On incrémente le nombre de tick de la codeuse
}


/**
 * \fn calculVitesse
 * \param unsigned int tick_codeuse, unsigned long duration
 * \brief calcule la vitesse instantannée
 * \return unsigned long vitesse
 */
double calculVitesse(unsigned int tick_codeuse, unsigned long duration)
{
        double nombre_tours = (double) tick_codeuse / (double) nombreTicksPour1TourDeRoue;
        double tour_par_seconde = ((double)nombre_tours/(double)duration)*1000;

        return (double)perimetreRoueCodeuse * tour_par_seconde;
}


void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

        Serial.print (int(val)); //prints the int part
        Serial.print("."); // print the decimal point
        unsigned int frac;
        if(val >= 0)
                frac = (val - int(val)) * precision;
        else
                frac = (int(val)- val ) * precision;
        int frac1 = frac;
        while( frac1 /= 10 )
                precision /= 10;
        precision /= 10;
        while(  precision /= 10)
                Serial.print("0");

        Serial.println(frac,DEC);
}


void endTest()
{
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        digitalWrite(IN1MotorR, LOW);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
}


/**
 * \fn void testDriver()
 * \brief fonction qui teste qui les drivers fonctionnent correctement
 */
void testDriver()
{
        //Moteur gauche
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        Serial.print("On moteuL gauche \t 1");
        digitalWrite(IN1MotorR, LOW);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, HIGH);
        digitalWrite(IN2MotorL, LOW);
        Serial.print("\t 2");
        analogWrite(MotorL,100);
        analogWrite(MotorR,255);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //Moteur droit
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        Serial.print("On moteuL droit \t 1");
        digitalWrite(IN1MotorR, HIGH);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);
        Serial.print("\t 2");
        analogWrite(MotorL,255);
        analogWrite(MotorR,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //2 moteurs
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        Serial.print("On 2 moteurs \t 1");
        digitalWrite(IN1MotorR, HIGH);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, HIGH);
        digitalWrite(IN2MotorL, LOW);
        Serial.print("\t 2");
        analogWrite(MotorL,100);
        analogWrite(MotorR,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");


        //Moteur droit
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        Serial.print("On moteuL droit \t 1");
        digitalWrite(IN1MotorR, HIGH);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);
        Serial.print("\t 2");
        analogWrite(MotorL,255);
        analogWrite(MotorR,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //Moteur gauche
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        Serial.print("On moteuL gauche \t 1");
        digitalWrite(IN1MotorR, LOW);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, HIGH);
        digitalWrite(IN2MotorL, LOW);
        Serial.print("\t 2");
        analogWrite(MotorL,100);
        analogWrite(MotorR,255);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //2 moteurs
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
        Serial.print("On 2 moteurs \t 1");
        digitalWrite(IN1MotorR, HIGH);
        digitalWrite(IN2MotorR, LOW);
        digitalWrite(IN1MotorL, HIGH);
        digitalWrite(IN2MotorL, LOW);
        Serial.print("\t 2");
        analogWrite(MotorL,100);
        analogWrite(MotorR,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");
}
