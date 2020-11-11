/*
  RC Controller Code
  By: PVC Pipe
  Github URL: 
  Input components: 2 joysticks, 2 dial potentiometers, 4 toggle switches, 2 buttons
  Output components: nRF24L01 radio transmitter
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// radio setup
RF24 radio(?, ?); // CNS, CE
const uint64_t address = 0xE7E7E7E7E7;

// define analog inputs
#define joyRightX A0
#define joyRightY A1
#define joyLeftX  A2
#define joyLefty  A3
#define potRight  A4
#define potLeft   A5

// define digital inputs
#define toggle1     ?
#define toggle2     ?
#define toggle3     ?
#define toggle4     ?
#define buttonRight ?
#define buttonLeft  ?

void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // get the joystick values
  int rightXVal = analogRead(joyRightX);
  int rightYVal = analogRead(joyRightY);
  int leftXVal = analogRead(joyLeftX);
  int leftYVal = analogRead(joyLeftY);

  // get the dial potentiometer values
  int potRightVal = analogRead(potRight);
  int potLeftVal = analogRead(potLeft);

  // get the 4 toggle switch values
  int toggle1Val = digitalRead(toggle1);
  int toggle2Val = digitalRead(toggle2);
  int toggle3Val = digitalRead(toggle3);
  int toggle4Val = digitalRead(toggle4);

  //get the 2 button values
  int buttonRightVal = digitalRead(buttonRight);
  int buttonLeftVal = digitalRead(buttonLeft);

  // put the values into an array
  int data[] = {rightXVal, rightYVal, leftXVal, leftYVal, 
                potRightVal, potLeftVal, 
                toggle1Val, toggle2Val, toggle3Val, toggle4Val, 
                buttonRightVal, buttonLeftVal};

  // send the data to the receiver
  radio.write(&data, sizeof(data));
}
