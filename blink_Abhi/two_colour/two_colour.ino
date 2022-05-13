int led1 = 13;
int led2 = 9;
int led3 = 2;
void setup() {
  // put your setup code here, to run once:

pinMode (led1,OUTPUT);
pinMode (led2,OUTPUT);
pinMode (led3,OUTPUT);
}

void loop() {
digitalWrite(led1,LOW);
digitalWrite(led2,HIGH);
digitalWrite(led3,HIGH);
delay(150);
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
digitalWrite(led3,LOW);
delay(150);
digitalWrite(led1,LOW);
digitalWrite(led2,HIGH);
digitalWrite(led3,HIGH);
delay(150);
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
digitalWrite(led3,LOW);
delay(150);digitalWrite(led1,LOW);
digitalWrite(led2,HIGH);
digitalWrite(led3,HIGH);
delay(150);
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
digitalWrite(led3,LOW);
delay(150);

digitalWrite(led1,LOW);
digitalWrite(led2,HIGH);
digitalWrite(led3,HIGH);
delay(500);
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
digitalWrite(led3,LOW);
delay(500);
digitalWrite(led1,LOW);
digitalWrite(led2,HIGH);
digitalWrite(led3,HIGH);
delay(500);
digitalWrite(led1,HIGH);
digitalWrite(led2,HIGH);
digitalWrite(led3,LOW);
delay(500);
}
