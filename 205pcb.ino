#include "pinDeclarations.h"

elapsedMillis maindelay;
const unsigned int maindelayMax = 50;
bool checkinputsFlag = 0;


const int SHORT_PRESS_TIME = 1500; // 1000 milliseconds
const int LONG_PRESS_TIME  = 1500; // 1000 milliseconds

int currentState433A;
int lastState433A = LOW;
unsigned long pressedTime433A  = 0;
unsigned long releasedTime433A = 0;
bool isPressing433A = false;
bool isLongDetected433A = false;



bool remoteshortpressFlagA = false;
bool remotelongpressFlagA = false;
bool remoteshortpressFlagB = false;
bool remotelongpressFlagB = false;
bool remoteshortpressFlagC = false;
bool remotelongpressFlagC = false;
bool remoteshortpressFlagD = false;
bool remotelongpressFlagD = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(200);
  pinMode(13, OUTPUT);
  flashLED(10);

  
  pinMode(remotePinA, INPUT_PULLDOWN);            //433 MHz module inputs
  pinMode(remotePinB, INPUT_PULLDOWN);
  pinMode(remotePinC, INPUT_PULLDOWN);
  pinMode(remotePinD, INPUT_PULLDOWN);
//  attachInterrupt(digitalPinToInterrupt(433A), 433interrupt, HIGH);
//  attachInterrupt(digitalPinToInterrupt(433B), 433interrupt, HIGH);
//  attachInterrupt(digitalPinToInterrupt(433C), 433interrupt, HIGH);
//  attachInterrupt(digitalPinToInterrupt(433D), 433interrupt, HIGH);
  
}

void loop() {
  // put your main code here, to run repeatedly:

if (maindelay > maindelayMax)               //Main high speed loop 
{
  maindelay = 0;                            //resetting maindelay timer
  checkremoteInputs();
}
  
}

void checkremoteInputs()
{
  currentState433A = digitalRead(remotePinA);
  if(lastState433A == LOW && currentState433A == HIGH)         //button is pressed
  {
    pressedTime433A = millis();
    isPressing433A = true;
    isLongDetected433A = false;
  }
  else if(lastState433A == HIGH && currentState433A == LOW)     //button is released
  { 
    isPressing433A = false;
    releasedTime433A = millis();
    long pressDuration433A = releasedTime433A - pressedTime433A;
    if(pressDuration433A < SHORT_PRESS_TIME)
    {
      Serial.println("A short press is detected");              //Short press detected
      bool remoteshortpressFlagA = true;
      flashLED(3);
    }
  }
  if(isPressing433A == true && isLongDetected433A == false)
  {
    long pressDuration433A = millis() - pressedTime433A;
    if( pressDuration433A > LONG_PRESS_TIME ) 
    {
      Serial.println("A long press is detected");               //Long press detected
      
      isLongDetected433A = true;
      remotelongpressFlagA = true;
      flashLED(10);
    }
  }

  lastState433A = currentState433A;
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
