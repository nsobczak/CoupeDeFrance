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
 *      Fonctions
 * ======================================================================================================
 */
#define _DEBUG false
#define encoder0PinA_L 13   //encodeur gauche A
#define encoder0PinB_L 12   //encodeur gauche B
#define encoder0PinA_R 11   //encodeur droit A
#define encoder0PinB_R 10   //encodeur droit B

int MotorR =3; // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
int MotorL = 6;
int IN1MotorL = 22;
int IN2MotorL = 23;
int IN1MotorR = 52;
int IN2MotorR = 53;

SimpleTimer timer;                 // Timer pour échantillonnage
const int _MOTEUR =  9;            // Digital pin pour commande moteur
unsigned int tick_codeuse = 0;     // Compteur de tick de la codeuse
int cmd = 0;                       // Commande du moteur

const int frequence_echantillonnage = 50;  // Fréquence du pid
const int rapport_reducteur = 29;          // Rapport entre le nombre de tours de l'arbre moteur et de la roue
const int tick_par_tour_codeuse = 32;      // Nombre de tick codeuse par tour de l'arbre moteur

float consigne_moteur_nombre_tours_par_seconde = 5.;  //  Nombre de tours de roue par seconde

float erreur_precedente = consigne_moteur_nombre_tours_par_seconde;
float somme_erreur = 0;   // Somme des erreurs pour l'intégrateur
float kp = 300;           // Coefficient proportionnel
float ki = 5.5;           // Coefficient intégrateur
float kd = 100;           // Coefficient dérivateur


/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup()
{
    Serial.begin(115200);         // Initialisation port COM
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

    pinMode(_MOTEUR, OUTPUT);     // Sortie moteur
    analogWrite(_MOTEUR, 255);    // Sortie moteur à 0

    delay(5000);                  // Pause de 5 sec pour laisser le temps au moteur de s'arréter si celui-ci est en marche

    attachInterrupt(0, compteur, CHANGE);    // Interruption sur tick de la codeuse (interruption 0 = pin2 arduino mega)
    timer.setInterval(1000/frequence_echantillonnage, asservissement);  // Interruption pour calcul du PID et asservissement
}


/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
 void loop(){
     timer.run();
     delay(10);
 }


/* Interruption sur tick de la codeuse */
void compteur(){
    tick_codeuse++;  // On incrémente le nombre de tick de la codeuse
}


 /**
  * \fn asservissement()
  * \brief Interruption pour calcul du PID
  */
 void asservissement()
 {
    // Réinitialisation du nombre de tick de la codeuse
    int tick = tick_codeuse;
    tick_codeuse=0;

    // Calcul des erreurs
    int frequence_codeuse = frequence_echantillonnage*tick;
    float nb_tour_par_sec = (float)frequence_codeuse/(float)tick_par_tour_codeuse/(float)rapport_reducteur;
    float erreur = consigne_moteur_nombre_tours_par_seconde - nb_tour_par_sec;
    somme_erreur += erreur;
    float delta_erreur = erreur-erreur_precedente;
    erreur_precedente = erreur;

    // PID : calcul de la commande
    cmd = kp*erreur + ki*somme_erreur + kd*delta_erreur;

    // Normalisation et contrôle du moteur
    if(cmd < 0) cmd=0;
    else if(cmd > 255) cmd = 255;
    analogWrite(_MOTEUR, 255-cmd);

    // DEBUG
    if(_DEBUG)  Serial.println(nb_tour_par_sec,8);
}
