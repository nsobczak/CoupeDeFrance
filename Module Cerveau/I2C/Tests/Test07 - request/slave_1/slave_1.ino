/**
 *    \file slave_1.cpp
 *    \brief Esclave du Test07 I2C
 *    \author Nicolas Sobczak
 *    \date decembre 2016
*/

#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 8
#define _SENDADRESS_ 9


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
// variable to store the reading from analog
byte data[3];
byte value[2];
int i;

void setup()
{
  Serial.begin(9600);           // Begin Serial coomunication
  Wire.begin(_SENDADRESS_);     // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  i = 0;
}


void loop()
{
// Read from the sensors and assign to variables
  /*
  pot1 = analogRead(A0);
  pot2 = analogRead(A1);
  */

  intTo2Bytes(value, 3092);

// Write the value of variables to the array
  data[0] = value[1];

  i+=1;
  if (i > 6) i = 0;
  data[1] = i + 3;
  data[2] = i + 5;

//Print the array to Serial buffer (for debuging)
  Serial.print("tab : ");
  Serial.print('\t');
  for (int i = 0; i < 3; i++)
  {
    Serial.print(data[i]);
    Serial.print('\t');
  }
  Serial.println();

  delay(700);
}


/**
 * \fn void requestEvent()
 * \brief function that executes whenever data is requested by master this function is registered as an event, see setup()
 */
void requestEvent()
{
  Wire.write(data, 3);
}
