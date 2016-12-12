  int MotorR =3; // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
  int MotorL = 6; 
  int IN1MotorL = 22;
  int IN2MotorL = 21;
  int IN1MotorR = 52;
  int IN2MotorR = 53;
 
 
void setup() {
  pinMode(MotorR,OUTPUT);
  pinMode(MotorL,OUTPUT);
  pinMode(IN1MotorR,OUTPUT);
  pinMode(IN2MotorR,OUTPUT);
  pinMode(IN1MotorL,OUTPUT);
  pinMode(IN2MotorL,OUTPUT);
  //analogWriteResolution(8);// seulement sur Arduino Due pour gere la Resolution du convertisseur DAC 
}

void loop() {
  /*digitalWrite(IN1MotorR,LOW);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,LOW);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);*/
  //delay(500);
  digitalWrite(IN1MotorR,HIGH);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,HIGH);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);
  /*delay(1000);
  digitalWrite(IN1MotorR,LOW);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,LOW);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);
  delay(500);
  digitalWrite(IN1MotorR,HIGH);
  digitalWrite(IN2MotorR,LOW);
  digitalWrite(IN1MotorL,HIGH);
  digitalWrite(IN2MotorL,LOW);
  analogWrite(MotorR,10);
  analogWrite(MotorL,10);
  delay(1000);*/
}
