#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 5


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
//fonction qui change l'ordre
int changeOrder(int order)
{
  order++;
  if (order==2)
  {
    order = 0;
  }
  return order;
}


//fonction qui change l'addresse
int swapAdress(int adress)
{
  adress++;
  if (adress==9)
  {
    adress = 6;
  }
  return adress;
}

  
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup()
{
}


int sendadress = 6;
uint8_t order = 0;

void loop()
{
  order = changeOrder(order); 
  for (int i=0; i<3; ++i)
  {
    i2csend(order, sendadress);
    sendadress = swapAdress(sendadress);
    delay(400);
  }
  i2creceive(_RECEIVEADRESS_);
  Serial.println(order); 
  delay(500);
}

