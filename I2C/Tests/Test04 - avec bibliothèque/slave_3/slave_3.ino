#include <Wire.h>
#include "i2cCommunication.h"

#define _PIN_ 13
#define _SENDADRESS_ 5
#define _RECEIVEADRESS_ 8


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
int changeOrder(int order)
{
  order++;
  if (order==2)
  {
    order = 0;
  }
  return order;
}


void changeMasterLed(byte state)
{
  i2csend(state, _SENDADRESS_);
}


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

void setup()
{
}


int state = 0;

void loop()
{
  i2creceive(_RECEIVEADRESS_); 
  state = digitalRead(_PIN_);
  changeMasterLed(state);
  delay(500);
}


