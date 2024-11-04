// Sourced and modified from https://maker.pro/arduino/projects/how-to-control-an-arduino-robot-with-a-ps4-bluetooth-controller

#include <PS4BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statement in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>
// Motor A connections
int enA = 10;
int in1 = 2;
int in2 = 4;
// Motor B connections
int enB = 11;
int in3 = 8;
int in4 = 7;
// Motor B connections
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
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors initially
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

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

  // Debugging
  // PS4.getAnalogHat: Return the analog value in the range of 0-255. ??? at rest
  Serial.print("Right hat: ");
  Serial.print(abs(PS4.getAnalogHat(RightHatY) - 127));
  Serial.print(", Left hat: ");
  Serial.println(abs(PS4.getAnalogHat(LeftHatY) - 127));

  if (PS4.connected()) {
    if (PS4.getAnalogHat(RightHatY) > 235) {
      digitalWrite(in2, LOW);
      digitalWrite(in1, HIGH);
    } else if (PS4.getAnalogHat(RightHatY) < 20) {
      digitalWrite(in2, HIGH);
      digitalWrite(in1, LOW);
    } else {
      analogWrite(enA, 0);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
    }
    if (PS4.getAnalogHat(LeftHatY) > 235) {
      digitalWrite(in4, LOW);
      digitalWrite(in3, HIGH);
    } else if (PS4.getAnalogHat(LeftHatY) < 20) {
      digitalWrite(in4, HIGH);
      digitalWrite(in3, LOW);
    } else {
      analogWrite(enB, 0);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
    }
  }
}