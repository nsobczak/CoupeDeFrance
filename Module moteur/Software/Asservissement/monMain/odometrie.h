/**
 *    \file odometrie.h
 *    \brief Classe odometrie du grand robot.
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER
 *     Mars 2014
 *Modification: Nicolas SOBCZAK
 *              Octobre 2016
 */
//_______________________________________________________________________________________________________

#ifndef DEF_ODOMETRIE
#define DEF_ODOMETRIE


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "structures.h"


/* ======================================================================================================
 *      Class Odometrie
 * ======================================================================================================
 */
/**
 * \class Odometrie
 * \brief Classe Odometrie
 */
class Odometrie{

    public:

	Odometrie();

    	void calculer_pas();
    	void calculer_variation_distance();
    	void calculer_variation_angle();
        void calculer_ImpulionParmm();
        void calculer_ImpulsionParrad();
    	void calculer_distanceParcourueRoue(Tick codeuse);
    	void retournerValeur(Position *roueCodeuse,Tick codeuse);
    	void calculer_angle(Position *destination, Position robot);
        double calculer_distance(Position destination, Position robot);


    private:

    	double m_perimetre_roue;
    	double m_nombre_ticktour; //nombres de tick par tour
    	double m_impulsionParmm;
        double m_impulsionParrad;

        //double m_variation_distance;
        double m_variation_distance;
        double m_delta_distance;
        double m_memory_variation_distance;

	double m_variation_angle;
        double m_delta_angle;
        double m_delta_angleRad;
        double m_memory_variation_angle;
        double m_variation_angle_moy;
        double m_variation_angle_moyRad;

        double m_distance;
        double m_distance_roue_codeuse;

	double m_DistanceRoueGauche;
	double m_DistanceRoueDroit;

        double K;
	double CORFUGE;
	double M; //masse du robot

};

#endif
