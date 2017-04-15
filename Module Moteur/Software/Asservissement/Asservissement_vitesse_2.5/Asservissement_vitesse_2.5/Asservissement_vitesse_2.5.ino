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
#define _TEST_SANS_I2C_ false

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

const int _MAX_PWM_ = 255;
const float _PI_ = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*_PI_;

//=== VARIABLES ===
unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse

//=== ASSERVISSEMENT ===
SimpleTimer timer;
unsigned long _PERIODE_ASSERVISSEMENT_ = 20;
unsigned long testStart;

// Vitesse
float consigneVitesseMoteur;
float erreurPrecedenteGauche = 0;
float erreurPrecedenteDroite = 0;
//TODO: update kp, r0, r1 values
int kp = 1;
long r0 = 1170734.2; //coeff qui marche bien : 1829979.4;
long r1 = 3659851.5; //coeff qui marche bien : 21525987.0;
int cmdPrecedenteDroite = 0;
int cmdPrecedenteGauche = 0;

// Position
float consigneDistance;
int somme_ordre_tick_codeuse_L = 0;
int somme_ordre_tick_codeuse_R = 0;
byte order = 5;
int ordre_termine = 1;


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */

/**
 * \fn void requestEvent()
 * \brief function that executes whenever data is requested by master this function is registered as an event, see setup()
 */
void asservissementRequestEvent()
{
        /*Notes: tableau id i2c
           10 => ordre_termine;
           11 => somme_ordre_tick_codeuse_L;
           12 => somme_ordre_tick_codeuse_R;
         */
        if ( variableSent > 12) variableSent = 10;
        if (_DEBUG_) Serial.println("request received");
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
                intTo2Bytes(bytesTab, somme_ordre_tick_codeuse_L);  // conversion sur 2 octets de la valeur à envoyer
                data[0] = variableSent;
                data[1] = bytesTab[0];
                data[2] = bytesTab[1];
                Wire.write(data, 3);
                break;
        case 12:
                intTo2Bytes(bytesTab, somme_ordre_tick_codeuse_R);  // conversion sur 2 octets de la valeur à envoyer
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
 */
void asservissementReceiveEvent(int howMany)
{
        if (Wire.available() == 5)
        {
                //lecture de la variable
                byte var = Wire.read();
                //lecture des 2 octets suivants
                byte distanceIntPart = Wire.read();
                byte distanceIntDecPart = Wire.read();
                //reconstitution de la valeur
                distanceIntDecPart *= _DISTANCE_PRECISION_;
                consigneDistance = (float)distanceIntPart + (float)distanceIntDecPart;
                //lecture des 2 octets suivants
                byte speedIntPart = Wire.read();
                byte speedIntDecPart = Wire.read();
                //reconstitution de la valeur
                speedIntDecPart *= _SPEED_PRECISION_;
                consigneVitesseMoteur = (float)speedIntPart + (float)speedIntDecPart;

                switch ( var ) // cf. les références des variables en haut du fichier
                {
                case 1 ... 5:
                        if (_DEBUG_) Serial.println("ordre recu");
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


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void asservissementI2CReceive()
{
        // Wire.begin(adresse);     // Joindre le Bus I2C avec adresse
        Wire.onReceive(asservissementReceiveEvent); // enregistrer l'événement (lorsqu'une demande arrive)
        Wire.endTransmission(); // fin transmission
}


//____________________________________________________________________________________________________
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
 * \return double distance parcourue
 */
double calculDistance(unsigned int tick_codeuse)
{
        double nombre_tours = (double) tick_codeuse / (double) nombreTicksPour1TourDeRoue;
        return (double)perimetreRoueCodeuse * nombre_tours;
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
int bornCommand(int command)
{
        if(command < 0) command = 0;
        else if (command > _MAX_PWM_) command = _MAX_PWM_;
        return command;
}


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


void asservissementVitesse()
{
        double vitesseReelleGauche = calculVitesse(tick_codeuse_L, _PERIODE_ASSERVISSEMENT_);
        double vitesseReelleDroite = calculVitesse(tick_codeuse_R, _PERIODE_ASSERVISSEMENT_);

        //calcul erreur pour la correction proportionnelle
        float erreurGauche = consigneVitesseMoteur - (float)vitesseReelleGauche;
        float erreurDroite = consigneVitesseMoteur - (float)vitesseReelleDroite;
        erreurPrecedenteGauche = erreurGauche;
        erreurPrecedenteDroite = erreurDroite;
        int CorrectionVitesse = kp*(tick_codeuse_R-tick_codeuse_L);
        int cmdMoteurDroite = r0 * erreurDroite + r1 * erreurPrecedenteDroite + cmdPrecedenteDroite;
        int cmdMoteurGauche = r0 * erreurGauche + r1 * erreurPrecedenteGauche + cmdPrecedenteGauche + CorrectionVitesse;

        cmdMoteurDroite = bornCommand(cmdMoteurDroite);
        cmdMoteurGauche = bornCommand(cmdMoteurGauche);
        cmdPrecedenteDroite = _MAX_PWM_ - cmdMoteurDroite;
        cmdPrecedenteGauche = _MAX_PWM_ - cmdMoteurGauche;

        if (_TEST_SANS_I2C_) robotGoBack(_MAX_PWM_ - cmdMoteurGauche, _MAX_PWM_ - cmdMoteurDroite);
        else {
                if(ordre_termine == 1)
                {
                        ordre_termine = 0;
                        somme_ordre_tick_codeuse_R = 0;
                        somme_ordre_tick_codeuse_L = 0;
                }
                else
                {
                        somme_ordre_tick_codeuse_R += tick_codeuse_R;
                        somme_ordre_tick_codeuse_L += tick_codeuse_L;
                }
                executeOrder(order, cmdMoteurGauche, cmdMoteurDroite);
        }

        if (_DEBUG_) {
                // Serial.print("\t tick_codeuse_L : \t");
                // Serial.println(tick_codeuse_L);

                // TODO: ne pas oublier de changer l'encodeur L ou R
                // Serial.print("\t consignePWM : \t " );
                // Serial.println(cmdMoteurDroit);
                // Serial.print("\t consigneVitesseMoteur : \t");
                Serial.println(consigneVitesseMoteur);
                // Serial.print("\t calculVitesse : \t " );
                printDouble(calculVitesse(tick_codeuse_L, _PERIODE_ASSERVISSEMENT_), 1000000);
        }

        tick_codeuse_R = 0;
        tick_codeuse_L = 0;
}


//_____________________________________________________________________________________________________________
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

        if (_TEST_SANS_I2C_) {
                testStart = millis();
        }else{
                Wire.begin(_ASSERVISSEMENT_SENDRECEIVEADRESS_);
                Wire.onRequest(asservissementRequestEvent); //
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
                        consigneVitesseMoteur = 0.5;
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
        else
        {
                timer.run();
                if (ordre_termine == 1) asservissementI2CReceive();
        }
}
