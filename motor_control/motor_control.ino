// Sourced and modified from https://maker.pro/arduino/projects/how-to-control-an-arduino-robot-with-a-ps4-bluetooth-controller

#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statement in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
// right motor connections
uint8_t enA = 3;
int in1 = 2;
int in2 = 4;
// left motor connections
uint8_t enB = 5;
int in3 = 8;
int in4 = 7;
// scooper motor connections
int in5 = 12;
int in6 = 13;

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside
BTD Btd(&Usb);  // You have to create the Bluetooth Dongle instance like so

/* You can create the instance of the PS4BT class in two ways */
// This will start an inquiry and then pair with the PS4 controller - you only have to do this once
// You will need to hold down the PS and Share button at the same time, the PS4 controller will then start to blink rapidly indicating that it is in pairing mode
PS4BT PS4(&Btd, PAIR);

// After that you can simply create the instance like so and then press the PS button on the device
//PS4BT PS4(&Btd);

bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  // set pin outputs
  pinMode(in1, OUTPUT); // right motor
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in3, OUTPUT); // left motor
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in5, OUTPUT); // scooper motor
  pinMode(in6, OUTPUT);

  // Turn off motors initially
  digitalWrite(in1, LOW); // right motor
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  digitalWrite(in3, LOW); // left motor
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
  digitalWrite(in5, LOW); // scooper motor
  digitalWrite(in6, LOW);

  // set baud rate
  Serial.begin(115200);

#if !defined(__MIPSEL__)
  while (!Serial)
    ;  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  // throw error if serial port does not connect
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  // Halt
  }
  // confirm start up
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
}


void loop() {
  Usb.Task();

  if (PS4.connected()) {

    // convert hat Y pos to motor speed
    // hat Y [0, 255] to triangle function: f(x) = abs(x - 127.5), 0 <= f(x) <= 255
    int rightHatYOffset = abs(PS4.getAnalogHat(RightHatY) - 127.5) * 2;
    // constrain between [0, 255]
    if (rightHatYOffset < 0) {
      rightHatYOffset = 0;
    } else if (rightHatYOffset > 255) {
      rightHatYOffset = 255;
    }
    int leftHatYOffset = abs(PS4.getAnalogHat(LeftHatY) - 127.5) * 2;
    // constrain between [0, 255]
    if (leftHatYOffset < 0) {
      leftHatYOffset = 0;
    } else if (leftHatYOffset > 255) {
      leftHatYOffset = 255;
    }

  } else if 
  
    // right motor
    if (PS4.getAnalogHat(RightHatY) > 137) {
      digitalWrite(in2, LOW);
      digitalWrite(in1, HIGH);
      analogWrite(enA, rightHatYOffset);
    } else if (PS4.getAnalogHat(RightHatY) < 117) {
      digitalWrite(in2, HIGH);
      digitalWrite(in1, LOW);
      analogWrite(enA, rightHatYOffset);
    } else {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      analogWrite(enA, 0);
    }
    // left motor
    if (PS4.getAnalogHat(LeftHatY) > 137) {
      digitalWrite(in4, LOW);
      digitalWrite(in3, HIGH);
      analogWrite(enB, leftHatYOffset);
    } else if (PS4.getAnalogHat(LeftHatY) < 117) {
      digitalWrite(in4, HIGH);
      digitalWrite(in3, LOW);
      analogWrite(enB, leftHatYOffset);
    } else {
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      analogWrite(enB, 0);
    }
    // scooper motor
    if (PS4.getButtonPress(UP)) {  
      digitalWrite(in5, LOW);
      digitalWrite(in6, HIGH);
    } else if (PS4.getButtonPress(DOWN)) {
      digitalWrite(in5, HIGH);
      digitalWrite(in6, LOW);
    } else {
      digitalWrite(in5, LOW);
      digitalWrite(in6, LOW);
    }

    // Debug I/O
    Serial.print("Right hat: ");
    Serial.print(abs(PS4.getAnalogHat(RightHatY)));
    Serial.print(", Left hat: ");
    Serial.print(abs(PS4.getAnalogHat(LeftHatY)));
    Serial.print(", rightHatYOffset: ");
    Serial.print(rightHatYOffset);
    Serial.print(", leftHatYOffset: ");
    Serial.print(leftHatYOffset);
    Serial.print(", pinA: ");
    Serial.print(analogRead(enA));
    Serial.print(", pinB: ");
    Serial.println(analogRead(enB));
  }
}