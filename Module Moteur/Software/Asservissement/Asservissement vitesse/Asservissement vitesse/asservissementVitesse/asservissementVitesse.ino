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

/*#define MotorR 3 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define MotorL 6
#define IN1MotorL 22
#define IN2MotorL 23
#define IN1MotorR 52
#define IN2MotorR 53
*/

#define MotorR 6 // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
#define MotorL 3
#define IN1MotorR 52
#define IN2MotorR 53
#define IN1MotorL 22
#define IN2MotorL 23

#define diametreRoueCodeuse 0.05228 // 52,28mm
//#define nombreTicksPour1TourDeRoue 2500 // au lieux de 2500
double nombreTicksPour1TourDeRoue = 2500;
const float Pi = 3.14159;
const float perimetreRoueCodeuse = diametreRoueCodeuse*Pi;

SimpleTimer timer;                 // Timer pour échantillonnage
unsigned int tick_codeuse_R = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_L = 0;   // Compteur de tick de la codeuse
unsigned int tick_codeuse_moyenne = 0;   // Compteur de tick de la codeuse
int cmd = 0;                       // Commande du moteur

const int frequence_echantillonnage = 50;  // Fréquence du pid
const int periode = 1/50;
const int tick_par_tour_codeuse = 2500;      // Nombre de tick codeuse par tour de roue codeuse au lieux de 1250
const int tick_par_tour_non_codeuse = 3836;      // Nombre de tick codeuse par tour de roue non codeuse
const float rapport_roueCodeuse_roueNonCodeuse = (52.28)/(80.22);


double consigne_vitesse_moteur = 0.5;  //  Consigne de vitesse en m/s

double erreur_precedente = consigne_vitesse_moteur;
double somme_erreur = 0;   // Somme des erreurs pour l'intégrateur
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
        double buffer_tick_codeuse_R = tick_codeuse_R;
        // int buffer_tick_codeuse_L = tick_codeuse_L;
        tick_codeuse_R = 0;
        Serial.print("\t buffer_tick_codeuse_R :\t");
        Serial.println(buffer_tick_codeuse_R, 8);
        // tick_codeuse_L = 0;
                Serial.print("\t tick_codeuse_R :\t");
                Serial.println(tick_codeuse_R, 8);
        // Calcul des erreurs
        double nombre_tours = (double) buffer_tick_codeuse_R / (double) nombreTicksPour1TourDeRoue;
        double tour_par_seconde = ((double)nombre_tours / (double)periode)*1000;
        double vitesse = (double)perimetreRoueCodeuse * tour_par_seconde;
        double PWMConsigne = -180.85*consigne_vitesse_moteur;
        double erreur = consigne_vitesse_moteur - vitesse;
        somme_erreur += erreur;
        double delta_erreur = erreur - erreur_precedente;
        erreur_precedente = erreur;

        
        // PID : calcul de la commande255
        cmd = kp*erreur + ki*somme_erreur + kd*delta_erreur+ PWMConsigne;

        // Normalisation et contrôle du moteur
        if(cmd < 0) cmd=255;
        else if(cmd > 255) cmd = 0;
        // analogWrite(Motesse_moteur - vitesse;
        // analogWrite(MotorL, (-1)*(cmd-255));
        //Moteur droit
        digitalWrite(IN1MotorR, LOW);
        digitalWrite(IN2MotorR, HIGH);
        //Moteur gauche
        digitalWrite(IN1MotorL, LOW);
        digitalWrite(IN2MotorL, LOW);
        analogWrite(MotorR, PWMConsigne);
        analogWrite(MotorL, PWMConsigne);

        if(_DEBUG)
        {
                Serial.print("\t PWMConsigne :\t");
                Serial.println(PWMConsigne, 8);

                Serial.print("\t vitesse  :\t");
                //Serial.println(vitesse);
                printDouble( vitesse, 100);

                Serial.print("\t nb_tour_par_sec : \t");
                Serial.println(tour_par_seconde);
        }

}


//______________________________________________________________________________
/**
 * \fn robotGo
 * \brief fonction pour direiger le robot
 * \param int pwm, int direction
 */
void robotGo(int pwm, int direction)
{
        analogWrite(MotorL,255);
        analogWrite(MotorR,255);
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

void robotGoStraightAhead(int vitesse)
{
        robotGo(1, vitesse);
}

void robotGoBack(int vitesse)
{
        robotGo(-1, vitesse);
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
