#include "NewPing.h"
#include "WiFiNINA.h"
#include "SPI.h"

int LEFT_ENABLE = 2;
int LEFT_FORWARDS = 3;
int LEFT_BACKWARDS = 4;
int RIGHT_ENABLE = 5;


int RIGHT_FORWARDS = 6;
int RIGHT_BACKWARDS = 7;
int LEFT_SENSOR = 9;
int RIGHT_SENSOR = 8;
bool TURNING_FLAG = false;

#define ECHO_PIN 10
#define TRIGGER_PIN 11
#define MAX_DISTANCE 1290
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;
unsigned int start = 0;

unsigned int serverdelay = 0;

//char SSID[] = "VM66ED111";
//char pass[] = "6pdExjQrk7nh";

char SSID[] = "Aaron's";
char pass[] = "12345678";
WiFiServer server(5200);
char command = 's';


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  for(int i = 2; i<= 7; i++){
    pinMode(i, OUTPUT);
  }

  pinMode(RIGHT_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);
  
  start = millis();
  serverdelay = millis();

  IPAddress ip(192,168,129,30);  
  WiFi.config(ip);
  
  WiFi.begin(SSID, pass);
  delay(5000);
  server.begin();
  distance = 9;
  //IPAddress ip = WiFi.localIP();
  Serial.print("IP Address = ");
  Serial.println(ip);
  
}

void loop() {
  // Client Stuff

  WiFiClient client = server.available();

  if (client.connected()) {
    Serial.println("Client Connected");
    if (client.available()) {
    command = client.read();
    Serial.println(command);
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    //client.stop();
    //Serial.println("client disconnected");

  }

  if(((millis()-start) % 200)){
    measureDistance(true);
  }                                                                                                                    

  driving();
}

void driving(){
  bool LEFT_READ = digitalRead(LEFT_SENSOR);
  bool RIGHT_READ = digitalRead(RIGHT_SENSOR);


  if(distance < 14.00 && (distance != 0) || (command == 's')){
    stop();

    if((millis()-serverdelay) % 1000 == 0){
      if((command == 's')){
        server.write("s");
      }else{
        server.write("o");
      }
      serverdelay = millis();
    }

  }else if(LEFT_READ == 1 && RIGHT_READ == 0 ){
    move_diff_steering(10, 0, 80, 200);
    TURNING_FLAG = true;
  }else if(LEFT_READ == 0 && RIGHT_READ == 1){
    move_diff_steering(10,0, 200, 80);
    TURNING_FLAG = true;
  }else{
    if(TURNING_FLAG){
      for(int i = 130; i <=140; i++){
        move_forward(5, 0, 255);
      }
      TURNING_FLAG = false;
    }
    move_forward(100, 0, 255);
    server.write("f");

  }


}

void measureDistance(bool silence){
  distance = sonar.ping_cm();
	
	// Send results to Serial Monitor
  if(!silence){
	  Serial.print("Distance = ");
	  Serial.print(distance);
	  Serial.println(" cm");
  }

  start = millis();
}

void stop() {
  digitalWrite(LEFT_ENABLE, LOW);
  digitalWrite(RIGHT_ENABLE, LOW);
}


void move_forward(int run_time, int stop_time, int speed) {
  analogWrite(LEFT_ENABLE, speed);
  analogWrite(RIGHT_ENABLE, speed);
  digitalWrite(LEFT_FORWARDS, HIGH);
  digitalWrite(RIGHT_FORWARDS, HIGH);
  delay(run_time);
  digitalWrite(LEFT_FORWARDS, LOW);
  digitalWrite(RIGHT_FORWARDS, LOW);
  delay(stop_time);
}

void move_backward(int run_time, int stop_time, int speed) {
  analogWrite(LEFT_ENABLE, speed);
  analogWrite(RIGHT_ENABLE, speed);
  digitalWrite(LEFT_BACKWARDS, HIGH);
  digitalWrite(RIGHT_BACKWARDS, HIGH);
  delay(run_time);
  digitalWrite(LEFT_BACKWARDS, LOW);
  digitalWrite(RIGHT_BACKWARDS, LOW);
  delay(stop_time);
}

void move_left(int run_time, int stop_time, int speed) {
  analogWrite(LEFT_ENABLE, speed);
  analogWrite(RIGHT_ENABLE, speed);
  digitalWrite(LEFT_BACKWARDS, HIGH);
  digitalWrite(RIGHT_FORWARDS, HIGH);
  delay(run_time);
  digitalWrite(LEFT_BACKWARDS, LOW);
  digitalWrite(RIGHT_FORWARDS, LOW);
  delay(stop_time);
}

void move_right(int run_time, int stop_time, int speed) {
  analogWrite(LEFT_ENABLE, speed);
  analogWrite(RIGHT_ENABLE, speed);
  digitalWrite(LEFT_FORWARDS, HIGH);
  digitalWrite(RIGHT_BACKWARDS, HIGH);
  delay(run_time);
  digitalWrite(LEFT_FORWARDS, LOW);
  digitalWrite(RIGHT_BACKWARDS, LOW);
  delay(stop_time);
}

void move_diff_steering(int run_time, int stop_time, int speed_left, int speed_right) {
  analogWrite(LEFT_ENABLE, speed_left);
  analogWrite(RIGHT_ENABLE, speed_right);
  digitalWrite(LEFT_FORWARDS, HIGH);
  digitalWrite(RIGHT_FORWARDS, HIGH);
  delay(run_time);
  digitalWrite(LEFT_FORWARDS, LOW);
  digitalWrite(RIGHT_FORWARDS, LOW);
  delay(stop_time);
}



