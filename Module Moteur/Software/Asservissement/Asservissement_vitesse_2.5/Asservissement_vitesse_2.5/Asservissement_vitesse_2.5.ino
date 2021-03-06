/**
 *    \file asservissement_vitesse_2.5.ino
 *    \brief code d'asservissement
 *
 *    \author Arthur Duytschaever et Nicolas Sobczak
 *    \date Avril 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include <Arduino.h>
#include <Wire.h>
#include "i2cCommunication.h"
#include <SimpleTimer.h>
#include "botDirection.h"


#define _DEBUG_ true
#define _TEST_SANS_I2C_ true


//=== I2C ===
#define _ASSERVISSEMENT_SENDRECEIVEADRESS_ 6

#define _ASSERVISSMENT_BOTGOFORWARD_ 1
#define _ASSERVISSMENT_BOTGOBACKWARD_ 2
#define _ASSERVISSMENT_BOTTURNRIGHT_ 3
#define _ASSERVISSMENT_BOTTURNLEFT_ 4
#define _ASSERVISSMENT_BOTSTOP_ 5

#define _SPEED_PRECISION_ 100
#define _DISTANCE_PRECISION_ 100

int variableSent = 10;


//=== ENCODEUR ===
#define _ENDODER_0_PinA_L_ 13   //encodeur gauche A
#define _ENDODER_0_PinB_L_ 12   //encodeur gauche B
#define _ENDODER_0_PinA_R_ 11   //encodeur droit A
#define _ENDODER_0_PinB_R_ 10   //encodeur droit B


//=== CONSTANTES ===
#define diametreRoueCodeuse 0.05228 // 52,28mm
#define nombreTicksPour1TourDeRoue 1250

const float _MAX_SPEED_RIGHT_ = 1.41;
const float _MAX_SPEED_LEFT_ = 1.41;
const int _MAX_PWM_ = 255;
const float _PI_ = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*_PI_;
const float _VOIE_ROUES_ = 0.2575; //en metre, ecart entre les roues d'un même essieu


//=== VARIABLES ===
unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse


//=== ASSERVISSEMENT ===
SimpleTimer timer;
unsigned int FREQUENCE_ECHANTILLIONNAGE = 50; // en Hz
unsigned long _PERIODE_ASSERVISSEMENT_ = (1000/FREQUENCE_ECHANTILLIONNAGE); // en ms !
unsigned long testStart;

// Vitesse
float consigneVitesseMoteur;
float erreurPrecedenteGauche;
float erreurPrecedenteDroite;
float cmdPrecedenteDroitePourcentage;
float cmdPrecedenteGauchePourcentage;

//TODO: update kp, r0, r1 values

// correction proportionnelle sur la vitesse pour realiser une ligne droite
float kp = 0.0;

// coeff correcteur PI moteur droit
float R0_MOTOR_RIGHT = 904.49;//-272.70;//-2385.96;//-2298.55;//9.742;//1170734.2; //coeff qui marche bien : 1829979.4;
float R1_MOTOR_RIGHT = 2779.98;//-1023.57;//-8905.34;//-7046.59;//29.94;//3659851.5; //coeff qui marche bien : 21525987.0;
float IRIGHT = R0_MOTOR_RIGHT - (R1_MOTOR_RIGHT/FREQUENCE_ECHANTILLIONNAGE);

// coeff correcteur PI moteur gauche
float R0_MOTOR_LEFT = 922.54;//-277.24;//-2425.72;//-2517.57;//9.926;//1170734.2;
float R1_MOTOR_LEFT = 2835.70;//-1040.63;//-9053.78;//-7716.98;//30.51;//3659851.5;
float ILEFT = R0_MOTOR_LEFT - (R1_MOTOR_LEFT/FREQUENCE_ECHANTILLIONNAGE);


// Position
float consigneDistance;
int somme_ordre_tick_codeuse_L = 0;
int somme_ordre_tick_codeuse_R = 0;
int somme_ordre_tick_codeuse_L_to_be_sent = somme_ordre_tick_codeuse_L;
int somme_ordre_tick_codeuse_R_to_be_sent = somme_ordre_tick_codeuse_R;
byte order = 5;
int ordre_termine = 1;



/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */

/**
 * \fn void compteur_tick_R()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_R()
{
        tick_codeuse_R++;  // On incrémente le nombre de ticks de la codeuse
}


/**
 * \fn void compteur_tick_L()
 * \brief Interruption sur tick de la codeuse left
 */
void compteur_tick_L()
{
        tick_codeuse_L++;  // On incrémente le nombre de ticks de la codeuse
}


/**
 * \fn calculDistance
 * \param unsigned int tick_codeuse
 * \brief calcule la distance parcourue à partir d'un nombre de ticks
 * \return float distance parcourue en metre
 */
float calculDistance(unsigned int tick_codeuse)
{
        double nombre_tours = (double) tick_codeuse / (double) nombreTicksPour1TourDeRoue;
        return perimetreRoueCodeuse * (float)nombre_tours;
}


/**
 * \fn calculDistanceFromAngle
 * \param float angle
 * \brief calcule la distance à parcourir à pour effectuer un angle donné
 * \return float distance à parcourir en metre
 */
float calculDistanceFromAngle(float angle)
{
        return _VOIE_ROUES_*angle;
}


/**
 * \fn calculVitesse
 * \param unsigned int tick_codeuse, unsigned long duration
 * \brief calcule la vitesse instantannée
 * \return double vitesse
 */
double calculVitesse(unsigned int tick_codeuse, unsigned long duration)
{
        double nombre_tours = (double) tick_codeuse / (double) nombreTicksPour1TourDeRoue;
        double tour_par_seconde = ((double)nombre_tours/(double)duration)*1000;
        return (double)perimetreRoueCodeuse * tour_par_seconde;
}


/**
 * \fn printDouble
 * \param double val, unsigned int precision
 * \brief prints val with number of decimal places determine by precision
 * NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
 * example: printDouble(3.1415, 100); // prints 3.14 (two decimal places)
 */
void printDouble(double val, unsigned int precision)
{
        Serial.print (int(val));  //prints the int part
        Serial.print(".");  // print the decimal point
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

        Serial.println(frac, DEC);
}


//______________________________________________________________________________
/**
 * \fn executeOrder
 * \param int order, int cmdMoteurGauche, int cmdMoteurDroite
 * \brief function that execute a botDirection order from the boolean "order"
 */
void executeOrder(int order, int cmdMoteurGauche, int cmdMoteurDroite)
{
        switch ( order ) // cf. les références des variables en haut du fichier
        {
        case 1:
                if (_DEBUG_) Serial.println("ordre execute : _ASSERVISSMENT_BOTGOFORWARD_");
                robotGoStraightAhead(_MAX_PWM_ - cmdMoteurGauche, _MAX_PWM_ - cmdMoteurDroite);
                break;
        case 2:
                if (_DEBUG_) Serial.println("ordre execute : _ASSERVISSMENT_BOTGOBACKWARD_");
                robotGoBack(_MAX_PWM_ - cmdMoteurGauche, _MAX_PWM_ - cmdMoteurDroite);
                break;
        case 3:
                if (_DEBUG_) Serial.println("ordre execute : _ASSERVISSMENT_BOTTURNRIGHT_");
                robotTurnAroundFrontRight(_MAX_PWM_ - cmdMoteurGauche);
                break;
        case 4:
                if (_DEBUG_) Serial.println("ordre execute : _ASSERVISSMENT_BOTTURNLEFT_");
                robotTurnAroundFrontLeft(_MAX_PWM_ - cmdMoteurDroite);
                break;
        case 5:
                if (_DEBUG_) Serial.println("ordre execute : _ASSERVISSMENT_BOTSTOP_");
                consigneVitesseMoteur = 0;
                robotStop();
                ordre_termine = 1;
                break;
        default:
                if (_DEBUG_) Serial.println("variable recue inconnue");
                ordre_termine = 1;
                break;
        }
}


/**
 * \fn handleOrder
 * \param int cmdMoteurGauche, int cmdMoteurDroite
 * \brief function that handles running and not running orders, also handles the "ordre_termine" boolean
 */
void handleOrder(int cmdMoteurGauche, int cmdMoteurDroite)
{
        if(ordre_termine == 1)
        {
                order = 5;
                somme_ordre_tick_codeuse_R = 0;
                somme_ordre_tick_codeuse_L = 0;
        }
        else
        {
                somme_ordre_tick_codeuse_R += tick_codeuse_R;
                somme_ordre_tick_codeuse_L += tick_codeuse_L;
                somme_ordre_tick_codeuse_L_to_be_sent = somme_ordre_tick_codeuse_L;
                somme_ordre_tick_codeuse_R_to_be_sent = somme_ordre_tick_codeuse_R;
        }
        executeOrder(order, cmdMoteurGauche, cmdMoteurDroite);
        if (order == 3 || order == 4)
        {
                if (calculDistanceFromAngle(somme_ordre_tick_codeuse_L) >= consigneDistance ||
                    calculDistanceFromAngle(somme_ordre_tick_codeuse_R) >= consigneDistance )
                {ordre_termine = 1; if (_DEBUG_) Serial.println("\t \t \n\nordre termine\n\n"); }
        }
        else
        {
                if (calculDistance(somme_ordre_tick_codeuse_L) >= consigneDistance ||
                    calculDistance(somme_ordre_tick_codeuse_R) >= consigneDistance )
                {ordre_termine = 1; if (_DEBUG_) Serial.println("\t \t \n\nordre termine\n\n"); }
        }
}


/**
 * \fn bornCommand
 * \param float command
 * \brief function that keeps a pwm command between 0 and 100
 */
float bornCommandLimitateur(float command, float maxSpeed)
{
        float depassementMax = 150 * consigneVitesseMoteur/maxSpeed;
        if(command < 0) command = 0;
        else if(command > depassementMax) command = depassementMax;
        return command;
}

float bornCommand(float command)
{
        if(command < 0) command = 0;
        else if(command > 100) command = 100;
        return command;
}


/**
 * \fn convertCommandFromPourcentageToPWM
 */
float convertCommandFromPourcentageToPWM(float command)
{
        return 255 - command*2.55;
}


/**
 * \fn asservissementVitesse
 * \brief function that does the asservissment work
 */
void asservissementVitesse()
{
        // Calcul erreur pour la correction proportionnelle
        double vitesseReelleGauche = calculVitesse(tick_codeuse_L, _PERIODE_ASSERVISSEMENT_);
        double vitesseReelleDroite = calculVitesse(tick_codeuse_R, _PERIODE_ASSERVISSEMENT_);

        float erreurGauche = consigneVitesseMoteur - (float)vitesseReelleGauche;
        float erreurDroite = consigneVitesseMoteur - (float)vitesseReelleDroite;

        //float CorrectionVitesse = kp * ((float)vitesseReelleDroite - (float)vitesseReelleGauche);
        float cmdMoteurDroitePourcentage = R0_MOTOR_RIGHT * erreurDroite -  IRIGHT * erreurPrecedenteDroite + (float)cmdPrecedenteDroitePourcentage;
        float cmdMoteurGauchePourcentage = R0_MOTOR_LEFT * erreurGauche - ILEFT * erreurPrecedenteGauche + (float)cmdPrecedenteGauchePourcentage;// + CorrectionVitesse;
    
        cmdMoteurDroitePourcentage = bornCommand(cmdMoteurDroitePourcentage);
        cmdMoteurGauchePourcentage = bornCommand(cmdMoteurGauchePourcentage);
        
        cmdPrecedenteDroitePourcentage = cmdMoteurDroitePourcentage;
        cmdPrecedenteGauchePourcentage = cmdMoteurGauchePourcentage;
        
        erreurPrecedenteGauche = erreurGauche;
        erreurPrecedenteDroite = erreurDroite;

        float cmdMoteurDroite = convertCommandFromPourcentageToPWM(cmdMoteurDroitePourcentage);
        float cmdMoteurGauche = convertCommandFromPourcentageToPWM(cmdMoteurGauchePourcentage);



        // = Ordre =
        if (_TEST_SANS_I2C_) robotGoBack(cmdMoteurGauche,cmdMoteurDroite);
        else handleOrder(cmdMoteurGauche, cmdMoteurDroite);


        // = Debug =
        if (_DEBUG_) {
                // Serial.print("\t tick_codeuse_L : \t");
                // Serial.println(tick_codeuse_L);

                // TODO: ne pas oublier de changer l'encodeur L ou R
                // Serial.print("\t consignePWM : \t " );
                // Serial.println(cmdMoteurDroit);
                // Serial.print("\t consigneVitesseMoteur : \t");
                //Serial.println(erreurGauche);
                //Serial.println(erreurDroite);
                // Serial.print("\t calculVitesse : \t " );
                printDouble(calculVitesse(tick_codeuse_L, _PERIODE_ASSERVISSEMENT_), 1000000);
                printDouble(calculVitesse(tick_codeuse_R, _PERIODE_ASSERVISSEMENT_), 1000000);
        }

        tick_codeuse_R = 0;
        tick_codeuse_L = 0;
}


//_____________________________________________________________________________________________________________
/**
 * \fn void requestEvent()
 * \brief function executed whenever data is requested by master this function is registered as an event, see setup()
 */
void asservissementRequestEvent()
{
        /*Notes: tableau id i2c
           10 => ordre_termine;
           11 => somme_ordre_tick_codeuse_L;
           12 => somme_ordre_tick_codeuse_R;
         */
        if ( variableSent > 12) variableSent = 10;
        byte bytesTab[2];
        byte data[3];

        switch (variableSent) {
        case 10:
                intTo2Bytes(bytesTab, ordre_termine);  // conversion sur 2 octets de la valeur à envoyer
                data[0] = variableSent;
                data[1] = bytesTab[0];
                data[2] = bytesTab[1];
                Wire.write(data, 3);
                break;
        case 11:
                intTo2Bytes(bytesTab, somme_ordre_tick_codeuse_L_to_be_sent);  // conversion sur 2 octets de la valeur à envoyer
                data[0] = variableSent;
                data[1] = bytesTab[0];
                data[2] = bytesTab[1];
                Wire.write(data, 3);
                break;
        case 12:
                intTo2Bytes(bytesTab, somme_ordre_tick_codeuse_R_to_be_sent);  // conversion sur 2 octets de la valeur à envoyer
                data[0] = variableSent;
                data[1] = bytesTab[0];
                data[2] = bytesTab[1];
                Wire.write(data, 3);
                break;
        default:
                if (_DEBUG_) Serial.println("resquestEvent: variable recue inconnue");
                break;
        }
        variableSent++;
}



/**
 * \fn void receiveEvent(int howMany - fonction qui est exécutée lorsque des données sont envoyées par le Maître. Cette fonction est enregistrée comme un événement ("event" en anglais), voir la fonction setup()
 * \param int howMany
 * \brief function called by the asservissementI2CReceive function
 */
void asservissementReceiveEvent(int howMany)
{
        if (ordre_termine == 1)
        {
                if (Wire.available() == 5)
                {
                        //lecture de la variable
                        byte var = Wire.read();
                        //lecture des 2 octets suivants + reconstitution de la valeur
                        byte distanceIntPart = Wire.read();
                        byte distanceIntDecPart = Wire.read();
                        consigneDistance  = recoverFloatFrom2Bytes(distanceIntPart, distanceIntDecPart, _DISTANCE_PRECISION_);
                        //lecture des 2 octets suivants + reconstitution de la valeur
                        byte speedIntPart = Wire.read();
                        byte speedIntDecPart = Wire.read();
                        consigneVitesseMoteur = recoverFloatFrom2Bytes(speedIntPart, speedIntDecPart, _SPEED_PRECISION_);

                        switch ( var ) // cf. les références des variables en haut du fichier
                        {
                        case 1 ... 5:
                                if (var == 3 || var == 4) consigneDistance = calculDistanceFromAngle(consigneDistance);
                                if (_DEBUG_) {Serial.println("ordre recu");
                                              Serial.print("consigneDistance = "); Serial.println(consigneDistance); }
                                ordre_termine = 0;
                                order = var;
                                break;
                        default:
                                if (_DEBUG_) Serial.println("variable recue inconnue");
                                ordre_termine = 1;
                        }
                }
                else if (_DEBUG_) Serial.println("Erreur : Pas 3 octets envoyes");
        }
}


//____________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        if (_DEBUG_) Serial.begin(115200);   // Initialisation port COM
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

        cmdPrecedenteDroitePourcentage = 0;
        cmdPrecedenteGauchePourcentage = 0;
        erreurPrecedenteGauche = 0;
        erreurPrecedenteDroite = 0;

        /*
           === Remarques ===
           Pour changer le pwm: 0 -> 255
           100% = 0
           75% = 63
           50% = 127
           25% = 191
           0% = 255
         */
        analogWrite(_MOTOR_L_, _MAX_PWM_);
        analogWrite(_MOTOR_R_, _MAX_PWM_);
        robotStop();

        if (_TEST_SANS_I2C_) 
        {
                testStart = millis();
                consigneVitesseMoteur = 0.2;
        }
        else
        {
                Wire.begin(_ASSERVISSEMENT_SENDRECEIVEADRESS_);
                Wire.onRequest(asservissementRequestEvent);
                Wire.onReceive(asservissementReceiveEvent);
        }
        timer.setInterval(_PERIODE_ASSERVISSEMENT_, asservissementVitesse);  // Interruption pour calcul du PID et asservissement
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop()
{
        if (_TEST_SANS_I2C_)
        {
                if ((millis() - testStart > 5000) && (millis() - testStart < 25000))
                {
                        // if ((millis() - testStart >= 5000) && (millis() - testStart < 10000)) consigneVitesseMoteur = 0.5;
                        // else if ((millis() - testStart >= 10000) && (millis() - testStart < 12500)) consigneVitesseMoteur = 0.5;
                        // else if ((millis() - testStart >= 12500) && (millis() - testStart < 15000)) consigneVitesseMoteur = 0.5;
                        // else if ((millis() - testStart >= 15000) && (millis() - testStart < 17500)) consigneVitesseMoteur = 0.5;
                        // else if ((millis() - testStart >= 17500) && (millis() - testStart < 22500)) consigneVitesseMoteur = 0.5;
                        // else if ((millis() - testStart >= 22500) && (millis() - testStart < 25000)) consigneVitesseMoteur = 0.5;
                        timer.run();
                }
                else robotStop();
        }
        else timer.run();
}
