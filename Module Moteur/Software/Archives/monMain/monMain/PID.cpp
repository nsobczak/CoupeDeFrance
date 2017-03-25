/**
 *    \file PID.cpp
 *    \brief Code du correcteur Proportionnel Intégrateur Dérivateur (PID) du grand Robot. Permet de corriger la consigne de sortie en fonction de la consigne d'entrée (éviter les oscilations de position et d'angle)
 *
 *Codé pour "Spark core"  www.spark.io
 *
 *Par : Thibaut LOCQUET
 *      Octobre 2012
 *Modification: Clément LETELLIER
 *              Mars 2014
 *Modification: Nicolas SOBCZAK
 *              Octobre 2016
 */
//_______________________________________________________________________________________________________


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "structures.h"
#include "odometrie.h"
#include "PID.h"


/* ======================================================================================================
 *      Fonctions de la classe PID
 * ======================================================================================================
 */
/**
 * \fn PID  
 * \brief constructeur qui initialise le PID
 * \param double kP, double kI, double kD coeff du PID utilise pour calculer le PWM
 * \param double periode ???
 */
PID::PID(double kP, double kI, double kD, double periode)
{

    //init coeff
    m_kP = kP;
    m_kI = kI;
    m_kD = kD;

    //init values
    m_sumError = 0;
    m_lastError = 0;
    m_diffError = 0;

    m_periode = periode;
}


/**
 * \fn PID  
 * \brief constructeur sans paramètres qui initialise le PID
 */
PID::PID()
{
}


//_______________________________________________________________________________________________________
/**
 * \fn PID  
 * \brief fonction qui reset le PID
 * \param double error reset le PID avec la derreniere error
 */
void PID::resetPID(double error)
{
    m_lastError = error;
    m_sumError = 0;
    m_diffError = 0;
}


//_______________________________________________________________________________________________________
/**
 * \fn PID  
 * \brief fonction qui calcule le PID 
 * \param double error utiliser pour avoir l'erreur l'actuel 
 * \return retourne la valeur de PID calculée
 */
double PID::computePID(double error)
{

    double P, I, D;
    double PID;

    m_sumError = m_sumError + error;

    //calcule la variation d'erreur
    m_diffError = error - m_lastError;

    //remplace dernière erreur
    m_lastError = error;


    //Propotionnel
    P = error * m_kP;

    //Intégrateur
    I = m_sumError * m_kI;// * m_periode;

    //Dérivateur
    D = (m_diffError * m_kD);// m_periode ;

    PID = P + I + D;

    /*Serial.print ("error :\t");
    Serial.println (error);
    Serial.print ("m_diffError :\t");
    Serial.println (m_diffError);
    Serial.print ("m_sumError :\t");
    Serial.println ( m_sumError);
    Serial.print ("P :\t");
    Serial.println (P);
    Serial.print ("I :\t");
    Serial.println (I);
    Serial.print ("D :\t");
    Serial.println (D);
    */
    return PID;
}
