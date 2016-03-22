import processing.serial.*;

// The serial port:
Serial myPort; 

void setup() {
  size(500, 500);

  myPort = new Serial(this, Serial.list()[2], 9600); 

  // don't know why this needs to be here, but it does or else
  // the first time the mouse is pressed and released, it won't be recognized by the Arduino
  myPort.write('0');
  myPort.write('1');
}

void draw() {
} 

void mousePressed () {
  myPort.write('0');
}

void mouseReleased () {
  myPort.write('1');
}