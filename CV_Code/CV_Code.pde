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

int hue = 177;

// <1> Set the range of Hue values for our filter
int rangeLow;
int rangeHigh;

Goldfish goldfish;
Region toyPiano;

boolean inPiano; 

void setup() {
  size(1280, 480); 
  String[] cameras = Capture.list();

  // use external webcam
  video = new Capture(this, 640, 480, cameras[0]);

  // use built-in webcam (testing purposes only) 
  //video = new Capture(this, 640, 480); 
  video.start();

  myPort = new Serial(this, Serial.list()[2], 9600); 

  opencv = new OpenCV(this, video.width, video.height);
  contours = new ArrayList<Contour>();

  rangeLow = hue-3;
  rangeHigh = hue+3;

  goldfish = new Goldfish(0, 0);
  //toyPiano = new Region(width/2, 0, width/4, height, #000000);
  toyPiano = new Region(0, 0, width, height/2, #000000);

  inPiano = false;
}

void draw() {

  background(255);

  // draw regions
  toyPiano.display();

  // Read last captured frame
  if (video.available()) {
    video.read();
  }

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

  // Display background images
  image(src, 0, 0); 

  if (contours.size() > 0) { 
    Contour biggestContour = contours.get(0);
    Rectangle r = biggestContour.getBoundingBox();

    noFill(); 
    strokeWeight(2); 
    stroke(0, 255, 0);
    rect(r.x, r.y, r.width, r.height);

    float ellipsePosX = r.x + r.width/2;
    float ellipsePosY = r.y + r.height/2;

    noStroke(); 
    fill(0, 255, 0);
    ellipse(ellipsePosX, ellipsePosY, 30, 30); 

    // TRACK GOLDFISH
    goldfish.update(ellipsePosX + src.width, ellipsePosY);
    fill(255, 0, 0);
    ellipse(goldfish.posX, goldfish.posY, 30, 30); 

    if (!inPiano && toyPiano.contains(goldfish.posX, goldfish.posY)) { 
      // play toy piano
      myPort.write('1'); 
      inPiano = true;
    } else if (inPiano && !toyPiano.contains(goldfish.posX, goldfish.posY)) { 
      // play drums
      myPort.write('0'); 
      inPiano = false;
    }
  }
}

void mousePressed() {

  color c = get(mouseX, mouseY);
  println("r: " + red(c) + " g: " + green(c) + " b: " + blue(c));

  int hue = int(map(hue(c), 0, 255, 0, 180));
  println("hue to detect: " + hue);

  rangeLow = hue - 5;
  rangeHigh = hue + 5;
}