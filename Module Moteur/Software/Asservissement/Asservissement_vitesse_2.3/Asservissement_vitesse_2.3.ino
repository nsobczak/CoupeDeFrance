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
#include "botDirection.h"


/* ======================================================================================================
 *      Variables globales
 * ======================================================================================================
 */
#define _DEBUG_ true

#define _ENDODER_0_PinA_L_ 13   //encodeur gauche A
#define _ENDODER_0_PinB_L_ 12   //encodeur gauche B
#define _ENDODER_0_PinA_R_ 11   //encodeur droit A
#define _ENDODER_0_PinB_R_ 10   //encodeur droit B

#define _MOTOR_R_ 6 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define _MOTOR_L_ 3
#define _IN1_MOTOR_R_ 52
#define _IN2_MOTOR_R_ 53
#define _IN1_MOTOR_L_ 22
#define _IN2_MOTOR_L_ 23

#define diametreRoueCodeuse 0.05228 // 52,28mm
#define nombreTicksPour1TourDeRoue 2500

const float Pi = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*Pi;

unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse

unsigned long testDuration;
unsigned long testStart;

float consigneVitesseMoteur;
/*float sommeErreurGauche = 0;
float sommeErreurDroite = 0;*/
float erreurPrecedenteGauche = 0;
float erreurPrecedenteDroite = 0;
/*float consignePWMR;
float consignePWML;
float kpGauche = 1;
float kiGauche = 0;
float kdGauche = 0;
float kpDroit = 92.65;//2500;//4.5;
float kiDroit = 0.42;//7.5;//0.4;
float kdDroit = 0;//5;*/
float r0 = 92.422;//19.786;
float r1 = -30;//-2.817; //-59.541;//-2.817;
int cmdPrecedenteDroite = 0;



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
        pinMode(_MOTOR_R_,OUTPUT);
        pinMode(_MOTOR_L_,OUTPUT);
        pinMode(_IN1_MOTOR_R_,OUTPUT);
        pinMode(_IN2_MOTOR_R_,OUTPUT);
        pinMode(_IN1_MOTOR_L_,OUTPUT);
        pinMode(_IN2_MOTOR_L_,OUTPUT);

        pinMode(_ENDODER_0_PinA_L_, INPUT);
        pinMode(_ENDODER_0_PinB_L_, INPUT);
        pinMode(_ENDODER_0_PinA_R_, INPUT);
        pinMode(_ENDODER_0_PinB_R_, INPUT);

        attachInterrupt(_ENDODER_0_PinA_R_, compteur_tick_R, CHANGE);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        attachInterrupt(_ENDODER_0_PinA_L_, compteur_tick_L, CHANGE);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)

        /*
           === Remarques ===
           Pour changer le pwm: 0 -> 255
           100% = 0
           75% = 63
           50% = 127
           25% = 191
           0% = 255
         */
        analogWrite(_MOTOR_L_, 255);
        analogWrite(_MOTOR_R_, 255);
        robotStop();

        consigneVitesseMoteur = 0.5;
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
                //Attente de 5" avant le début
                if ((millis() - testStart > 5000) && (millis() - testStart < 25000)) {

                        if ((millis() - testStart >= 5000) && (millis() - testStart < 10000)) consigneVitesseMoteur = 0.5;
                        else if ((millis() - testStart >= 10000) && (millis() - testStart < 12500)) consigneVitesseMoteur = 1.5;
                        else if ((millis() - testStart >= 12500) && (millis() - testStart < 15000)) consigneVitesseMoteur = 1.0;
                        else if ((millis() - testStart >= 15000) && (millis() - testStart < 17500)) consigneVitesseMoteur = 0.8;
                        else if ((millis() - testStart >= 17500) && (millis() - testStart < 22500)) consigneVitesseMoteur = 0.1;
                        else if ((millis() - testStart >= 22500) && (millis() - testStart < 25000)) consigneVitesseMoteur = 0.3;
                        //consignePWML = calculConsignePWMMotorL(consigneVitesseMoteur);
                        //consignePWMR = calculConsignePWMMotorR(consigneVitesseMoteur);

                        double vitesseReelleGauche = calculVitesse(tick_codeuse_L, millis() - testDuration);
                        double vitesseReelleDroite = calculVitesse(tick_codeuse_R, millis() - testDuration);

                        //calcul erreur pour la correction proportionnelle*/
                        float erreurGauche = consigneVitesseMoteur - (float)vitesseReelleGauche;
                        float erreurDroite = consigneVitesseMoteur - (float)vitesseReelleDroite;
                        //calcul erreur pour la correction intégrale
                        //sommeErreurGauche += erreurGauche;
                        //sommeErreurDroite += erreurDroite;
                        //calcul erreur pour la correction dérivation
                        /*float deltaErreurGauche = erreurGauche - erreurPrecedenteGauche;
                        float deltaErreurDroite = erreurDroite - erreurPrecedenteDroite;*/
                        erreurPrecedenteGauche = erreurGauche;
                        erreurPrecedenteDroite = erreurDroite;

                        //int cmdMoteurDroit = kpDroit * erreurDroite + kiDroit * sommeErreurDroite + kdDroit * deltaErreurDroite + consignePWMR;
                        //int cmdMoteurDroit = kpDroit * erreurDroite + kiDroit * sommeErreurDroite  + kdDroit * deltaErreurDroite; 
                        //int cmdMoteurGauche = kpGauche * erreurGauche + kiGauche * sommeErreurGauche + kdGauche * deltaErreurGauche + consignePWML;
                        int cmdMoteurDroite = r0 * erreurDroite + r1 * erreurPrecedenteDroite + cmdPrecedenteDroite ; 
                        cmdPrecedenteDroite = cmdMoteurDroite;
                        if(cmdMoteurDroite < 0) cmdMoteurDroite = 0;
                        else if (cmdMoteurDroite > 255) cmdMoteurDroite = 255;


                        // digitalWrite(_IN1_MOTOR_R_, LOW);
                        // digitalWrite(_IN2_MOTOR_R_, HIGH);
                        // digitalWrite(_IN1_MOTOR_L_, LOW);
                        // digitalWrite(_IN2_MOTOR_L_, HIGH);
                        // analogWrite(_MOTOR_L_, cmdMoteurGauche);
                        // analogWrite(_MOTOR_R_, cmdMoteurDroit);
                        // robotGoStraightAhead(cmdMoteurGauche, cmdMoteurDroit);
                        robotTurnAroundFrontRight(255-cmdMoteurDroite);
                        if (_DEBUG_) {
                                // Serial.print("\t testDuration : \t " );
                                // Serial.println(millis() - testDuration);
                                // Serial.print("\t tick_codeuse_L : \t");
                                // Serial.println(tick_codeuse_L);
                                // Serial.print("\t testDuration : \t");
                                Serial.println(testDuration);

                                // TODO: ne pas oublier de change l'encoder L ou R
                                // Serial.print("\t consignePWM : \t " );
                                // Serial.println(cmdMoteurDroit);
                                // Serial.print("\t consigneVitesseMoteur : \t");
                                Serial.println(consigneVitesseMoteur);
                                // Serial.print("\t calculVitesse : \t " );
                                printDouble(calculVitesse(tick_codeuse_R, millis() - testDuration), 1000000);
                        }
                }
                else if (millis() > 20000) {
                        robotStop();
                }
                testDuration = millis();
                // Decommenter pour le moteur droit
                tick_codeuse_R = 0;
                tick_codeuse_L = 0;
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


/**
 * \fn calculConsignePWMMotorR(double y)
 * \brief fonction contenant l'équation permettant de convertir la consigne vitesse en consigne PWM
 * \param double y, y > 1,5056
 * \return x
 */
float calculConsignePWMMotorR(float consigne_vitesse_moteur)
{
        //if(consigne_vitesse_moteur < 0) consigne_vitesse_moteur = 0.0;
        //else if(consigne_vitesse_moteur > 1.421) consigne_vitesse_moteur = 1.421;
        //y = -73.928 * x² - 35.964 * x + 204.14; = -60,778x2 - 62,861x + 212,94
        return -73.928 * consigne_vitesse_moteur*consigne_vitesse_moteur - 35.964 * consigne_vitesse_moteur + 204.14; // sqrt(1.0/(-0.00002) * (consigne_vitesse_moteur - 1.5056) ) - 65.0;
}
float calculConsignePWMMotorL(float consigne_vitesse_moteur)
{
        //if(consigne_vitesse_moteur < 0) consigne_vitesse_moteur = 0.0;
        //else if(consigne_vitesse_moteur > 1.421) consigne_vitesse_moteur = 1.421;
        //y = -73.928 * x² - 35.964 * x + 204.14; = -60,778x2 - 62,861x + 212,94
        return -60.778 * consigne_vitesse_moteur*consigne_vitesse_moteur - 62.861 * consigne_vitesse_moteur + 212.94; // sqrt(1.0/(-0.00002) * (consigne_vitesse_moteur - 1.5056) ) - 65.0;
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


//______________________________________________________________________________
/**
 * \fn void testDriver()
 * \brief fonction qui teste qui les drivers fonctionnent correctement
 */
void testDriver()
{
        //Moteur gauche
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,255);
        Serial.print("On moteuL gauche \t 1");
        digitalWrite(_IN1_MOTOR_R_, LOW);
        digitalWrite(_IN2_MOTOR_R_, LOW);
        digitalWrite(_IN1_MOTOR_L_, HIGH);
        digitalWrite(_IN2_MOTOR_L_, LOW);
        Serial.print("\t 2");
        analogWrite(_MOTOR_L_,100);
        analogWrite(_MOTOR_R_,255);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //Moteur droit
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,255);
        Serial.print("On moteuL droit \t 1");
        digitalWrite(_IN1_MOTOR_R_, HIGH);
        digitalWrite(_IN2_MOTOR_R_, LOW);
        digitalWrite(_IN1_MOTOR_L_, LOW);
        digitalWrite(_IN2_MOTOR_L_, LOW);
        Serial.print("\t 2");
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //2 moteurs
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,255);
        Serial.print("On 2 moteurs \t 1");
        digitalWrite(_IN1_MOTOR_R_, HIGH);
        digitalWrite(_IN2_MOTOR_R_, LOW);
        digitalWrite(_IN1_MOTOR_L_, HIGH);
        digitalWrite(_IN2_MOTOR_L_, LOW);
        Serial.print("\t 2");
        analogWrite(_MOTOR_L_,100);
        analogWrite(_MOTOR_R_,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");


        //Moteur droit
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,255);
        Serial.print("On moteuL droit \t 1");
        digitalWrite(_IN1_MOTOR_R_, HIGH);
        digitalWrite(_IN2_MOTOR_R_, LOW);
        digitalWrite(_IN1_MOTOR_L_, LOW);
        digitalWrite(_IN2_MOTOR_L_, LOW);
        Serial.print("\t 2");
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //Moteur gauche
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,255);
        Serial.print("On moteuL gauche \t 1");
        digitalWrite(_IN1_MOTOR_R_, LOW);
        digitalWrite(_IN2_MOTOR_R_, LOW);
        digitalWrite(_IN1_MOTOR_L_, HIGH);
        digitalWrite(_IN2_MOTOR_L_, LOW);
        Serial.print("\t 2");
        analogWrite(_MOTOR_L_,100);
        analogWrite(_MOTOR_R_,255);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");

        //2 moteurs
        analogWrite(_MOTOR_L_,255);
        analogWrite(_MOTOR_R_,255);
        Serial.print("On 2 moteurs \t 1");
        digitalWrite(_IN1_MOTOR_R_, HIGH);
        digitalWrite(_IN2_MOTOR_R_, LOW);
        digitalWrite(_IN1_MOTOR_L_, HIGH);
        digitalWrite(_IN2_MOTOR_L_, LOW);
        Serial.print("\t 2");
        analogWrite(_MOTOR_L_,100);
        analogWrite(_MOTOR_R_,100);
        Serial.print("\t 3");
        delay(3000);
        Serial.println("\t 4");
}
