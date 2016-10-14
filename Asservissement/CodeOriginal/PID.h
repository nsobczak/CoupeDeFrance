/*
 *Class PID Proportionnel Intégrateur Dérivateur du grand robot
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par: Clément LETELLIER 
 *     Mars 2014
*/

#ifndef DEF_PID
#define DEF_PID

#include "structures.h"
#include "odometrie.h"

#include "spark_wiring.h"
#include "application.h"


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