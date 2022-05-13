//author Abhi
//IoTuneZ Internship

void setup() {
Serial.begin(9600);
}

void loop() {
  if(analogRead(0)<120 &&(analogRead(0)>60)){
Serial.println(analogRead(0));
delay(100);
}}
