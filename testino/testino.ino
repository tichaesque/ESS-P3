
// !! Requires Arduino-MOS scheduler library
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

int numloops = 0;

// the position in the score
int ScorePosition;
int BGScorePosition;

// the length of the Melody track
int MelodyLength;
boolean playingMelody;

// checks if melody track is synced with background track
boolean MelodySynced;

// when this value is LOW, the Melody track is muted
int MelodyVal;

// the note being played in the melody track and its duration
int MelodyNote;
int MelodyNoteDuration;

int MelodyScore [93][2] = { {B_HIGH, 1000}, {E_LOW, 1000},
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


int BackgroundScore [4][2] = { {E_LOW, 500},  {B_HIGH, 500}, {E_HIGH, 500}, {B_HIGH, 500} };
//int BackgroundScore [1][2] = { {E_LOW, 2000} };

// the note being played in the background track and its duration
int BGNote;
int BGNoteDuration;
int BGlength;

void setup() {
  for (int i = 2; i <= 11; i++) {
    pinMode(i, OUTPUT);
  }

  Serial.begin(9600);        // connect to the serial port

  MelodyLength = 93;
  BGlength = 4;
  ScorePosition = 0;
  BGScorePosition = 0;
  playingMelody = false;
  MelodySynced = false;
  MelodyVal = LOW;

}

void loop () {

  MOS_Call(SerialListener);
  MOS_Call(MelodyLoop);
  MOS_Call(BackgroundLoop);

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
    MOS_WaitForCond(tcb, MelodySynced);

    if (playingMelody) {
      MelodyVal = HIGH;
    }
    else {
      MelodyVal = LOW;
    } 

    MelodyNote = MelodyScore[ScorePosition % MelodyLength][0];
    MelodyNoteDuration = MelodyScore[ScorePosition % MelodyLength][1];

    if (MelodyNote == EB_BLOCK) {
      digitalWrite(E_LOW, MelodyVal);
      digitalWrite(B_HIGH, MelodyVal);

      MOS_Delay(tcb, MelodyNoteDuration);

      digitalWrite(E_LOW, LOW);
      digitalWrite(B_HIGH, LOW);
      ScorePosition++;
    }
    else if (MelodyNote != REST) {
      digitalWrite(MelodyNote, MelodyVal);

      MOS_Delay(tcb, MelodyNoteDuration);

      digitalWrite(MelodyNote, LOW);
      ScorePosition++;
    }
    else {
      MOS_Delay(tcb, MelodyNoteDuration);
      ScorePosition++;
    }

  }


}

// the loop that continuously plays in the background
void BackgroundLoop(PTCB tcb) {
  MOS_Continue(tcb);

  while (1) {
    if (playingMelody && BGScorePosition % BGlength == 0) {
      MelodySynced = true;
    }

    BGNote = BackgroundScore[BGScorePosition % BGlength][0];
    BGNoteDuration = BackgroundScore[BGScorePosition % BGlength][1];

    digitalWrite(BGNote, HIGH);
    MOS_Delay(tcb, BGNoteDuration-50);
    digitalWrite(BGNote, LOW);
    MOS_Delay(tcb, 50);

    BGScorePosition++;
  }
}



