#include <Wire.h>

void setup()
{
  Wire.begin(); // joindre le bus i2c (adresse est optionnelle pour un maître)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(4); // Commencer transmission vers l'esclave  #4
  Wire.write("x is ");       // Envoi de 5 octets (5 bytes)
  Wire.write(x);             // envoi d'un byte/octet (valeur numérique)  
  Wire.endTransmission();    // fin transmission

  x++;
  delay(500); // delay de 500 ms
}

