#include "Ecran.h"

//Constructeurs
Ecran::Ecran(){
  for (int i = 0; i < 10; ++i){
    this->values[i] = 0;
    this->noms[i] = "";
  }
};


//getters
float Ecran::getValue(int i){
  return this->values[i];
}

String Ecran::getNom(int i){
  return this->noms[i];
}


//setters
void Ecran::setValue(float maValue, int indice){
  this->values[indice] = maValue;
}

void Ecran::setNom(String monNom, int indice){
  this->noms[indice] = monNom;
}



