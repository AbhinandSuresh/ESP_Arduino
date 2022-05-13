void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
unsigned long nowTime1,nowTime2;
nowTime1= millis();
delay(1000);
nowTime2=millis();
Serial.println(nowTime2-nowTime1);
}
