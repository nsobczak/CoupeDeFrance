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
#define IN1MotorL 22
#define IN2MotorL 23
#define IN1MotorR 52
#define IN2MotorR 53

SimpleTimer timer;                 // Timer pour échantillonnage
unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_moyenne = 0;   // Compteur de tick de la codeuse
int cmd = 0;                       // Commande du moteur

const int frequence_echantillonnage = 50;  // Fréquence du pid
const int tick_par_tour_codeuse = 2500;      // Nombre de tick codeuse par tour de roue codeuse
const int tick_par_tour_non_codeuse = 3836;      // Nombre de tick codeuse par tour de roue non codeuse
const float rapport_roueCodeuse_roueNonCodeuse = (52.28)/(80.22);


float consigne_moteur_nombre_tours_par_seconde = 7.0;  //  Nombre de tours de roue par seconde

float erreur_precedente = consigne_moteur_nombre_tours_par_seconde;
float somme_erreur = 0;   // Somme des erreurs pour l'intégrateur
float kp = 0;           // Coefficient proportionnel
float ki = 0;           // Coefficient intégrateur
float kd = 0;           // Coefficient dérivateur


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
        // Serial.begin(115200);     // Initialisation port COM
        Serial.begin(9600);
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
        digitalWrite(IN2MotorR, HIGH);
        //Moteur gauche
        digitalWrite(IN1MotorL, HIGH);
        digitalWrite(IN2MotorL, LOW);

        delay(5000);              // Pause de 5 sec pour laisser le temps au moteur de s'arréter si celui-ci est en marche

        attachInterrupt(encoder0PinA_R, compteur_tick_R, CHANGE); // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        attachInterrupt(encoder0PinA_L, compteur_tick_L, CHANGE); // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
        timer.setInterval(1000/frequence_echantillonnage, asservissement); // Interruption pour calcul du PID et asservissement


}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop(){
        timer.run();
        delay(10);
}


/**
 * \fn void compteur_tick_R()
 * \brief Interruption sur tick de la codeuse right
 */
void compteur_tick_R(){
        tick_codeuse_R++; // On incrémente le nombre de tick de la codeuse
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
        // Réinitialisation du nombre de tick de la codeuse
        tick_codeuse_moyenne = (tick_codeuse_R + tick_codeuse_L) / 2;
        int tick = tick_codeuse_moyenne;
        tick_codeuse_R = 0;
        tick_codeuse_L = 0;

        // Calcul des erreurs
        int frequence_codeuse = frequence_echantillonnage*tick;
        float nb_tour_par_sec = (float)frequence_codeuse/(float)tick_par_tour_non_codeuse/rapport_roueCodeuse_roueNonCodeuse;
        float erreur = consigne_moteur_nombre_tours_par_seconde - nb_tour_par_sec;
        somme_erreur += erreur;
        float delta_erreur = erreur-erreur_precedente;
        erreur_precedente = erreur;

        // PID : calcul de la commande
        cmd = kp*erreur + ki*somme_erreur + kd*delta_erreur;

        // Normalisation et contrôle du moteur
        //  if(cmd < 0) cmd=255;
        //  else if(cmd > 255) cmd = 0;
        //  analogWrite(MotorR, (-1)*(cmd-255));
        //  analogWrite(MotorL, (-1)*(cmd-255));
        analogWrite(MotorR, 255);
        analogWrite(MotorL, 255);

        if(_DEBUG)
        {
                Serial.print("\t cmd :\t");
                Serial.println(cmd, 8);

                Serial.print("\t tick_codeuse_moyenne  :\t");
                Serial.println(tick_codeuse_moyenne, 8);

                Serial.print("\t nb_tour_par_sec : \t");
                Serial.println(nb_tour_par_sec, 8);
        }


}
