/*
  RC Controller Code
  By: PVC Pipe
  Github URL: https://github.com/PVC-Pipe/RC-Controller
  Input components: 2 joysticks, 2 dial potentiometers, 4 toggle switches, 2 buttons
  Output components: nRF24L01 radio transmitter
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// radio setup
// MOSI: 11, MISO: 12, SCK: 13
RF24 radio(2, 3); // CSN, CE
const uint64_t address = 0xE7E7E7E7E7;

// LCD setup
#define OLED_WIDTH 128
#define OLED_HEIGHT 32
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

// define analog inputs
#define joyLeftX  A0
#define joyLeftY  A1
#define joyRightX A2
#define joyRightY A3
// display SDA: A4
// display SCL: A5
#define potLeft   A6
#define potRight  A7

// define digital inputs
// radio CSN:       2
// radio CE:        3
#define toggle1     4
#define toggle2     5
#define toggle3     6
#define toggle4     7
#define buttonLeft  8
#define buttonRight 9
#define buttonCalibrate 10
// radio MOSI:      11
// radio MISO:      12
// radio SCK:       13

// ints to store the min and max values of the joysticks
int leftXMin = 0;
int leftXMax = 0;
int leftYMin = 0;
int leftYMax = 0;
int rightXMin = 0;
int rightXMax = 0;
int rightYMin = 0;
int rightYMax = 0;

void setup() {
  // calibrate the joysticks on startup
  calibrate();
  // setup the radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // if calibration button is pressed, calibrate the joysticks
  if (digitalRead(buttonCalibrate)) {
    calibrate();
  }
  
  // get the joystick values
  byte leftXVal = map(analogRead(joyLeftX), leftXMin, leftXMax, 0, 255);
  byte leftYVal = map(analogRead(joyLeftY), leftYMin, leftYMax, 0, 255);
  byte rightXVal = map(analogRead(joyRightX), rightXMin, rightXMax, 0, 255);
  byte rightYVal = map(analogRead(joyRightY), rightYMin, rightYMax, 0, 255);

  // get the dial potentiometer values
  byte potLeftVal = map(analogRead(potLeft), 0, 1023, 0, 255);
  byte potRightVal = map(analogRead(potRight), 0, 1023, 0, 255);

  // get the 4 toggle switch values
  byte toggle1Val = digitalRead(toggle1);
  byte toggle2Val = digitalRead(toggle2);
  byte toggle3Val = digitalRead(toggle3);
  byte toggle4Val = digitalRead(toggle4);

  //get the 2 button values
  byte buttonLeftVal = digitalRead(buttonLeft);
  byte buttonRightVal = digitalRead(buttonRight);

  // put the values into an array
  byte data[] = {leftXVal, leftYVal, 
                rightXVal, rightYVal, 
                potLeftVal, potRightVal, 
                toggle1Val, toggle2Val, toggle3Val, toggle4Val, 
                buttonLeftVal, buttonRightVal};

  // send the data to the receiver
  radio.write(&data, sizeof(data));
}

// calibrates the minimum and maximum values of the joysticks using global variables
void calibrate() {
  // calibrate left joystick Y max
  display.print("Push the left joystick up");
  delay(2000);
  leftYMax = analogRead(joyLeftY);

  // calibrate left joystick X max
  display.print("Push the left joystick right");
  delay(2000);
  leftXMax = analogRead(joyLeftX);

  // calibrate left joystick Y min
  display.print("Push the left joystick down");
  delay(2000);
  leftYMin = analogRead(joyLeftY);

  // calibrate left joystick X min
  display.print("Push the left joystick left");
  delay(2000);
  leftXMin = analogRead(joyLeftX);

  // calibrate right joystick Y max
  display.print("Push the right joystick up");
  delay(2000);
  rightYMax = analogRead(joyRightY);

  // calibrate right joystick X max
  display.print("Push the right joystick right");
  delay(2000);
  rightXMax = analogRead(joyRightX);

  // calibrate right joystick Y min
  display.print("Push the right joystick down");
  delay(2000);
  rightYMin = analogRead(joyRightY);

  // calibrate right joystick X min
  display.print("Push the right joystick left");
  delay(2000);
  rightXMin = analogRead(joyRightX);
}
