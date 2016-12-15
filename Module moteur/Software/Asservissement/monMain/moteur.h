/**
 *    \file moteur.h
 *    \brief Classe moteur du grand robot
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER
 *     Mars 2014
 *Modification: Nicolas SOBCZAK
 *              Octobre 2016
 */
//_______________________________________________________________________________________________________


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#ifndef DEF_MOTEUR
#define DEF_MOTEUR


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "odometrie.h"
#include "Arduino.h"


/* ======================================================================================================
 *      Define
 * ======================================================================================================
 */
#define PMW_MOTEUR_A0  5 //IN1         //moteur Gauche
//#define PMW_MOTEUR_A1  6 //IN2
#define PMW_MOTEUR_B0  3               //moteur Droit
//#define PMW_MOTEUR_B1  4


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
/**
 * \class Moteur
 * \brief Classe Moteur
 */
class Moteur
{
  public:
        Moteur();
        void initPWM();
        int convertir_pourcentage_en_octet ();
        void brake(int choix_moteur);
	void fonctionnement_moteur(double vitesseGauche, double vitesseDroit);
	//void analogWrite2(uint16_t pin, uint8_t value);
        //void envoieData();

  private:
        byte m_PWM_G[2];
        byte m_PWM_D[2];
        double m_vitesse_moteur;
};

#endif
