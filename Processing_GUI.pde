import processing.net.*;
import controlP5.*;
Client arduino;
ControlP5 gui;
Button start_button;
Button stop_button;
Button textoutput;
PImage background;
char arduino_message;

void setup(){
  fullScreen();
  //size(1280, 720);
  arduino = new Client(this, "192.168.129.30", 5200);
  background = loadImage("Untitled design.png");
  background.resize(width, height);
  noStroke();
  gui = new ControlP5(this);
  
  start_button = gui.addButton("Start") //Sets the label of the button
    .setCaptionLabel("Start")//Sets the visible label of the button
    .setColorBackground(color(255, 255, 255, 1))
    .setColorForeground(color(255, 255, 255, 1))
    .setLabelVisible(true)
    .setSize(547, 169)
    .setPosition(108, 108)
    .setFont(createFont("Courier New Bold", 100)); 
    
  stop_button = gui.addButton("Stop") //Sets the label of the button
    .setCaptionLabel("Stop")//Sets the visible label of the button
    .setColorBackground(color(255, 255, 255, 1))//Sets the normal colour of the button
    .setColorForeground(color(255, 255, 255, 1))//Sets the colour when mouse is hovering over the button 
    .setLabelVisible(true)
    .setSize(547, 169)
    .setPosition(1238, 108)
    .setFont(createFont("Courier New Bold", 100)); //Show the visible label

  textoutput = gui.addButton("Output")
    .setCaptionLabel("Nothing to Report Yet")
    .setPosition(480, 687)
    .setSize(960, 180)
    .setFont(createFont("Courier New Bold", 80))
    .setColorBackground(color(255, 255, 255, 1));

}

void draw(){
  background(background);
  if(arduino.available()>0){
    arduino_message = arduino.readChar();
  }
  
  if (arduino_message == 's'){
    textoutput.setCaptionLabel("Stop Button Pressed");
  }else if (arduino_message == 'o'){
    textoutput.setCaptionLabel("Object Detected");
  }else if (arduino_message == 'f') {
    textoutput.setCaptionLabel("Moving Forward");
  }
  
  //println(arduino_message);
  arduino.clear();
}

void Start(){
  /*This function is the same as the label of start_button.
  So when the start button is pressed the event handler automatically
  calls this function. 
  */
  arduino.write('g');
  //println("Start");
  
}

void Stop(){
  /*This function is the same as the label of start_button.
  So when the start button is pressed the event handler automatically
  calls this function. 
  */
  arduino.write('s');
  //println("Stop");
  
}
