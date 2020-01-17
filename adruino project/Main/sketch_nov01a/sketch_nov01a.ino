/*  Autonomous RC Car
 *  Code written by: Kyle Naumann - last edited 15 May 2018
 *  
 *  A few things about this code:
 *  - You may notice that there are if/else statements stacked within each other. I did this to "double check" readings and prevent random turns due to odd readings.
 *  - I did my best to organize the variables below. The steering servo positions and sonar thresholds can be changed. The thresholds are in centimeters.
 *  - Pseudocode has been incldued to explain what some of the code does.
 *  - I have disabled debugging. This can be re-enabled by going to Edit > Find > Search for "//Serial" and replace with "Serial". Be sure to un-check "Ignore Case"
 *  
 *  Feel free to modify any parts of this program.
 */

#include <Servo.h>

const int echo = 7;     //Pins for readFront() - Sonar on front
const int trig = 6;
const int echo2 = 5;    //Pins for readLeft() - Left side
const int trig2 = 4;
const int echo3 = 3;    //Pins for readRight() - Right side
const int trig3 = 2;
const int echo4 = 11;   //Pins for readFrontLeft() - Front left side
const int trig4 = 10;
const int echo5 = 8;    //Pins for readFrontRight() - Front right side
const int trig5 = 9;

int distance;          //Necessary vars for sonar readings, setting speeds, etc.
int distance2;
int distance3;
int distance4;
int distance5;

Servo motor;            //Create ESC drive motor 'motor' and steering servo 'steer'
Servo steer;

int speedRead;          //Vars necessary to control the speed of the motor via the ESC.
int speedRemap;
int forwardSpeed;     
int reverseSpeed;
int speedCenter = 1500;

const int neutral = 90;           //Steering servo positions for straight, left turns, and right turns. These can be changed and are set in degrees.
const int steerRight = 40;
const int steerLeft = 140;

const int frontThreshold = 66;    //Thresholds for the different 'types' of sonar sensors -- front, front side, and side. These can be changed and are set in centimeters.
const int frontSideThreshold = 45;
const int sideThreshold = 10;



void setup() {
  //Serial.begin(9600);             //Begins //Serial Monitor for debugging
  pinMode(echo, INPUT);           //Configuring pins for sonar modules
  pinMode(trig, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo3, INPUT);
  pinMode(trig3, OUTPUT);
  pinMode(echo4, INPUT);
  pinMode(trig4, OUTPUT);
  pinMode(echo5, INPUT);
  pinMode(trig5, OUTPUT);
  motor.attach(12, 1000, 2000);           //Attaches motor to pin 12
  steer.attach(13);                       //Attaches steering servo to pin 13
  //Serial.println("MOTOR ATTACHED");       //Prints a message for debug
  delay(500);
  motor.writeMicroseconds(forwardSpeed);  //Starts the car moving forward
}

void readFront(unsigned long duration) {        //Function called to read in front of car
  digitalWrite(trig, LOW);
  delayMicroseconds(3);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH, 100000);
  distance = (duration / 2) / 29.1;
  return (distance);
}

void readLeft(unsigned long leftDuration) {    //Function called to read left side of car
  digitalWrite(trig2, LOW);
  delayMicroseconds(3);
  digitalWrite(trig2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  leftDuration = pulseIn(echo2, HIGH, 40000);
  distance2 = (leftDuration / 2) / 29.1;
  return (distance2);
}

void readRight(unsigned long rightDuration) {   //Function called to read right side of car
  digitalWrite(trig3, LOW);
  delayMicroseconds(3);
  digitalWrite(trig3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  rightDuration = pulseIn(echo3, HIGH, 40000);
  distance3 = (rightDuration / 2) / 29.1;
  return (distance3);
}

void readFrontLeft(unsigned long frontLeftDuration) {    //Function called to read front left of car
  digitalWrite(trig4, LOW);
  delayMicroseconds(3);
  digitalWrite(trig4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig4, LOW);
  frontLeftDuration = pulseIn(echo4, HIGH, 87500);
  distance4 = (frontLeftDuration / 2) / 29.1;
  return (distance4);
}

void readFrontRight(unsigned long frontRightDuration) { //Function called to read front right of car
  digitalWrite(trig5, LOW);
  delayMicroseconds(3);
  digitalWrite(trig5, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig5, LOW);
  frontRightDuration = pulseIn(echo5, HIGH, 87500);
  distance5 = (frontRightDuration / 2) / 29.1;
  return (distance5);
}

void rightTurn() {              //Function called to execute a short, right turn to get away from a wall
  steer.write(steerRight);  //40 deg
  motor.writeMicroseconds(forwardSpeed); // dynamic speed (in loop) 
  delay(50);
  steer.write(neutral);
  //Serial.println("RIGHT TURN");
}

void leftTurn() {               //Function called to execute a short, left turn to get away from a wall
  steer.write(steerLeft);
  motor.writeMicroseconds(forwardSpeed);
  delay(50);
  steer.write(neutral);
  //Serial.println("LEFT TURN");
}

void longLeftTurn() {           //Function called to execute a long, left turn when a wall is detected by the front sensor. Should be approximately 90 degrees.
  steer.write(steerLeft);
  motor.writeMicroseconds(forwardSpeed);
  delay(475);
  steer.write(neutral);
  //Serial.println("LONG LEFT");
}

void longRightTurn() {          //Function called to execute a long, right turn when a wall is detected by the front sensor. Should be approximately 90 degrees.
  steer.write(steerRight);
  motor.writeMicroseconds(forwardSpeed);
  delay(475);
  steer.write(neutral);
  //Serial.println("LONG RIGHT");
}

void frontLeftTurn() {          //Function called to execute a 45 degree right turn when the front left sensor is triggered.
  steer.write(steerLeft);
  motor.writeMicroseconds(forwardSpeed);
  delay(90);
  steer.write(neutral);
  //Serial.println("FRONT LEFT TURN");
}

void frontRightTurn() {         //Function called to execute a 45 degree right turn when the front left sensor is triggered.
  steer.write(steerRight);
  motor.writeMicroseconds(forwardSpeed);
  delay(90);
  steer.write(neutral);
  //Serial.println("FRONT RIGHT TURN");
}

void readSidesAndFront() {      //Function called to determine if any short or 45 degree turns are needed. I made this a separate function since this is called three times in the loop, and the code
  readFrontLeft(0);             //became unnecessarily long. It reads all of the sensors except the front sensor and determines whether or not any minor corrections are necessary.
  //Serial.print("Front Left: ");
  //Serial.println(distance4);
  delay(3);
  readFrontRight(0);
  //Serial.print("Front Right: ");
  //Serial.println(distance5);
  delay(3);
  readLeft(0);
  //Serial.print("Left: ");
  //Serial.println(distance2);
  delay(3);
  readRight(0);
  //Serial.print("Right: ");
  //Serial.println(distance3);
  if (distance3 < sideThreshold && distance3 != 0) {
    readLeft(0);
    //Serial.print("Left: ");
    //Serial.println(distance2);
    if (distance3 < sideThreshold && distance3 != 0) {
      readLeft(0);
      //Serial.print("Left: ");
      //Serial.println(distance2);
      if (distance3 < sideThreshold && distance3 != 0) {
        leftTurn();
      }
      else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    } 
    else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  }
  else if (distance2 < sideThreshold && distance2 != 0) {
    readRight(0);
    //Serial.print("Right: ");
    //Serial.println(distance3);
    if (distance2 < sideThreshold && distance2 != 0) {
      readRight(0);
      //Serial.print("Right: ");
      //Serial.println(distance3);
      if (distance2 < sideThreshold && distance2 != 0) {
        rightTurn();
      }
      else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    }
    else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  }
  else if (distance5 <= 4 && distance5 != 0) {
    steer.write(steerRight);
    motor.writeMicroseconds(speedCenter);
    delay(1000);
    motor.writeMicroseconds(reverseSpeed);
    //Serial.println("Reversing");
    delay(950);
    motor.writeMicroseconds(speedCenter);
    steer.write(neutral);
  }
  else if (distance5 < frontSideThreshold && distance5 != 0) {
    readFrontRight(0);
    //Serial.print("Front Right: ");
    //Serial.println(distance5);
    if (distance5 < frontSideThreshold && distance5 != 0) {
      readFrontRight(0);
      //Serial.print("Front Right: ");
      //Serial.println(distance5);
      if (distance5 < frontSideThreshold && distance5 != 0) {
        frontLeftTurn();
      }
      else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    }
    else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  }
  else if (distance4 < 4 && distance4 != 0) {
    steer.write(steerLeft);
    motor.writeMicroseconds(speedCenter);
    delay(1000);
    motor.writeMicroseconds(reverseSpeed);
    //Serial.println("Reversing");
    delay(950);
    motor.writeMicroseconds(speedCenter);
    steer.write(neutral);
  }
  else if (distance4 < frontSideThreshold && distance4 != 0) {
    readFrontLeft(0);
    //Serial.print("Front Left: ");
    //Serial.println(distance4);
    if (distance4 < frontSideThreshold && distance4 != 0) {
      readFrontLeft(0);
      //Serial.print("Front Left: ");
      //Serial.println(distance4);
      if (distance4 < frontSideThreshold && distance4 != 0) {
        frontRightTurn();
      }
      else {
        motor.writeMicroseconds(forwardSpeed);
        steer.write(neutral);
      }
    }
    else {
      motor.writeMicroseconds(forwardSpeed);
      steer.write(neutral);
    }
  }
}
void loop() {                             
  speedRead = analogRead(A0);                   //Reads the potentiometer, remaps the values, and creates a forward and reverse speed that are both controlled by the potentiometer. This was    
  speedRemap = map(speedRead, 0, 1023, 0, 500); //implemented to accommodate the car battery discharging, since you can write a higher signal to a lower power battery to get the same speed
  forwardSpeed = speedCenter + speedRemap;      //as a lower signal with a higher power battery.
  reverseSpeed = speedCenter - speedRemap;
  //Serial.print("Forward Speed: ");
  //Serial.println(forwardSpeed);
  //Serial.print("Reverse Speed: ");
  //Serial.println(reverseSpeed);
  steer.write(neutral);
  motor.writeMicroseconds(forwardSpeed);
  digitalWrite(trig, LOW);
  digitalWrite(trig2, LOW);
  digitalWrite(trig3, LOW);
  digitalWrite(trig4, LOW);
  digitalWrite(trig5, LOW);
  readFront(0);
  //Serial.print("Forward Reading: ");    //Here is an example of the stacked if/else statements I mentioned before. I had issues with the car randomly turning even when no object was present.
  //Serial.println(distance);             //This was due to noise in the sonar readings. Here, the four if statements within each other essentially quadruple check to make sure that an obstacle
  delay(8);                             //is present before turning. This has a negligible impact on reading performance, and I used a similar structure for the readSidesAndFront() function above.
  if (distance < frontThreshold && distance != 0) {
    readFront(0);
    //Serial.print("Forward Reading: ");
    //Serial.println(distance);
    delay(8);
    if (distance < frontThreshold && distance != 0) {
      readFront(0);
      //Serial.print("Forward Reading: ");
      //Serial.println(distance);
      delay(8);
      if (distance < frontThreshold && distance != 0) {
        readFront(0);
        //Serial.print("Forward Reading: ");
        //Serial.println(distance);
        delay(8);
        if (distance > frontThreshold || distance == 0) {
          motor.writeMicroseconds(forwardSpeed);
          steer.write(neutral);
        }
        else {            //Here is the code for the passage of the quadruple check, indicating an obstacle. Below, the car reads the two sides and decides which one has more room. If one sensor
          readRight(0);   //returns an error (0), the car turns in that direction due to errors being returned for wide open spaces.
          readLeft(0);
          if (distance2 > distance3 && distance2 != 0) {
            longLeftTurn();
          }
          else if (distance3 > distance2 && distance3 != 0) {
            longRightTurn();
          }
          else if (distance2 == 0) {
            longLeftTurn();
          }
          else if (distance3 == 0) {
            longRightTurn();
          }
          readFront(0);                         //Code for backing up. If the front sonar is too close to the wall (less than 5cm), the car will pause, then back up and carry on with normal function.
          if (distance <= 5 && distance != 0) {
            motor.writeMicroseconds(speedCenter);
            delay(1000);
            motor.writeMicroseconds(reverseSpeed);
            //Serial.println("Reversing");
            delay(950);
            motor.writeMicroseconds(speedCenter);
            steer.write(neutral);
          }
        }
      }
      else {
        readSidesAndFront();    //Here, if the multiple checks for a wall in front fail at any point, the car reads the rest of the sensors for minor adjustments. It calls the function mentioned above.
      }
    }
    else {
      readSidesAndFront();
    }
  }
  else {
    readSidesAndFront();
  }
}
