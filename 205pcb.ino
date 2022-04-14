#include "pinDeclarations.h"

elapsedMillis maindelay;
const unsigned int maindelayMax = 50;

const int LONG_PRESS_MIN  = 1500; // 1000 milliseconds

struct remoteModule
{
  byte id;
  byte modulePin;
  int currentState;
  int lastState = LOW;
  unsigned long pressedTime;
  unsigned long releasedTime;
  bool isPressing = false;
  bool isLongDetected = false;
  bool shortpressFlag = false;
  bool longpressFlag = false;
} buttonNr[4];

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
  
  buttonNr[0].modulePin = 14;
  buttonNr[1].modulePin = 15;
  buttonNr[2].modulePin = 16;
  buttonNr[3].modulePin = 17;
  
  buttonNr[0].id = 1;
  buttonNr[1].id = 2;
  buttonNr[2].id = 3;
  buttonNr[3].id = 4;
}

void loop() {
  // put your main code here, to run repeatedly:

if (maindelay > maindelayMax)                                         //Main high speed loop 
{
  maindelay = 0;                                                      //resetting maindelay timer
  for (int i = 0; i<4; i++)                            //looping through all remote buttons
  {
    checkremoteInput(buttonNr[i]);
  }
}
  
}

void checkremoteInput(remoteModule &buttonToCheck)
{
  buttonToCheck.currentState = digitalRead(buttonToCheck.modulePin);
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
      Serial.print("A short press is detected on input: ");                    //Short press detected
      Serial.println(buttonToCheck.id);
      buttonToCheck.shortpressFlag = true;
      flashLED(3);
    }
  }
  if(buttonToCheck.isPressing == true && buttonToCheck.isLongDetected == false)
  {
    long pressDurationA = millis() - buttonToCheck.pressedTime;
    if(pressDurationA > LONG_PRESS_MIN) 
    {
      Serial.print("A long press is detected on input: ");                     //Long press detected
      Serial.println(buttonToCheck.id);
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
