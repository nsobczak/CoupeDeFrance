const int PWM = 3;
const int IN1 = 4;
const int IN2 = 5;
void setup() {
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

}

void loop() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(PWM,1);
  delay(5000);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(PWM,100);
  delay(10000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(PWM,1);
  delay(5000);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(PWM,1);
  delay(10000);

}
