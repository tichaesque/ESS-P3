
// using the Arduino-MOS scheduler library
#include <MOS.h>
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
boolean playingMelody;

int numloops = 0;

float tempo;

int note;
int duration;

boolean flag;

int MainScore [93][2] = { {B_HIGH, 1000}, {E_LOW, 1000},
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
  playingMelody = false;

  tempo = 1.0;

  flag = false;
}

void loop () {

  MOS_Call(SerialListener);
  MOS_Call(MelodyLoop);

}

void SerialListener(PTCB tcb) {
  MOS_Continue(tcb);                    // Continue at previous suspended position

  while (1) {
    MOS_WaitForCond(tcb, Serial.available());

    int val = Serial.read();      // read the serial port
    if (val == '1') {
      playingMelody = true;
    }
    else if (val == '0') {
      playingMelody = false;
    }
    
  }
}

// the loop containing 'Dolphin Dance'
void MelodyLoop(PTCB tcb) {
  MOS_Continue(tcb);                    // Continue at previous suspended position

  while (1) {
    MOS_WaitForCond(tcb, playingMelody);

    note = MainScore[ScorePosition % ScoreLength][0];
    duration = MainScore[ScorePosition % ScoreLength][1];

    if (note == EB_BLOCK) {
      digitalWrite(E_LOW, HIGH);
      digitalWrite(B_HIGH, HIGH);

      MOS_Delay(tcb, duration);

      digitalWrite(E_LOW, LOW);
      digitalWrite(B_HIGH, LOW);

    }
    else if (note != REST) {
      digitalWrite(note, HIGH);

      MOS_Delay(tcb, duration);

      digitalWrite(note, LOW);
      ScorePosition++;
      flag = false;
    }
    else {
      MOS_Delay(tcb, duration);
      ScorePosition++;
    }

  }


}

// the loop that continuously plays in the background
/*void DefaultLoop() {

  }
*/


