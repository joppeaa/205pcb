#include "pinDeclarations.h"

elapsedMillis maindelay;
const unsigned int maindelayMax = 50;

const int LONG_PRESS_MIN  = 1500; // 1500 milliseconds

struct remoteModule
{
  int currentState;
  int lastState = LOW;
  unsigned long pressedTime;
  unsigned long releasedTime;
  bool isPressing = false;
  bool isLongDetected = false;
  bool shortpressFlag = false;
  bool longpressFlag = false;
};

struct remoteModule Abutton,Bbutton,Cbutton,Dbutton;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(200);
  pinMode(13, OUTPUT);
  flashLED(10);

  pinMode(remotePinA, INPUT_PULLDOWN);                                //433 MHz module inputs
  pinMode(remotePinB, INPUT_PULLDOWN);
  pinMode(remotePinC, INPUT_PULLDOWN);
  pinMode(remotePinD, INPUT_PULLDOWN);
}

void loop() {
  // put your main code here, to run repeatedly:

if (maindelay > maindelayMax)                                         //Main high speed loop 
{
  maindelay = 0;                                                      //resetting maindelay timer
  checkremoteInputs();
}
  
}

void checkremoteInputs()
{
  Abutton.currentState = digitalRead(remotePinA);
  if(Abutton.lastState == LOW && Abutton.currentState == HIGH)                //button is pressed
  {
    Abutton.pressedTime = millis();
    Abutton.isPressing = true;
    Abutton.isLongDetected = false;
  }
  else if(Abutton.lastState == HIGH && Abutton.currentState == LOW)           //button is released
  { 
    Abutton.isPressing = false;
    Abutton.releasedTime = millis();
    long pressDurationA = Abutton.releasedTime - Abutton.pressedTime;
    if(pressDurationA < LONG_PRESS_MIN)
    {
      Serial.println("A short press is detected");                    //Short press detected
      Abutton.shortpressFlag = true;
      flashLED(3);
    }
  }
  if(Abutton.isPressing == true && Abutton.isLongDetected == false)
  {
    long pressDurationA = millis() - Abutton.pressedTime;
    if(pressDurationA > LONG_PRESS_MIN) 
    {
      Serial.println("A long press is detected");                     //Long press detected
      
      Abutton.isLongDetected = true;
      Abutton.longpressFlag = true;
      flashLED(20);
    }
  }

  Abutton.lastState = Abutton.currentState;
  
}

void flashLED(int timestoflash)
{
  for (int i = 0; i < timestoflash; i++)
  {
    digitalWrite(13, HIGH);
    delay(30);
    digitalWrite(13, LOW);
    delay(30);
  }
    
}
