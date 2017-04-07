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
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_INITIALISATION_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}

void Clamp::catchCylinder()
{
        //TODO: I2C - envoyer info d'attraper le cylindre au module pince
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_CATCH_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}

void Clamp::releaseCylinder()
{
        //TODO: I2C - envoyer info de lacher le cylindre au module pince
        // conversion sur 2 octets de la valeur à envoyer
        byte bytesTab[2];
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_RELEASE_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}
