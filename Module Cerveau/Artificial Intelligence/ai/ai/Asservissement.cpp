/**
 *    \file Asservissement.cpp
 *    \brief classe asservissement
 *
 *    \author Nicolas SOBCZAK
 *    \date Avril 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Asservissement.h"


/* ======================================================================================================
 *      Constructor, getter, setter
 * ======================================================================================================
 */

Asservissement::Asservissement()
{
}

/** \fn void Asservissement::botGoForward
 *  \param int sendAddress, int speed
 *  \brief fonction qui déplace le robot vers l'avant
 */
void Asservissement::botGoForward(double distance, double speed)
{
        //TODO: I2C - envoyer info de déplacer le robot au module asservissement
        int distanceIntPart = (int)distance;
        double distanceDoubleDecPart = (distance - distanceIntPart)*_DISTANCE_PRECISION_;
        int distanceIntDecPart = (int)distanceDoubleDecPart;

        int speedIntPart = (int)speed;
        double speedDoubleDecPart = (speed - speedIntPart)*_SPEED_PRECISION_;
        int speedIntDecPart = (int)speedDoubleDecPart;

        i2csend5bytes(_ASSERVISSMENT_BOTGOFORWARD_, speedIntPart, speedIntDecPart, distanceIntPart, distanceIntDecPart, _ASSERVISSMENT_SENDADRESS_);
}

/** \fn void Asservissement::botGoBackward
 *  \param int sendAddress, int speed
 *  \brief fonction qui déplace le robot vers l'arrière
 */
void Asservissement::botGoBackward(double distance, double speed)
{
        //TODO: I2C - envoyer info de déplacer le robot au module asservissement
        int distanceIntPart = (int)distance;
        double distanceDoubleDecPart = (distance - distanceIntPart)*_DISTANCE_PRECISION_;
        int distanceIntDecPart = (int)distanceDoubleDecPart;

        int speedIntPart = (int)speed;
        double speedDoubleDecPart = (speed - speedIntPart)*_SPEED_PRECISION_;
        int speedIntDecPart = (int)speedDoubleDecPart;

        i2csend5bytes(_ASSERVISSMENT_BOTGOBACKWARD_, speedIntPart, speedIntDecPart, distanceIntPart, distanceIntDecPart, _ASSERVISSMENT_SENDADRESS_);
}

void Asservissement::botTurnAroundRight(double speed)
{
        //TODO: I2C - envoyer info de tourner le robot vers la droite au module asservissement
        int intPart = (int)speed;
        double doubleDecPart = (speed - intPart)*_SPEED_PRECISION_;
        int intDecPart = (int)doubleDecPart;
        i2csend3bytes(_ASSERVISSMENT_BOTTURNRIGHT_, intPart, intDecPart, _ASSERVISSMENT_SENDADRESS_);
}

void Asservissement::botTurnAroundLeft(double speed)
{
        //TODO: I2C - envoyer info de tourner le robot vers la gauche au module asservissement
        int intPart = (int)speed;
        double doubleDecPart = (speed - intPart)*_SPEED_PRECISION_;
        int intDecPart = (int)doubleDecPart;
        i2csend3bytes(_ASSERVISSMENT_BOTTURNLEFT_, intPart, intDecPart, _ASSERVISSMENT_SENDADRESS_);
}

void Asservissement::botStop()
{
        //TODO: I2C - envoyer info d'arrêter le robot au module asservissement
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_ASSERVISSMENT_BOTSTOP_, bytesTab[0], bytesTab[1], _ASSERVISSMENT_SENDADRESS_);
}
