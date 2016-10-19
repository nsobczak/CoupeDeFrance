/**
 *    \file Ecran.cpp
 *    \brief Class ecran
 *    \author Nicolas Sobczak
 *    \date octobre 2016
*/


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Ecran.h"


/**____________________________________________________
 *   \fn Ecran::Ecran()
 *   Constructeur
 */
Ecran::Ecran(){
  for (int i = 0; i < 10; ++i){
    this->values[i] = 0;
    this->noms[i] = "";
  }
};


/**____________________________________________________
 * getters
 */
/**
 *   \fn float Ecran::getValue(int i)
 *   \param int i
 */
float Ecran::getValue(int i){
  return this->values[i];
}

/**
 *   \fn String Ecran::getNom(int i)
 *   \param int i
 */
String Ecran::getNom(int i){
  return this->noms[i];
}

/**____________________________________________________
 * setters
 */
/**
 *   \fn void Ecran::setValue(float maValue, int indice)
 *   \param float maValue, int indice
 */
void Ecran::setValue(float maValue, int indice){
  this->values[indice] = maValue;
}

/**
 *   \fn void Ecran::setNom(String monNom, int indice)
 *   \param String monNom, int indice
 */
void Ecran::setNom(String monNom, int indice){
  this->noms[indice] = monNom;
}


/**____________________________________________________
 * autres fonctions
 */





