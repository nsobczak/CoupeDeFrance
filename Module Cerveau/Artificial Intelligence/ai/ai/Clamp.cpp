/**
 *    \file Clamp.cpp
 *    \brief classe pince
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
 #include "Clamp.h"


/* ======================================================================================================
 *      Constructor, getter, setter
 * ======================================================================================================
 */
Clamp::Clamp()
{
}

/* ======================================================================================================
 *      Methods
 * ======================================================================================================
 */
void Clamp::initialisation()
{
        //TODO: I2C - envoyer info d'initialisation au module pince
}

void Clamp::catchCylinder()
{
        //TODO: I2C - envoyer info d'attraper le cylindre au module pince
}

void Clamp::releaseCylinder()
{
        //TODO: I2C - envoyer info de lacher le cylindre au module pince
}