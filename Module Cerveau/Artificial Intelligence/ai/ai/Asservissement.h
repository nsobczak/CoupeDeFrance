/**
 *    \file Asservissement.h
 *    \brief classe asservissement
 *
 *    \author Nicolas SOBCZAK
 *    \date Avril 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
 #include "Arduino.h"
 #include "i2cCommunication.h"


 #define _ASSERVISSMENT_SENDADRESS_ 6

 #define _ASSERVISSMENT_BOTGOFORWARD_ 1
 #define _ASSERVISSMENT_BOTGOBACKWARD_ 2
 #define _ASSERVISSMENT_BOTTURNRIGHT_ 3
 #define _ASSERVISSMENT_BOTTURNLEFT_ 4
 #define _ASSERVISSMENT_BOTSTOP_ 5


 #define _PRECISION_ 1000

/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Asservissement
{
private:

public:
Asservissement();

void botGoForward(double speed);
void botGoBackward(double speed);
void botTurnAroundRight(double speed);
void botTurnAroundLeft(double speed);
void botStop();

};
