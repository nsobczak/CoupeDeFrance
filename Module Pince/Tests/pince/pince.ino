/* |=====================|
 * | Gestion de la pince |
 * |=====================|
 */
/**
 *    \file pince.cpp
 *    \brief Gestion de la pince
 *    \author Olivier JOMBART
 *    \date Janvier 2016
*/
//_____________________________________________________________________________________________
//_____________________________________________________________________________________________

#include <Servo.h>


Servo servo_capture;        // create servo object to control a servo
Servo servo_rotation;


/**
 * \fn void attraper_cylindre(int angle_fermeture, int angle_rotation_droite,int temps)
 * \brief, Fn qui permet d'attraper et de retourner verticalement le cylindre
 */
 
void attraper_cylindre(int angle_fermeture, int angle_rotation_droite,int temps){  
    
  servo_capture.write(angle_fermeture);                              // la pince se ferme (100)
  delay(temps);                      
  servo_rotation.write(angle_rotation_droite);                       // rotation de la pince vers la droite (20)
  delay(temps);      
  
}
/**
 * \fn void relacher_cylindre(int angle_ouverture, int angle_rotation_initial, int temps)
 * \brief, Fn qui permet de relacher le cylindre après sa capture, la pince revient à son état inital (ouverte)
 */
void relacher_cylindre(int angle_ouverture, int angle_rotation_initial, int temps) {
  servo_capture.write(angle_ouverture);                             // la pince s'ouvre (140)
  delay(temps);
  servo_rotation.write(angle_rotation_initial);                     // rotation à l'état initial de la pince (80)
  delay(temps);
}


//____________________________________________________________________________________________________
//____________________________________________________________________________________________________
/**
 * \fn void setup()
 * \brief fonction setup d'arduino
 */
void setup() {
  Serial.begin(9600);
  servo_capture.attach(5);                                 // attaches the servo on pin 2 to the servo object
  servo_rotation.attach(4);                                // attaches the servo on pin 3 to the servo object
}

/**
 * \fn void loop()
 * \brief fonction loop d'arduino
 */
void loop() 
{
  attraper_cylindre(150,75,1000);          // angle à respecter
  relacher_cylindre(80,155,1000);          // angle à respecter
  }
