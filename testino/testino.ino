/* To use this cd into arduino-serial folder
    ./arduino-serial -b 9600 -p /dev/cu.usbmodem1411 -s THECHARACTER
*/

/* IMPORTANT: To turn on a device, a LOW signal has to be sent! NOT HIGH!! */

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <elapsedMillis.h>

#define D_FLAT     2
#define E_FLAT     3
#define E_LOW      4
#define F_LOW      5
#define G_LOW      6
#define A_FLAT     7
#define A_LOW      8
#define B_FLAT     9
#define B_HIGH    10
#define E_HIGH    11

#define EB_BLOCK   0
#define REST      -1

int ScoreLength;
int ScorePosition;
boolean playingPiano;

int Score [93][2] = { {B_HIGH, 1000}, {E_LOW, 1000},
  {A_LOW, 1000}, {REST, 500},  {B_HIGH, 125}, {E_HIGH, 375},
  {B_HIGH, 125}, {E_LOW, 250},  {REST, 125}, {B_HIGH, 1000}, {E_LOW, 1000},
  {A_LOW, 1000}, {REST, 500},
  {REST, 2000},
  {B_HIGH, 1000}, {E_LOW, 1000},
  {A_LOW, 1000}, {REST, 500},  {B_HIGH, 125}, {E_HIGH, 375},
  {B_HIGH, 125}, {E_LOW, 250},  {REST, 125}, {B_HIGH, 1000}, {E_LOW, 1000},
  {A_LOW, 1000}, {REST, 500},
  {REST, 250}, {B_FLAT, 250},  {A_FLAT, 500}, {G_LOW, 1000},
  {G_LOW, 375}, {A_FLAT, 125},  {B_FLAT, 250}, {REST, 250}, {F_LOW, 2000},
  {REST, 1000},
  {E_FLAT, 375}, {F_LOW, 125},  {G_LOW, 250}, {REST, 250}, {D_FLAT, 2000},
  {REST, 500}, {B_HIGH, 1000},
  {E_LOW, 1000}, {A_LOW, 1000},
  {REST, 500}, {B_HIGH, 1000},
  {E_LOW, 1000}, {A_LOW, 1000},
  {A_FLAT, 1000}, {G_LOW, 1000},
  {G_LOW, 375}, {A_FLAT, 125},  {B_FLAT, 250}, {REST, 250}, {F_LOW, 2000},
  {REST, 1000},
  {E_FLAT, 375}, {F_LOW, 125},  {G_LOW, 250}, {REST, 250}, {D_FLAT, 2000},
  {REST, 1000},
  {EB_BLOCK, 1000}, {EB_BLOCK, 1000},
  {A_LOW, 1000}, {REST, 500},  {B_HIGH, 125}, {E_HIGH, 375},
  {B_HIGH, 125}, {E_LOW, 375}, {REST, 500}, {EB_BLOCK, 1000},
  {EB_BLOCK, 1000}, {A_LOW, 2000},
  {REST, 1000},
  {REST, 250}, {B_FLAT, 250},  {A_FLAT, 500}, {G_LOW, 1000},
  {G_LOW, 375}, {A_FLAT, 125},  {B_FLAT, 250}, {REST, 250}, {F_LOW, 2000},
  {REST, 1000},
  {E_FLAT, 375}, {F_LOW, 125},  {G_LOW, 250}, {REST, 250}, {D_FLAT, 2000},
  {E_FLAT, 500}
};

void setup() {
  for (int i = 2; i <= 11; i++) {
    pinMode(i, OUTPUT);
  }

  Serial.begin(9600);        // connect to the serial port

  ScoreLength = 93;
  ScorePosition = 0;
  playingPiano = false;
}

void loop () {
  if (Serial.available()) {
    int val = Serial.read();      // read the serial port
    if (val == '1') {
      playingPiano = true;
    }
    else if (val == '0') {
      playingPiano = false;
    }
  }
  
  if (ScorePosition < ScoreLength) {
    // note: sending HIGH to the solenoid pulls it down, sending LOW pulls up
    if (!playingPiano) {
      // play drum
    }

    else if (playingPiano) {
      int note = Score[ScorePosition][0];
      int duration = Score[ScorePosition][1];

      if (note == REST) {
        delay(duration);
      }
      else if (note == EB_BLOCK) {
        digitalWrite(E_LOW, HIGH);
        digitalWrite(B_HIGH, HIGH);

        delay(duration);

        digitalWrite(E_LOW, LOW);
        digitalWrite(B_HIGH, LOW);

      }
      else {
        digitalWrite(note, HIGH);
        delay(duration);
        digitalWrite(note, LOW);
      }

      ScorePosition++;
    }
  }


}
