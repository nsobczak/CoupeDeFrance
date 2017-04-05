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


#define diametreRoueCodeuse 0.05228 // 52,28mm
#define nombreTicksPour1TourDeRoue 1250

const float Pi = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*Pi;

unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse

unsigned long testDuration;
unsigned long testStart;

float consigneVitesseMoteur;
float erreurPrecedenteGauche = 0;
float erreurPrecedenteDroite = 0;
int kp = 50;
long r0 = 1170734.2;//coef qui marche bien : 1829979.4;
long r1 = 3659851.5;//coef qui marche bien : 21525987.0;
int cmdPrecedenteDroite = 0;
int cmdPrecedenteGauche = 0;

unsigned int sommeTickR = 0;
unsigned int sommeTickL = 0;




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

        attachInterrupt(_ENDODER_0_PinA_R_, compteur_tick_R, RISING);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        attachInterrupt(_ENDODER_0_PinA_L_, compteur_tick_L, RISING);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)

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

        consigneVitesseMoteur = 0.3;
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
                        else if ((millis() - testStart >= 10000) && (millis() - testStart < 12500)) consigneVitesseMoteur = 0.5;
                        else if ((millis() - testStart >= 12500) && (millis() - testStart < 15000)) consigneVitesseMoteur = 0.5;
                        else if ((millis() - testStart >= 15000) && (millis() - testStart < 17500)) consigneVitesseMoteur = 0.5;
                        else if ((millis() - testStart >= 17500) && (millis() - testStart < 22500)) consigneVitesseMoteur = 0.5;
                        else if ((millis() - testStart >= 22500) && (millis() - testStart < 25000)) consigneVitesseMoteur = 0.5;


                        double vitesseReelleGauche = calculVitesse(tick_codeuse_L, millis() - testDuration);
                        double vitesseReelleDroite = calculVitesse(tick_codeuse_R, millis() - testDuration);

                        //calcul erreur pour la correction proportionnelle*/
                        float erreurGauche = consigneVitesseMoteur - (float)vitesseReelleGauche;
                        float erreurDroite = consigneVitesseMoteur - (float)vitesseReelleDroite;
                        erreurPrecedenteGauche = erreurGauche;
                        erreurPrecedenteDroite = erreurDroite;
                        int CorrectionVitesse = kp*(tick_codeuse_R-tick_codeuse_L); 
                        int cmdMoteurDroite = r0 * erreurDroite + r1 * erreurPrecedenteDroite + cmdPrecedenteDroite; 
                        int cmdMoteurGauche = r0 * erreurGauche + r1 * erreurPrecedenteGauche + cmdPrecedenteGauche + CorrectionVitesse ;
                        
                        if(cmdMoteurDroite < 0) cmdMoteurDroite = 0;
                        else if (cmdMoteurDroite > 255) cmdMoteurDroite = 255;
                        if(cmdMoteurGauche < 0) cmdMoteurGauche = 0;
                        else if (cmdMoteurGauche > 255) cmdMoteurGauche = 255;
                        cmdPrecedenteDroite = 255-cmdMoteurDroite;
                        cmdPrecedenteGauche = 255-cmdMoteurGauche;



                         robotGoBack(255-cmdMoteurGauche, 255-cmdMoteurDroite);
                        //robotTurnAroundFrontLeft(255-cmdMoteurGauche);
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
                                printDouble(calculVitesse(tick_codeuse_L, millis() - testDuration), 1000000);
                        }
                }
                else if (millis() > 20000) {
                        robotStop();
                }
                testDuration = millis();


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


