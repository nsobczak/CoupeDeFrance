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
/**fonction qui convertit un entier en 2 bytes - low ici
 * parametres : 1 tableau de byte
 */
byte getLowByte(int n)
{
  byte result;
  
  result = n%256;
  result = byte(result);
  
  return result;
}

/**fonction qui convertit un entier en 2 bytes - high ici
 * parametres : 1 tableau de byte
 */
byte getHighByte(int n)
{
  byte result;
  
  result = n/256;
  result = byte(result);
  
  return result;
}

/**fonction qui affiche les 2 bytes d'un nombre entier convertit en binaire
 * parametres : 1 tableau de byte
 *              1 entier : nombre à convertir
 */
void intTo2Bytes(byte bytesTab[], int n)
{    
  Serial.println("high byte.low byte : ");
  
  bytesTab[0] = getHighByte(n);
  bytesTab[1] = getLowByte(n);
  Serial.print(bytesTab[0], HEX);
  Serial.print(".");
  Serial.println(bytesTab[1], HEX);
}


/**fonction qui affiche un entier ayant ete convertit en 2 bytes binaire
 * parametres : 1 tableau de byte
 *              1 entier : nombre à convertir
 */
int recoverIntFrom2Bytes(byte bytesTab[])
{    
  int result;
  
  result = bytesTab[0]*256;
  result += bytesTab[1];
  Serial.print("Recover : ");
  Serial.println(result);
  
  return result;
}

  
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
void setup()
{
}


int sendadress = 6;
int test = 1;
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
  
  Serial.println("===test begin===");
  Serial.print("valeur test a convertir : ");
  Serial.println(test);
  byte bytesTab[2];
  intTo2Bytes(bytesTab, test);
  recoverIntFrom2Bytes(bytesTab);
  test += 200;
  Serial.println("===test end===");
}

