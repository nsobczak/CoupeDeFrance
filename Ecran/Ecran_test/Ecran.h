#include "Arduino.h"

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

