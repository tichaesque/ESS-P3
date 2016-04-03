
// !! Requires Arduino-MOS scheduler library
#include <MOS.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <elapsedMillis.h>

#define D1     2
#define E1     3
#define G1     4
#define A1     5
#define B1     6
#define C2     7
#define D2     8
#define E2     9
#define G2    10
#define A2    11
#define B2    12

#define G2_B2   1012
#define REST    -1

int numloops = 0;

int *RefLoopPosition;

// # of notes in measure
int RefLoopLength;

int Loop1 [8][2] = { {E1, 500}, {B1, 500}, {E2, 500}, {B1, 500},
  {D1, 500}, {A1, 500}, {D2, 500}, {A1, 500},

};
boolean PlayingLoop1 = true;
boolean Loop1Synced = true;
int Loop1Length = 8;
int Loop1Position = 0;
int Loop1Val = HIGH;
int Loop1Note;
int Loop1Duration;

int Loop2 [5][2] = { {B1, 1000}, {E2, 950}, {REST, 50},
  {E2, 1000}, {G2_B2, 1000},

};
boolean PlayingLoop2 = false;
boolean Loop2Synced = false;
int Loop2Length = 5;
int Loop2Position = 0;
int Loop2Val = LOW;
int Loop2Note;
int Loop2Duration;

int Loop3 [24][2] = { {E1, 125}, {B1, 375}, {REST, 500}, {E2, 125}, {B1, 375}, {REST, 500},
  {D2, 125}, {A1, 375}, {REST, 500}, {A1, 125}, {D1, 375}, {REST, 500},
  {E1, 125}, {B1, 375}, {REST, 500}, {E2, 125}, {B1, 375}, {REST, 500},
  {D2, 125}, {A1, 375}, {REST, 500}, {C2, 125}, {G1, 375}, {REST, 500},

};
boolean PlayingLoop3 = false;
boolean Loop3Synced = false;
int Loop3Length = 24;
int Loop3Position = 0;
int Loop3Val = LOW;
int Loop3Note;
int Loop3Duration;

int Loop4 [8][2] = { {E1, 500}, {G2_B2, 750}, {E1, 250}, {D2, 500},
  {E1, 500}, {G2_B2, 750}, {D1, 250}, {A2, 500},

};
boolean PlayingLoop4 = false;
boolean Loop4Synced = false;
int Loop4Length = 8;
int Loop4Position = 0;
int Loop4Val = LOW;
int Loop4Note;
int Loop4Duration;

void setup() {
  for (int i = 2; i <= 12; i++) {
    pinMode(i, OUTPUT);
  }

  Serial.begin(9600);        // connect to the serial port

  RefLoopPosition = (int *) malloc(sizeof(int));

  RefLoopPosition = &Loop1Position;
  RefLoopLength = 8;
}

void loop () {

  MOS_Call(SerialListener);
  //MOS_Call(UpdateRefLoop);

  MOS_Call(PlayLoop1);
  MOS_Call(PlayLoop2);
  MOS_Call(PlayLoop3);
  MOS_Call(PlayLoop4);

}

void SerialListener(PTCB tcb) {
  MOS_Continue(tcb);                    // Continue at previous suspended position

  while (1) {
    MOS_WaitForCond(tcb, Serial.available());

    int val = Serial.read();      // read the serial port

    switch (val) {
      case '1':
        PlayingLoop1 = true;
        PlayingLoop2 = false;
        PlayingLoop3 = false;
        PlayingLoop4 = false;

        RefLoopPosition = &Loop1Position;
        RefLoopLength = Loop1Length;
        break;

      case '2':
        PlayingLoop1 = false;
        PlayingLoop2 = true;
        PlayingLoop3 = false;
        PlayingLoop4 = false;

        RefLoopPosition = &Loop2Position;
        RefLoopLength = Loop2Length;
        break;

      case '3':
        PlayingLoop1 = false;
        PlayingLoop2 = false;
        PlayingLoop3 = true;
        PlayingLoop4 = false;

        RefLoopPosition = &Loop3Position;
        RefLoopLength = Loop3Length;
        break;

      case '4':
        PlayingLoop1 = false;
        PlayingLoop2 = false;
        PlayingLoop3 = false;
        PlayingLoop4 = true;

        RefLoopPosition = &Loop4Position;
        RefLoopLength = Loop4Length;
        break;
    }

  }
}

void PlayLoop2(PTCB tcb) {
  MOS_Continue(tcb);                    // Continue at previous suspended position

  while (1) {
    MOS_WaitForCond(tcb, Loop2Synced);

    /*
        if (PlayingLoop2) {
          Loop2Val = HIGH;
        }
        else {
          Loop2Val = LOW;
        }
    */
    Loop2Note = Loop2[Loop2Position % Loop2Length][0];
    Loop2Duration = Loop2[Loop2Position % Loop2Length][1];

    if (Loop2Note == G2_B2) {
      if (PlayingLoop2) {
        digitalWrite(G2, HIGH);
        digitalWrite(B2, HIGH);
        MOS_Delay(tcb, Loop2Duration);

        digitalWrite(G2, LOW);
        digitalWrite(B2, LOW);
      }
      else {
        MOS_Delay(tcb, Loop2Duration);
      }
      //digitalWrite(G2, Loop2Val);
      //digitalWrite(B2, Loop2Val);


      Loop2Position++;
    }
    else if (Loop2Note != REST) {
      if (PlayingLoop2) {
        digitalWrite(Loop2Note, HIGH);
        MOS_Delay(tcb, Loop2Duration);

        digitalWrite(Loop2Note, LOW);
      }
      else {
        MOS_Delay(tcb, Loop2Duration);
      }
      //digitalWrite(Loop2Note, Loop2Val);


      Loop2Position++;
    }
    else {
      MOS_Delay(tcb, Loop2Duration);
      Loop2Position++;
    }

  }
}

void PlayLoop3(PTCB tcb) {
  MOS_Continue(tcb);                    // Continue at previous suspended position

  while (1) {
    MOS_WaitForCond(tcb, Loop3Synced);

    Loop3Note = Loop3[Loop3Position % Loop3Length][0];
    Loop3Duration = Loop3[Loop3Position % Loop3Length][1];

    if (Loop3Note != REST) {
      if (PlayingLoop3) {
        digitalWrite(Loop3Note, HIGH);
        MOS_Delay(tcb, Loop3Duration);

        digitalWrite(Loop3Note, LOW);
      }
      else {
        MOS_Delay(tcb, Loop3Duration);
      }
      //digitalWrite(Loop3Note, Loop3Val);


      Loop3Position++;
    }
    else {
      MOS_Delay(tcb, Loop3Duration);
      Loop3Position++;
    }

  }
}

void PlayLoop4(PTCB tcb) {
  MOS_Continue(tcb);                    // Continue at previous suspended position

  while (1) {
    MOS_WaitForCond(tcb, Loop4Synced);

    /*
        if (PlayingLoop4) {
          Loop4Val = HIGH;
        }
        else {
          Loop4Val = LOW;
        }*/

    Loop4Note = Loop4[Loop4Position % Loop4Length][0];
    Loop4Duration = Loop4[Loop4Position % Loop4Length][1];

    if (Loop4Note == G2_B2) {
      if (PlayingLoop4) {
        digitalWrite(G2, HIGH);
        digitalWrite(B2, HIGH);

        MOS_Delay(tcb, Loop4Duration);

        digitalWrite(G2, LOW);
        digitalWrite(B2, LOW);
      }
      else {
        MOS_Delay(tcb, Loop4Duration);
      }
      //digitalWrite(G2, Loop4Val);
      //digitalWrite(B2, Loop4Val);



      Loop4Position++;
    }
    else if (Loop4Note != REST) {
      if (PlayingLoop4) {
        digitalWrite(Loop4Note, HIGH);
        MOS_Delay(tcb, Loop4Duration);

        digitalWrite(Loop4Note, LOW);
      }
      else {
        MOS_Delay(tcb, Loop4Duration);
      }
      //digitalWrite(Loop4Note, Loop4Val);


      Loop4Position++;
    }
    else {
      MOS_Delay(tcb, Loop4Duration);
      Loop4Position++;
    }

  }
}

// Special loop that also serves as a metronome
void PlayLoop1(PTCB tcb) {
  MOS_Continue(tcb);

  while (1) {

    if (PlayingLoop2 && Loop1Position % 4 == 0) {
      Loop2Synced = true;
    }
    if (PlayingLoop3 && Loop1Position % 4 == 0) {
      Loop3Synced = true;
    }
    if (PlayingLoop4 && Loop1Position % 4 == 0) {
      Loop4Synced = true;
    }

    Loop1Note = Loop1[Loop1Position % Loop1Length][0];
    Loop1Duration = Loop1[Loop1Position % Loop1Length][1];

    if (PlayingLoop1) {
      digitalWrite(Loop1Note, HIGH);
      MOS_Delay(tcb, Loop1Duration);
      digitalWrite(Loop1Note, LOW);
    }
    else {
      MOS_Delay(tcb, Loop1Duration);
    }
    //digitalWrite(Loop1Note, Loop1Val);
    //MOS_Delay(tcb, Loop1Duration);
    //digitalWrite(Loop1Note, LOW);

    Loop1Position++;
  }
}




