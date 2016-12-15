/**
 *    \file PID.h
 *    \brief Class PID Proportionnel Intégrateur Dérivateur du grand robot.
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER
 *     Mars 2014
 *Modification: Nicolas SOBCZAK
 *              Octobre 2016
 */
//_______________________________________________________________________________________________________
#ifndef DEF_PID
#define DEF_PID


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "structures.h"
#include "odometrie.h"


/* ======================================================================================================
 *      Class PID
 * ======================================================================================================
 */
/**
 * \class PID
 * \brief Classe pour le correcteur Proportionnel Intégrateur Dérivé
 */
class PID{

	public:

        PID();
        PID(double kP,double kI,double kD,double periode);
        void resetPID (double error);
        double computePID(double error);
        

	private:

	double m_kP;     //Coefficient proportionnel
        double m_kI;     //Coefficient Intégrateur
        double m_kD;     //Coefficient Dérivateur

        double m_sumError; //somme des erreurs
        double m_lastError; //dernière erreur
        double m_diffError; //différences erreurs

        double m_periode;
};

#endif
