#include <Wire.h>

// Fonction qui est exécutée lorsque des données sont envoyées par le Maître.
// Cette fonction est enregistrée comme une événement ("event" en anglais), voir la fonction setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // Lire tous les octets sauf le dernier
  {
    char c = Wire.read();     // lecture de l'octet/byte comme caractère
    Serial.print(c);          // afficher le caractère
  }
  int x = Wire.read();        // lecture de l'octet/byte ignoré comme un entier
  Serial.println(x);          // Afficher la valeur numérique 
}


void i2creceive()
{
  Wire.begin(4);                // Joindre le Bus I2C avec adresse #4
  Wire.onReceive(receiveEvent); // enregistrer l'événement (lorsqu'une demande arrive)
  Serial.begin(9600);           // Démarrer une communication série
}


void setup()
{
  i2creceive();
}


void loop()
{
  delay(100);
}


