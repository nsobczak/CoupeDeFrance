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

void setup() 
{
  Serial.begin(9600);           // Begin Serial coomunication 
  Wire.begin(_SENDADRESS_);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
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
  data[0] = 'v';
  data[1] = value[0];
  data[2] = value[1];

//Print the array to Serial buffer (for debuging)
  Serial.print("tab : ");
  Serial.print('\t');
  for (int i = 0; i < 3; i++)
  {
    Serial.print(data[i]); 
    Serial.print('\t');
  }
  Serial.println();
}


/**
 * \fn void requestEvent()
 * \brief function that executes whenever data is requested by master this function is registered as an event, see setup()
 */
void requestEvent() 
{
  Wire.write(data, 3);
}


