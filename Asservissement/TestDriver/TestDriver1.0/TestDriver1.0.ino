 int pwm1 =3; 
 int IN1 = 4;
 int IN2 = 5;
void setup() {
  pinMode(3,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  analogWriteResolution(8);// seulement sur Arduino Due 
}

void loop() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(pwm1,1000);
  delay(5000);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(3,100);
  delay(10000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(3,10);
  delay(5000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(3,10);
  delay(10000);

}
