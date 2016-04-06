/**
 * Modified version of:
 * HSVColorTracking example
 */

import gab.opencv.*;
import processing.video.*;
import java.awt.Rectangle;
import processing.serial.*;

// The serial port:
Serial myPort; 

Capture video;
OpenCV opencv;
PImage src, colorFilteredImage;
ArrayList<Contour> contours;

// The hue to track
//int hue = 177;
int hue = 2; 

// <1> Set the range of Hue values for our filter
int rangeLow;
int rangeHigh;

Goldfish goldfish; 
Region Loop1;
Region Loop2;
Region Loop3;
Region Loop4;

boolean inLoop1;
boolean inLoop2; 
boolean inLoop3; 
boolean inLoop4; 

int startTime; 

boolean finished = true;

void setup() {
  //size(1280, 480);
  size(640, 480); 
  String[] cameras = Capture.list();
  //printArray(cameras); 
  //printArray(Serial.list()); 

  // use external webcam (performance)
  //video = new Capture(this, 640, 480, cameras[15]);

  // use built-in webcam (testing purposes only) 
  video = new Capture(this, 640, 480); 
  
  video.start();

  myPort = new Serial(this, Serial.list()[2], 9600); 

  opencv = new OpenCV(this, video.width, video.height);
  contours = new ArrayList<Contour>();

  rangeLow = hue-3;
  rangeHigh = hue+3;

  goldfish = new Goldfish(0, 0);

  // these are the different quadrants the goldfish can swim to
  // when a goldfish lands on a specific quadrant, it triggers the loop attributed
  // to that quadrant
  Loop1 = new Region(0, 0, width/2, height/2, #00FFFF);
  Loop2 = new Region(width*0.5, 0, width/2, height/2, #FF0000);
  Loop3 = new Region(0, height/2, width/2, height/2, #00FF00);
  Loop4 = new Region(width*0.5, height/2, width/2, height/2, #0000FF);

  inLoop1 = false;
  inLoop2 = false;
  inLoop3 = false;
  inLoop4 = false;

  startTime = 0;
}

void draw() {
  background(255);

  // Read last captured frame
  if (video.available()) {
    video.read();
  }

  // all the computer vision magic is here
  opencv.loadImage(video);

  opencv.useColor();
  src = opencv.getSnapshot();

  opencv.useColor(HSB); 
  opencv.setGray(opencv.getH().clone()); 
  opencv.inRange(rangeLow, rangeHigh);

  colorFilteredImage = opencv.getSnapshot();

  // <7> Find contours in our range image.
  //     Passing 'true' sorts them by descending area.
  contours = opencv.findContours(true, true);

  // Display camera output
  image(src, 0, 0); 

  // draw regions
  noStroke(); 
  Loop1.display();
  Loop2.display();
  Loop3.display();
  Loop4.display();

  if (contours.size() > 0) { 

    // Locks-on to the contour of the largest body that 
    // matches the hue we want to track, which in this case should be the fish
    Contour biggestContour = contours.get(0);
    Rectangle r = biggestContour.getBoundingBox();

    // draw bounding box
    noFill(); 
    strokeWeight(2); 
    stroke(#FFFFFF);
    rect(r.x, r.y, r.width, r.height);

    // the ellipse tracking the fish's position is 
    // in the center of the bounding box
    float ellipsePosX = r.x + r.width/2;
    float ellipsePosY = r.y + r.height/2;

    // update the goldfish's position to the ellipse's position
    goldfish.update(ellipsePosX, ellipsePosY);

    fill(#FFFFFF);
    ellipse(goldfish.posX, goldfish.posY, 30, 30); 

    if (!finished) {
      // keep track of time
      int currSec = (millis() - startTime) / 1000;
      int currMin = currSec / 60;  

      // song is 3 minutes long
      if (currMin >= 3) { 
        // stop playing
        myPort.write('5'); 

        finished = true;
      } else {

        if (!inLoop1 && Loop1.contains(goldfish.posX, goldfish.posY)) { 
          // play first loop 
          myPort.write('1'); 
          inLoop1 = true;
          inLoop2 = false;
          inLoop3 = false;
          inLoop4 = false;
        } else if (!inLoop2 && Loop2.contains(goldfish.posX, goldfish.posY)) { 
          // play second loop 
          myPort.write('2'); 
          inLoop1 = false;
          inLoop2 = true;
          inLoop3 = false;
          inLoop4 = false;
        } else if (!inLoop3 && Loop3.contains(goldfish.posX, goldfish.posY)) { 
          // play third loop 
          myPort.write('3'); 
          inLoop1 = false;
          inLoop2 = false;
          inLoop3 = true;
          inLoop4 = false;
        } else if (!inLoop4 && Loop4.contains(goldfish.posX, goldfish.posY)) { 
          // play fourth loop 
          myPort.write('4'); 
          inLoop1 = false;
          inLoop2 = false;
          inLoop3 = false;
          inLoop4 = true;
        }

        // display a timer so that the performer knows when the song finishes
        pushMatrix();
        translate(width*0.52, 50); 
        fill(255);
        textSize(60);
        text((2-currMin) + ":" + abs(60-(currSec%60)) % 60, 0, 0); 
        popMatrix();
      }
    }
  }
}

// starts toy piano when key is pressed and released
void keyReleased() {
  finished = false;  
  startTime = millis();
}

// changes tracking color
void mousePressed() {
  color c = get(mouseX, mouseY);
  println("r: " + red(c) + " g: " + green(c) + " b: " + blue(c));

  int hue = int(map(hue(c), 0, 255, 0, 180));
  println("hue to detect: " + hue);

  rangeLow = hue - 5;
  rangeHigh = hue + 5;
}