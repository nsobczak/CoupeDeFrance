/**
 *    \file Clamp.h
 *    \brief classe pince
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Initialisation
 * ======================================================================================================
 */
#include "Arduino.h"
#include "i2cCommunication.h"

#define _CLAMP_SENDADRESS_ 8

#define _CLAMP_INITIALISATION_ 1
#define _CLAMP_CATCH_ 2
#define _CLAMP_BRINGUP_ 3
#define _CLAMP_RELEASE_ 4


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Clamp
{
private:

//____________________
public:

Clamp();

void initialisation();
void catchCylinder();
void bringUpCylinder();
void releaseCylinder();


};
