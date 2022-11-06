#include <Arduino.h>
#include <NewPing.h>
#include <pins.h>

#define max_ping_distance 20  // maximum ping distance is 20 cm
#define keep_out_distance 8  // an obstacle is noticed if the robot comes this close to an obstacle


// create ultrasonic objects
NewPing left_sonar(LEFT_TRIG, LEFT_ECHO, max_ping_distance);
NewPing front_sonar(FRONT_TRIG, FRONT_ECHO, max_ping_distance);
NewPing right_sonar(RIGHT_TRIG, RIGHT_ECHO, max_ping_distance);

int my_delay = 10;

// speed constants
int speed = 60;
int right_spd = 70;
int turn_speed = 50;

// motor movement function prototypes
void forward(int, int);
void left(int, int);
void right(int, int);
void reverse(int, int);
void stop();

// ==============================Motor functions ====================================
void forward(int r_speed, int l_speed){

  // turn ON all motors  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  

  // left motor on 
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(ena, l_speed);
  analogWrite(enb, r_speed);
}

void left(int r_speed, int l_speed){
  // turn motors left - right motor on, left motor reversed direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);  

  analogWrite(ena, turn_speed);
  analogWrite(enb, r_speed);
}

void right(int r_speed, int l_speed){
  // turn motors right
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);  

  analogWrite(ena, l_speed);
  analogWrite(enb, turn_speed);
}

void stop(){
  // turn motors OFF
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  
}

void initialize_motors(){
  /*
  Initialize the motors
  */

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void setup() {
  // set up motor driver pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  // init motors
  initialize_motors();
  delayMicroseconds(2000);

  Serial.begin(9600);

}

void loop() {

  // debug
  Serial.print("Left: "); Serial.print(left_sonar.ping_cm()); Serial.print("\t");
  Serial.print("Front: "); Serial.print(front_sonar.ping_cm()); Serial.print("\t");
  Serial.print("Right: "); Serial.print(right_sonar.ping_cm()); Serial.print("\t");
  Serial.println();
  
  if(front_sonar.ping_cm() > keep_out_distance){ // no obstacle infront, keep moving forward
    forward(speed, speed);
  }

  if ((front_sonar.ping_cm() < keep_out_distance) && (right_sonar.ping_cm() > keep_out_distance) && (left_sonar.ping_cm() > keep_out_distance)){
    // obstacle infront, no obstacles to the left or right, go left (or right)
    left(speed, speed);
  }

  if((left_sonar.ping_cm() < keep_out_distance) && (right_sonar.ping_cm() > keep_out_distance)){ 
    // obstacle to the left, no obstacle to the right, go right
    right(speed, speed);
  }

  if((right_sonar.ping_cm() < keep_out_distance) && (left_sonar.ping_cm() > keep_out_distance)){
    // obstacle to the right, no obstacle to the left, go left
    left(speed, speed);
  }

  if((right_sonar.ping_cm() < keep_out_distance) && (left_sonar.ping_cm() < keep_out_distance) && (front_sonar.ping_cm() > keep_out_distance)){
    // obstacle to the right, obstacle to the left, no obstacle infront, move forward
    forward(speed, speed);
  }

  if((right_sonar.ping_cm() < keep_out_distance) && (left_sonar.ping_cm() > keep_out_distance) &&(front_sonar.ping_cm() < keep_out_distance)){
    // obstacles all sides, stop
    stop();
  }
}