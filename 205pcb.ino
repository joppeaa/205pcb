#include "pinDeclarations.h"

elapsedMillis mainDelay;
elapsedMillis lockDelay;
bool timerRunning = false;

const unsigned int maindelayMax = 50;

const int LONG_PRESS_MIN  = 1500; // 1500 milliseconds

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


  pinMode(hornPin, OUTPUT);
  pinMode(mirrorheatPin, OUTPUT);
  pinMode(cabinPin, OUTPUT);
  pinMode(indicatorPin, OUTPUT);
  pinMode(headlightPin, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  pinMode(servovoltagePin, OUTPUT);
  pinMode(keyonoutputPin, OUTPUT);
  pinMode(engineonoutputPin, OUTPUT);
  
  pinMode(mainrelayPin, OUTPUT);
  pinMode(lockPin, OUTPUT);
  pinMode(unlockPin, OUTPUT);
  
  writeoutputsLOW();                                                  //Writing all outputs LOW
  
  buttonNr[0].modulePin = 14;
  buttonNr[1].modulePin = 15;
  buttonNr[2].modulePin = 16;
  buttonNr[3].modulePin = 17;
  
  buttonNr[0].id = 1;
  buttonNr[1].id = 2;
  buttonNr[2].id = 3;
  buttonNr[3].id = 4;
}

void loop() 
{
  // put your main code here, to run repeatedly:

  if (mainDelay > maindelayMax)                                         //Main high speed loop 
  {
    mainDelay = 0;                                                      //resetting maindelay timer
    togglemainRelay();
    for (int i = 0; i<4; i++)                            //looping through all remote buttons
    {
      checkremoteInput(buttonNr[i]);
    }
    checkforLock();
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

void checkforLock()
{
  Serial.println("Checking for lock");
  if (buttonNr[0].shortpressFlag == true)
  {
    Serial.println("Lockrequest detected");
    lockCar();
  }
}

void lockCar()
{
  const unsigned int lockdelayMax = 800;
  if (timerRunning == false)
  {
    timerRunning = true;
    lockDelay = 0;  
  }
  if (lockDelay < lockdelayMax)
  {
    digitalWrite(mirrorheatPin, HIGH);  
    Serial.println("Locking Car");
  }
  else
  {
    digitalWrite(mirrorheatPin, LOW);  
    buttonNr[0].shortpressFlag = false;
    timerRunning = false;
  }
  
}

void togglemainRelay()
{
  digitalWrite(mainrelayPin, HIGH);
}

void writeoutputsLOW()
{
  digitalWrite(mainrelayPin, LOW);
  digitalWrite(hornPin, LOW);
  digitalWrite(mirrorheatPin, LOW);
  digitalWrite(cabinPin, LOW);
  digitalWrite(indicatorPin, LOW);
  digitalWrite(headlightPin, LOW);
  digitalWrite(heaterPin, LOW);
  digitalWrite(servovoltagePin, LOW);
  digitalWrite(lockPin, LOW);
  digitalWrite(unlockPin, LOW);
  digitalWrite(keyonoutputPin, LOW);
  digitalWrite(engineonoutputPin, LOW);
}
