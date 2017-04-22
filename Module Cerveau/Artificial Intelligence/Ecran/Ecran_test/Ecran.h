/**
 *    \file Ecran.h
 *    \brief Class ecran
 *    \author Nicolas Sobczak
 *    \date octobre 2016
*/


/* ======================================================================================================
 *      Include
 * ======================================================================================================
 */
#include "Arduino.h"


/* ======================================================================================================
 *      Class
 * ======================================================================================================
 */
class Ecran{
  private:
  
    float values[10];
    String noms[10];
    
    
  public:

    Ecran();
    
    float getValue(int i); 
    String getNom(int i);
    
    void setValue(float value, int indice);
    void setNom(String nom, int indice);
    
};

