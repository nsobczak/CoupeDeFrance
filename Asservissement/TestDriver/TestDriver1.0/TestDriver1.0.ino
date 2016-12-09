 int pwm1 =3; // Attention sur Due PWM ou Pwm sont des keyword -> donc ne pas les utiliser pour des nom de variable
 const int pwm2 = 6; 
 int IN1 = 50;
 int IN2 = 49;
 
void setup() {
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  //analogWriteResolution(8);// seulement sur Arduino Due pour gere la Resolution du convertisseur DAC 
}

void loop() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  delay(2000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(pwm1,10);
  analogWrite(pwm2,10);
  delay(5000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  delay(2000);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(pwm1,10);
  analogWrite(pwm2,10);
  delay(5000);

}
