/*
* Ce programme permet de réaliser:
* l'asservissement en vitesse d'un moteur à courant continu à l'aide d'un régulateur PID
* l'asservissement de la position de la roue (du robot mobile) pour le faire avancer de x mètres: Distance désirée
* Gilles TAGNE - Janvier 2017
*/
 
#include <SimpleTimer.h>           // http://arduino.cc/playground/Code/SimpleTimer pour télécharger la bibliothèque à inclure dans notre projet


/* DECLARATION DES VARIABLES GLOBALES */

// Définition de la fréquence d'échantillonnade de l'asservissement
SimpleTimer timer_asservissement;         // Déclaration du Timer qui sera utilisé pour la boucle de l'asservissement en vitesse (échantillonnage)
const int frequence_echantillonnage_asservissement = 50;  // Fréquence du contôleur PID  en Hz
const int periode_echantillonnage_asservissement_ms = 1000/frequence_echantillonnage_asservissement;  // Période d'échantillonnage du contôleur PID en milliseconde (ms)

// Paramètres du moteur + encodeur
const int rapport_reducteur_moteur = 53;          // Rapport entre le nombre de tours de l'arbre moteur et de la roue
const int nb_impulsions_par_tour_Codeuse = 3;      // Nombre d'impulsions de la codeuse (ou de encodeur) par tour de l'arbre moteur
const int rapport_global = rapport_reducteur_moteur*nb_impulsions_par_tour_Codeuse;      // Nombre global d'impulsions correspondant à un tour de la roue du moteur DC
const int Pin_PWM_MOTEUR_DC =  5;               // Pin digital servant à commandeer le moteur (doit être une sortie PWM)

// Elements pour l'asservissement en position : le principe sera de compter le nombre d'impulsions correspondant à la distance désirée
float consigne_distance_a_parcourir = 0.2*10;  //  distance désirée en m (pour mon exemple 10 tr de roue, sachant que 1 tr =0.2m). Mettre une grande valeur pour une rotation à infini
const float diametre_roue = 6.4/100;      // Diametre de la roue en m
// Calcul du nombre tick codeuse necessaire pour effectuer la distance désirée
const int nombre_tick_codeuse_necessaire = (consigne_distance_a_parcourir*rapport_global)/(3.14*diametre_roue);      // Nombre d'impulsions correspondants à la distance désirée

 // Elements pour asservissement en vitesse
float consigne_vitesse_moteur_nombre_tours_par_seconde = 1;  //  Vitesse désirée de la roue en tour par seconde (tr/s)
// Gains du PID
float Kp = 1;           // Gain proportionnel
float Ki = 0.05;        // Gain intégrateur
float Kd = 0;           // Gain dérivateur
// Variables permettant de calculer les erreurs et la commande
float erreur_precedente = 0;
float somme_erreur = 0; // Somme des erreurs pour l'intégrateur
float cmd = 0;                   // Commande calculée à appliquer au moteur (en Volt)
int cmd_normalisee_PWM = 0;           // Commande à appliquer au PWM piloter le moteur DC(0 à 255)  

// Variables permettant de compter le nombre d'impulsions de la codeuse ( incrémentation quand il y a une interruption)
unsigned int tick_codeuse_ass_vitesse = 0;    // Compteur de tick de la codeuse pour l'asservissement de la vitesse du moteur DC
unsigned int tick_codeuse_ass_position = 0;    // Compteur de tick de la codeuse pour l'asservissement en position (pour s'arrêtr à une distance donnée)


 
/* INITIALISATION DES ENTREES/SORTIES ET DES PROCESSUS */
void setup() {
    pinMode(Pin_PWM_MOTEUR_DC, OUTPUT);     // Initialisation du Pin moteur comme une Sortie
    analogWrite(Pin_PWM_MOTEUR_DC, 0);    // Initialisation de la Sortie moteur à 0 au début

    // Init et Run du processus d'interruption permettant de compter les inpulsions de la codeuse
    // A chaque fois qu'il y aura un front montant ("RISING") sur le pin interruption "0" de la carte Arduino, la fonction "compteur_et_condition_arret_ass_position" va s'executer
    // (permettant ainsi de compter les ticks le plus rapidement possible)
    attachInterrupt(0, compteur_et_condition_arret_ass_position, RISING);    // Interruption sur tick de la codeuse (interruption 0 = pin2 sur arduino uno/mega).
    
    // Initialisation du processus (ou fonction) "asservissement" qui s'execute toutes les périodes d'échantillionnage
    timer_asservissement.setInterval(periode_echantillonnage_asservissement_ms, asservissement);  // Interruption pour calcul du PID et asservissement toutes les periode_echantillonnage_asservissement_ms
    Serial.begin(9600);         // Initialisation du port COM pour visialiser les variables pendant l'execution du programme si besoin
}
 
/* PROGRAMME (fonction principale) */
void loop(){
    timer_asservissement.run(); // Démarrer l'asservissement
    delay(10);
}
 
/* Interruption sur tick de la codeuse */
void compteur_et_condition_arret_ass_position(){
    tick_codeuse_ass_vitesse++;  // On incrémente le nombre de tick de la codeuse
    tick_codeuse_ass_position++;
    if (tick_codeuse_ass_position>=nombre_tick_codeuse_necessaire) // On vérifie si le robot (moteur) est arrivé à la distance désirée
    consigne_vitesse_moteur_nombre_tours_par_seconde = 0;  // si oui, on arrête le moteur (vitesse nulle)
}
 
/* PROCESSUS asservissement permettant d'effectuer l'asservissement en vitesse (contrôleur PID) */
void asservissement()
{
  /** Elements pour asservissement en vitesse
 *  Le principe sera de :
 *  I - Modéliser le moteur afin d'avoir une approximation de la relation entre la tension moyenne appliquée (U) et la vitesse ( V en tr/s),
 *  II - Calculer la commande équivalente (tention) à appliquer pour se rapprocher de la vitesse désirée (permet d'eviter des grands gains Kp Ki Kd et de contrôler facilement le system)
 *  III - Calculer la correction PID à ajouter pour obtenir la vitesse désirée
 *  **/
 
   // I + II : Modélisation et calcul la commande équivalente
  // Calcul du terme direct (commande équivalente) permettant de se rapprocher rapidement du point de fonctionnement désiré en utilisant l'approximation du modèle linéaire du moteur:
  // V = a U + b, avec V: vitesse en tr/s, U: tension en Volt (comprise entre 0 et 5V), a et b deux constantes à determiner. de cette relation on obtient: U_O = (V_desiree - b)/a
  // Pour le moteur que j'utilise b = -0.22, a = 0.44 (Approximativement)
   float cmd_0 = (consigne_vitesse_moteur_nombre_tours_par_seconde + 0.22)/0.44; // Commande du moteur en tension en le positionant le plus proche du point de fonctionnement (Volt) //float cmd_0 = 0; // Commande du moteur en tension

   // III - Calculer la correction PID à ajouter pour obtenir la vitesse désirée
   
   // III- 1) Compter le nombre d'impulsions pendant une période d'echantillonnage pour estimer la vittesse du moteur
   // Pour ce faire, on récupère la valeur actuelle et réinitialise le nombre de tick de la codeuse pour asservissement en vitesse
    int nb_tick_par_periode_echantillonnage_asservissement = tick_codeuse_ass_vitesse;
    tick_codeuse_ass_vitesse=0; // Réinitialisation du nombre de tick de la codeuse
     int frequence_codeuse = frequence_echantillonnage_asservissement*nb_tick_par_periode_echantillonnage_asservissement;
    float vitesse_moteur_nb_tour_par_sec = (float)frequence_codeuse/(float)rapport_global;
    // Une fois la vitesse estimée, il faut calculer les erreurs ...
    
    // III- 2)Calcul des erreurs
    float erreur = consigne_vitesse_moteur_nombre_tours_par_seconde - vitesse_moteur_nb_tour_par_sec;
    somme_erreur += erreur;
    float delta_erreur = erreur-erreur_precedente;
    erreur_precedente = erreur;
 
    // III- 3) Calcul de la commande: correction PID + commande équivalente (cmd_0)
    cmd = Kp*erreur + Ki*somme_erreur + Kd*delta_erreur + cmd_0; // Cette commande est la tension moyenne en volt qu'il faut appliquer à notre moteur DC (comprise dans notre cas entre 0 et 5V voir II)
 
    // Normalisation de la commande pour application au PWM
    cmd_normalisee_PWM = cmd*255/5; // Considerant que la tension appliquée au moteur varie entre 0 et 5V
    if(cmd_normalisee_PWM < 0) cmd_normalisee_PWM =0;
    else if(cmd_normalisee_PWM > 255) cmd_normalisee_PWM = 255;
    analogWrite(Pin_PWM_MOTEUR_DC, cmd_normalisee_PWM);
    
    Serial.println(vitesse_moteur_nb_tour_par_sec,8);
}
