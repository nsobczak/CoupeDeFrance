#include <Wire.h>
#include "i2cCommunication.h"

#define _SENDADRESS_ 8


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
//fonction qui change l'ordre
int changeOrder(int order)
{
  order++;
  if (order==3)
  {
    order = 0;
  }
  return order;
}


//fonction qui envoie 3 octets: une variable et sa valeur
void sendVariable(char variable, int value)
{
  Serial.println("===sendVariable begin===");
  //Variable
  Serial.print("variable ");
  Serial.print(variable);
  Serial.print(" = ");
  Serial.println(variable, DEC);
  i2csend(variable, _SENDADRESS_);
  //Valeur
  Serial.print("valeur test a convertir = ");
  Serial.println(value);
  byte bytesTab[2];
  intTo2Bytes(bytesTab, value);
  uint8_t highByte = bytesTab[0];
  i2csend(highByte, _SENDADRESS_);
  uint8_t lowByte = bytesTab[1];
  i2csend(lowByte, _SENDADRESS_);
  Serial.println("===sendVariable end===");
}
  
  
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup()
{
  Serial.begin(9600);  
}

void loop()
{
  Serial.println("\nLoop begin");
  
  uint8_t order = 0x01;  
  char var = 'r';
  for (int i = 0; i < 3; ++i){
    order = changeOrder(order); 
    sendVariable(var, order);
  }
  
  delay(800);
}

