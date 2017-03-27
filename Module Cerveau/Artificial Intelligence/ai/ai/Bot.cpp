/**
 *    \file Bot.cpp
 *    \brief classe robot
 *
 *    \author Nicolas SOBCZAK
 *    \date Mars 2017
 */
//_______________________________________________________________________________________________________
/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Bot.h"


/* ======================================================================================================
 *      Functions
 * ======================================================================================================
 */
//TODO: fonction qui va rammasser les cylindres dans un certain ordre suivant la stratégie
Bot::Bot()
{
}

Bot::Bot(int couleur, int strategie)
{
        //couleur: 1 => bleu; 2 => jaune
        //strategie: 1 => ? ; 2 => ?
        if (couleur != 1 || couleur != 2) couleur = 1;
        if (strategie != 1 || strategie != 2) strategie = 1;
        this->couleur = couleur;
        this->strategie = strategie;
}

Clamp Bot::getClamp()
{
        return this->clamp;
}

void Bot::setClamp(Clamp newClamp)
{
        this->clamp = newClamp;
}
