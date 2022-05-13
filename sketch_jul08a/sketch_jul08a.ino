void setup() {
  // put your setup code here, to run once:
pinMode(0,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
float a;
a= analogRead(0);
Serial.print("Input: ");
Serial.println(a);
delay(1000);
}
