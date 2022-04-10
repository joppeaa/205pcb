#include "pinDeclarations.h"

elapsedMillis maindelay;
const unsigned int maindelayMax = 50;

const int LONG_PRESS_MIN  = 1500; // 1000 milliseconds

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

remoteModule Abutton,Bbutton,Cbutton,Dbutton;

remoteModule remoteButtons[] = {Abutton, Bbutton, Cbutton, Dbutton};

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
  for (int i = 0; i<sizeof(remoteButtons); i++) 
  {
    checkremoteInput(remoteButtons[i]);
  }
  if (Abutton.longpressFlag == true)
  {
    Serial.print("Is wouter een flikert?");
    Serial.println(Abutton.longpressFlag);
  }
}
  
}

void checkremoteInput(remoteModule buttonToCheck)
{
  buttonToCheck.currentState = digitalRead(remotePinA);
  if(buttonToCheck.lastState == LOW && buttonToCheck.currentState == HIGH)                //button is pressed
  {
    buttonToCheck.pressedTime = millis();
    buttonToCheck.isPressing = true;
    buttonToCheck.isLongDetected = false;
  }
  else if(buttonToCheck.lastState == HIGH && buttonToCheck.currentState == LOW)           //button is released
  { 
    buttonToCheck.isPressing = false;
    buttonToCheck.releasedTime = millis();
    long pressDurationA = buttonToCheck.releasedTime - buttonToCheck.pressedTime;
    if(pressDurationA < LONG_PRESS_MIN)
    {
      Serial.println("A short press is detected");                    //Short press detected
      buttonToCheck.shortpressFlag = true;
      flashLED(3);
    }
  }
  if(buttonToCheck.isPressing == true && buttonToCheck.isLongDetected == false)
  {
    long pressDurationA = millis() - buttonToCheck.pressedTime;
    if(pressDurationA > LONG_PRESS_MIN) 
    {
      Serial.println("A long press is detected");                     //Long press detected
      
      buttonToCheck.isLongDetected = true;
      buttonToCheck.longpressFlag = true;
      flashLED(20);
    }
  }

  buttonToCheck.lastState = buttonToCheck.currentState;
  
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
