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
/**
 * \fn Clamp::initialisation
 * \brief I2C - envoyer info d'initialisation au module pince
 */
void Clamp::initialisation()
{
        byte bytesTab[2]; // conversion sur 2 octets de la valeur à envoyer
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_INITIALISATION_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}


/**
 * \fn Clamp::catchCylinder
 * \brief I2C - envoyer info d'attraper le cylindre au module pince
 */
void Clamp::catchCylinder()
{
        byte bytesTab[2]; // conversion sur 2 octets de la valeur à envoyer
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_CATCH_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}


/**
 * \fn Clamp::bringUpCylinder
 * \brief I2C - envoyer info d'de monter le cylindre au module pince
 */
void Clamp::bringUpCylinder()
{
        byte bytesTab[2]; // conversion sur 2 octets de la valeur à envoyer
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_BRINGUP_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}


/**
 * \fn Clamp::releaseCylinder
 * \brief I2C - envoyer info de lacher le cylindre au module pince
 */
void Clamp::releaseCylinder()
{
        byte bytesTab[2]; // conversion sur 2 octets de la valeur à envoyer
        intTo2Bytes(bytesTab, 1);
        i2csend3bytes(_CLAMP_RELEASE_, bytesTab[0], bytesTab[1], _CLAMP_SENDADRESS_);
}
