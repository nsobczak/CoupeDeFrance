/**
 *    \file application.ino
 *    \brief asservissement vitesse
 *
 *    \author Arthur Duytschaever & Nicolas SOBCZAK
 *    \date Février 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include <Arduino.h>
#include <SimpleTimer.h>           // http://arduino.cc/playground/Code/SimpleTimer
#include <math.h>


/* ======================================================================================================
 *      Variables globales
 * ======================================================================================================
 */
#define _DEBUG true

#define encoder0PinA_L 13   //encodeur gauche A
#define encoder0PinB_L 12   //encodeur gauche B
#define encoder0PinA_R 11   //encodeur droit A
#define encoder0PinB_R 10   //encodeur droit B

#define MotorR 3 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define MotorL 6
#define IN1MotorL 52
#define IN2MotorL 53
#define IN1MotorR 22
#define IN2MotorR 23

const float diametreRoueCodeuse = 0.05228; // 52,28mm
const float Pi = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*Pi;
const float nombreTicksPour1TourDeRoue = 1250;

SimpleTimer timer;                 // Timer pour échantillonnage
unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_moyenne = 0;   // Compteur de tick de la codeuse
float cmd = 0;                       // Commande du moteur

const int frequence_echantillonnage = 50;  // Fréquence du pid
const float periode = 1.0/(float)frequence_echantillonnage;
const int tick_par_tour_codeuse = 2500;      // Nombre de tick codeuse par tour de roue codeuse
const int tick_par_tour_non_codeuse = 3836;      // Nombre de tick codeuse par tour de roue non codeuse
const float rapport_roueCodeuse_roueNonCodeuse = (52.28)/(80.22);

float consigne_vitesse_moteur = 1.0;  //  Consigne de vitesse en m/s

float erreur_precedente = consigne_vitesse_moteur;
float somme_erreur = 0;   // Somme des erreurs pour l'intégrateur
// Gains du PID
float kp = 0;           // Gain proportionnel
float ki = 0;           // Gain intégrateur
float kd = 0;           // Gain dérivateur


/* ======================================================================================================
 *      Fonctions
 * ======================================================================================================
 */


void initializeSetup()
{
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

        //Moteur droit
        digitalWrite(IN1MotorR, LOW);
        digitalWrite(IN2MotorR, LOW);
        //Moteur gauche
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);

        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
}

/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
        // Serial.begin(115200);     // Initialisation port COM
        Serial.begin(9600);
        initializeSetup();
        delay(5000);              // Pause de 5 sec pour laisser le temps au moteur de s'arréter si celui-ci est en marche

        attachInterrupt(encoder0PinA_R, compteur_tick_R, RISING); // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        attachInterrupt(encoder0PinA_L, compteur_tick_L, RISING); // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        timer.setInterval(1000/frequence_echantillonnage, asservissement); // Interruption pour calcul du PID et asservissement
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop()
{
        timer.run();
        delay(10);
}


/**
 * \fn void compteur_tick_R()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_R(){
        tick_codeuse_R++;                 // On incrémente le nombre de tick de la codeuse
}


/**
 * \fn void compteur_tick_L()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_L(){
        tick_codeuse_L++; // On incrémente le nombre de tick de la codeuse
}


/**
 * \fn asservissement()
 * \brief Interruption pour calcul du PID
 */
void asservissement()
{
        // Réinitialisation du nombre de ticks de la codeuse
        int buffer_tick_codeuse_R = tick_codeuse_R;
        // int buffer_tick_codeuse_L = tick_codeuse_L;
        tick_codeuse_R = 0;
        // tick_codeuse_L = 0;

        // Calculs vitesse et p
        float nombre_tours = (float) buffer_tick_codeuse_R / (float) nombreTicksPour1TourDeRoue;
        float tour_par_seconde = (float)nombre_tours * (float)frequence_echantillonnage;
        float vitesse = (float)perimetreRoueCodeuse * tour_par_seconde;

        float consignePWM = calculConsignePWM(consigne_vitesse_moteur);  //-180.85*consigne_vitesse_moteur - vitesse;

        // Calcul des erreurs
        float erreur = consigne_vitesse_moteur - vitesse;
        somme_erreur += erreur;
        float delta_erreur = erreur - erreur_precedente;
        erreur_precedente = erreur;

        // PID : calcul de la commande
        cmd = kp*erreur + ki*somme_erreur + kd*delta_erreur + consignePWM;
        // Normalisation et contrôle du moteur
        if(cmd < 0) cmd=255;
        else if(cmd > 255) cmd = 0;
        robotTurnAroundFrontRight(cmd);
        // robotGoStraightAhead(cmd);


        if(_DEBUG)
        {
                // Serial.print("\n\t buffer_tick_codeuse_R  :\t");
                // Serial.println(buffer_tick_codeuse_R, 1);
                //
                // Serial.print("\t nombreTicksPour1TourDeRoue  :\t");
                // Serial.println(nombreTicksPour1TourDeRoue, 1);
                //
                // Serial.print("\t nombre_tours  :\t");
                // Serial.println(nombre_tours, 6);
                //
                // Serial.print("\t periode  :\t");
                // Serial.println(periode, 6);
                //
                // Serial.print("\t nb_tour_par_sec : \t");
                // Serial.println(tour_par_seconde, 6);

                Serial.print("\t vitesse  :\t");
                Serial.println(vitesse, 6);

                Serial.print("\t consignePWM  :\t");
                Serial.println(consignePWM, 6);
                //
                // Serial.print("\t cmd :\t");
                // Serial.println(cmd, 6);
        }

}


/**
 * \fn calculConsignePWM(double y)
 * \brief fonction contenant l'équation permettant de convertir la consigne vitesse en consigne PWM
 * \param double y, y > 1,5056
 * \return x
 */
float calculConsignePWM(float consigne_vitesse_moteur)
{
        // Sécurité
        if(consigne_vitesse_moteur < 0) consigne_vitesse_moteur = 0.0;
        else if(consigne_vitesse_moteur > 1.421) consigne_vitesse_moteur = 1.421;
        return sqrt(1.0/(-0.00002) * (consigne_vitesse_moteur - 1.5056) ) - 65.0;
}


//______________________________________________________________________________
/**
 * \fn robotGo
 * \brief fonction pour diriger le robot
 * \param int pwm, int direction
 */
void robotGo(int pwm, int direction)
{
        switch (direction) {
        case 1:
                digitalWrite(IN1MotorR, HIGH);
                digitalWrite(IN2MotorR, LOW);
                digitalWrite(IN1MotorL, HIGH);
                digitalWrite(IN2MotorL, LOW);
                break;
        case -1:
                digitalWrite(IN1MotorR, LOW);
                digitalWrite(IN2MotorR, HIGH);
                digitalWrite(IN1MotorL, LOW);
                digitalWrite(IN2MotorL, HIGH);
                break;
        default:
                digitalWrite(IN1MotorR, LOW);
                digitalWrite(IN2MotorR, LOW);
                digitalWrite(IN1MotorL, LOW);
                digitalWrite(IN2MotorL, LOW);
                break;
        }
        analogWrite(MotorL, pwm);
        analogWrite(MotorR, pwm);
}

void robotStop()
{
        robotGo(0, 0);
}

void robotGoStraightAhead(int pwm)
{
        robotGo(pwm, 1);
}

void robotGoBack(int pwm)
{
        robotGo(pwm, -1);
}

/**
 * \fn robotTurnAround
 * \brief fonction faisant tourner le robot sur lui-même avec un seul moteur
 * \param int pwm, int direction
 */
void robotTurnAround(int pwm, int direction)
{
        switch (direction) {
        case 1:
                digitalWrite(IN1MotorR, HIGH);
                digitalWrite(IN2MotorR, LOW);
                digitalWrite(IN1MotorL, LOW);
                digitalWrite(IN2MotorL, LOW);
                analogWrite(MotorL, 255);
                analogWrite(MotorR, pwm);
                break;
        case -1:
                digitalWrite(IN1MotorR, LOW);
                digitalWrite(IN2MotorR, HIGH);
                digitalWrite(IN1MotorL, LOW);
                digitalWrite(IN2MotorL, LOW);
                analogWrite(MotorL, 255);
                analogWrite(MotorR, pwm);
                break;
        case 2:
                digitalWrite(IN1MotorR, LOW);
                digitalWrite(IN2MotorR, LOW);
                digitalWrite(IN1MotorL, HIGH);
                digitalWrite(IN2MotorL, LOW);
                analogWrite(MotorL, pwm);
                analogWrite(MotorR, 255);
                break;
        case -2:
                digitalWrite(IN1MotorR, LOW);
                digitalWrite(IN2MotorR, LOW);
                digitalWrite(IN1MotorL, LOW);
                digitalWrite(IN2MotorL, HIGH);
                analogWrite(MotorL, pwm);
                analogWrite(MotorR, 255);
                break;
        default:
                robotStop();
                break;
        }
}

void robotTurnAroundFrontRight(int pwm)
{
        robotTurnAround(pwm, 1);
}

void robotTurnAroundBackRight(int pwm)
{
        robotTurnAround(pwm, -1);
}

void robotTurnAroundFrontLeft(int pwm)
{
        robotTurnAround(pwm, 2);
}

void robotTurnAroundBackLeft(int pwm)
{
        robotTurnAround(pwm, -2);
}


//______________________________________________________________________________
void printDouble( double val, unsigned int precision)
{
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
