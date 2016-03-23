/* To use this cd into arduino-serial folder
    ./arduino-serial -b 9600 -p /dev/cu.usbmodem1411 -s THECHARACTER
*/

/* IMPORTANT: To turn on a device, a LOW signal has to be sent! NOT HIGH!! */

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <elapsedMillis.h>

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);

int ledPin = 13;

bool running = false;

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);        // connect to the serial port


  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz

  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);

  running = false;
}

void loop () {
  uint8_t i;

  if (Serial.available()) {
    int val = Serial.read();      // read the serial port

    switch (val) {
      case '0':
        if (!running) {
          /*
          myMotor->run(FORWARD);

          for (i = 0; i < 255; i++) {
            myMotor->setSpeed(i);
            delay(10);
          }*/
          digitalWrite(ledPin, HIGH);
          running = true;
        }
        break;


      case '1':
        if (running) {
          running = false;
          /*
          myMotor->run(FORWARD);
          for (i = 255; i != 0; i--) {
            myMotor->setSpeed(i);
            delay(10);
          }*/
          digitalWrite(ledPin, LOW);
          
        }
        break;
    }

  }


}
