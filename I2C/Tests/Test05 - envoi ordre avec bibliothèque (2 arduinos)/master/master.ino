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


  
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup()
{
  Serial.begin(9600);  
}

uint8_t order = 0x01;
char var;

void loop()
{
  Serial.println("\nLoop begin");

  //i2csend(order, _SENDADRESS_);
  //i2csend(order, _SENDADRESS_);
  
  Serial.println("===test begin===");
  //Variable
  var = 'r';
  i2csend(var, _SENDADRESS_);
  //Valeur
  Serial.print("valeur test a convertir : ");
  Serial.println(order);
  order = changeOrder(order); 
  byte bytesTab[2];
  intTo2Bytes(bytesTab, order);
  uint8_t highByte = bytesTab[0];
  i2csend(highByte, _SENDADRESS_);
  uint8_t lowByte = bytesTab[1];
  i2csend(lowByte, _SENDADRESS_);
  //recoverIntFrom2Bytes(bytesTab);
  Serial.println("===test end===");
  
  delay(500);
}

