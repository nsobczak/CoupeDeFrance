#include <Wire.h>
#include "i2cCommunication.h"

#define _RECEIVEADRESS_ 8
#define _SENDADRESS_ 9


//_____________________________________________________________________________________________
//_____________________________________________________________________________________________
// variable to store the reading from analog
int byte1;
int byte2;
int byte3;
byte data[3]; 


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
 
  byte1 = 1;
  byte2 = 2;
  byte3 = 3;

// Write the value of variables to the array
  data[0] = byte1;
  data[1] = byte2;
  data[2] = byte3;

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


