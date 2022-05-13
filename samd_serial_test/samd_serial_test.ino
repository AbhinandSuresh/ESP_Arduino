void setup() {
  Serial.begin(9600);
}
void loop() {
    int input;
  input=Serial.read();
  Serial.print("Output is: ");
  Serial.print(input);
}
