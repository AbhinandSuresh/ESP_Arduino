#define SWITCH 4            //switch pin
#define DEBOUNCE_TIME 20    //debounce check delay
long timePressed = 0;       //time of key press in milliseconds
long snapShot = 0;
int onFlag = 1;             //invert logic
void setup()
{
  pinMode(SWITCH, INPUT);
  Serial.begin(9600);
}

void loop()
{
  if (digitalRead(SWITCH) == HIGH && onFlag)
  {
    Serial.println("ON");
    snapShot = millis();
    Serial.print("Snapshot: ");
    Serial.println(snapShot);
    onFlag = 0;
  }
  else if (onFlag == 0) //if on
  {
    if (digitalRead(SWITCH) == LOW)
    {
      timePressed = millis() - snapShot;
      //return(timePressed)
      Serial.println(timePressed);
      onFlag=1;
    }
  }
}
