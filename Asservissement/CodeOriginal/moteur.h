/*
 *Class moteur du grand robot
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER 
 *     Mars 2014
*/

#ifndef DEF_MOTEUR
#define DEF_MOTEUR


#define PMW_MOTEUR_A0  A5               //moteur Gauche
#define PMW_MOTEUR_A1  A4               

#define PMW_MOTEUR_B0  A7               //Moteur Droit
#define PMW_MOTEUR_B1  A6


#include "spark_wiring.h"
#include "application.h"
#include "odometrie.h"


class Moteur{

	public:

	Moteur();

        void initPWM();
        int convertir_pourcentage_en_octet (); 
        
        void brake(int choix_moteur);
	    void fonctionnement_moteur(double vitesseGauche, double vitesseDroit);
	    void analogWrite2(uint16_t pin, uint8_t value);
        void envoieData();
        
	private:
        byte m_PWM_G[2];
        byte m_PWM_D[2];
	double m_vitesse_moteur;
	
       
};

#endif