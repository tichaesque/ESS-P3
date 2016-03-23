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

int hue = 8;

// <1> Set the range of Hue values for our filter
int rangeLow;
int rangeHigh;

Goldfish goldfish;
Region xylophone;

boolean[] inRegion;

void setup() {
  video = new Capture(this, 640, 480);
  video.start();
  
  myPort = new Serial(this, Serial.list()[2], 9600); 
  
  opencv = new OpenCV(this, video.width, video.height);
  contours = new ArrayList<Contour>();
  
  rangeLow = hue-3;
  rangeHigh = hue+3;
   
  goldfish = new Goldfish(0,0);
  xylophone = new Region(0,0,width,height/2, #000000);
  
  inRegion = new boolean[3];
  
  size(1280,480);
  
  // find ports
  printArray(Serial.list());
}

void draw() {
  
  background(255);
  
  // draw regions
  xylophone.display();
  
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
    
    if(!inRegion[0] && xylophone.contains(goldfish.posX, goldfish.posY)) {
      println("HI "+millis());
      myPort.write('0');
      
      inRegion[0] = true;
      inRegion[1] = false;
      inRegion[2] = false;
    }
    else if(inRegion[0] && !xylophone.contains(goldfish.posX, goldfish.posY)) {
      println("BYE "+millis());
      myPort.write('1');
      inRegion[0] = false;
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