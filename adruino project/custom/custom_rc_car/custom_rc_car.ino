const int pin_out_bw = 50;
const int pin_out_fw = 52;
const int pin_out_lf = 48;
const int pin_out_rg = 46;

const int trigFront = 2;
const int echoFront = 3;
const int trigFrontRight = 4;
const int echoFrontRight = 5;
const int trigFrontLeft = 6;
const int echoFrontLeft = 7;
const int trigLeft = 8;
const int echoLeft = 9;
const int trigRight = 10;
const int echoRight = 11;

const int nDelay = 4000;               //sec
const int nDelayForLed = 10000;        //sec
const int delToNeutralSensor = 2;      //microsec - To reset the signal from sensor
const int delToWaitRespondSensor = 10; //microsec
int distance;
bool trigToMakeManeuver;      //Triger for brainlessDrive to excecute the second part of algorythm
const int distanceTrigC = 50; //50 cm
const int distanceTrigA = 30; //30 cm
const int distanceTrigB = 15; //15 cm

long duration;

const long fixedValToGiveDistance = 0.034 / 2; //Need to multiplied with duration to find distance

//Main method it execute 1 time when arduino starts
void setup()
{
  pinMode(pin_out_bw, OUTPUT);
  pinMode(pin_out_fw, OUTPUT);
  pinMode(pin_out_lf, OUTPUT);
  pinMode(pin_out_rg, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigFront, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(trigFrontLeft, OUTPUT);
  pinMode(echoFrontLeft, INPUT);
  pinMode(trigFrontRight, OUTPUT);
  pinMode(echoFrontRight, INPUT);
  pinMode(trigLeft, OUTPUT);
  pinMode(echoLeft, INPUT);
  pinMode(trigRight, OUTPUT);
  pinMode(echoRight, INPUT);
  Serial.begin(9600); // Starts the serial communication
  mainInitAndDelay();
}

//Main method that execute in infinite loop
void loop()
{
  // mainfunctionOfRC();
  // testSensorts();
  brainlessDrive();
}

void brainlessDrive()
{
  if (!trigToMakeManeuver)
  {
    if (readSensorDistance(trigFront) > distanceTrigA)
    {

      goForward();
    }
    else if (readSensorDistance(trigFrontRight) > distanceTrigA && readSensorDistance(trigFront) > distanceTrigB)
    {

      goRightFw();
    }
    else if (readSensorDistance(trigFrontLeft) > distanceTrigA && readSensorDistance(trigFront) > distanceTrigB)
    {

      goLeftFw();
    }
    else if (readSensorDistance(trigRight) > distanceTrigA || readSensorDistance(trigFrontLeft) > distanceTrigA)
    {

      trigToMakeManeuver = true;
    }
    else
    {
      goNone();
    }
  }
  else
  { //Second part of algorythm

    if (readSensorDistance(trigRight) > distanceTrigB && readSensorDistance(trigFront) < distanceTrigC && readSensorDistance(trigFrontRight) < distanceTrigC)
    {
      goRightBw();
    }
    else if (readSensorDistance(trigLeft) > distanceTrigB && readSensorDistance(trigFront) < distanceTrigC && readSensorDistance(trigFrontRight) < distanceTrigC)
    {
      goLeftBw();
    }
    else
    {
      trigToMakeManeuver = false;
    }
  }
}

//Test method was created only for testing purpose
void testMethod()
{

  if (readSensorDistance(trigFront) > 20)
  {
    goForward();
  }
  else
  {
    goNone();
  }
  //goForward();
}

/*/////////////////////////////////////////*/ /*Read sensors distance*/
long readSensorDistance(int mTriger)
{
  int tempEcho;

  switch (mTriger)
  {
  case trigFront:
    tempEcho = echoFront;
    break;
  case trigFrontLeft:
    tempEcho = echoFrontLeft;
    break;
  case trigFrontRight:
    tempEcho = echoFrontRight;
    break;
  case trigLeft:
    tempEcho = echoLeft;
    break;
  case trigRight:
    tempEcho = echoRight;
    break;
  }
  digitalWrite(mTriger, LOW);
  delayMicroseconds(delToNeutralSensor);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(mTriger, HIGH);
  delayMicroseconds(delToWaitRespondSensor);
  digitalWrite(mTriger, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(tempEcho, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Triger: ");
  Serial.println(mTriger);
  Serial.println("__________");
  return distance;
}

/*/////////////////////////////////////////*/ /*Set on/of for pin to move RC*/
void setOnRelayForBw()
{
  analogWrite(pin_out_bw, 255);
}
void setOffRelayForBw()
{
  analogWrite(pin_out_bw, 0);
}
void setOnRelayForFw()
{
  analogWrite(pin_out_fw, 255);
}
void setOffRelayForFw()
{
  analogWrite(pin_out_fw, 0);
}
void setOnRelayForLf()
{
  analogWrite(pin_out_lf, 255);
}
void setOffRelayForLF()
{
  analogWrite(pin_out_lf, 0);
}
void setOnRelayForRg()
{
  analogWrite(pin_out_rg, 255);
}
void setOffRelayForRg()
{
  analogWrite(pin_out_rg, 0);
}
void setOnMainLed()
{
  digitalWrite(LED_BUILTIN, HIGH);
}
void setOffMainLed()
{
  digitalWrite(LED_BUILTIN, LOW);
}
void resetOutput()
{
  analogWrite(pin_out_fw, 0);
  analogWrite(pin_out_bw, 0);
  analogWrite(pin_out_lf, 0);
  analogWrite(pin_out_rg, 0);
}
void mainInitAndDelay()
{
  for (int i = 1; i <= 10; i++)
  {
    Serial.println(10 - i);
    setOnMainLed();
    delay((nDelay / i) / 4);
    setOffMainLed();
    delay((nDelay / i) / 4);
  }
}
void msgForMovement(const String &msg)
{
  Serial.print("Movement Act: ");
  Serial.println(msg);
}

/*/////////////////////////////////////////*/ /*Moving commands of RC*/
void goForward()
{
  resetOutput(); //38
  setOnRelayForFw();
  msgForMovement("Forward");
}
void goBackward()
{
  resetOutput();
  setOnRelayForBw();
  msgForMovement("Backward");
}

void goLeftFw()
{
  resetOutput();
  setOnRelayForFw();
  setOnRelayForLf();
  msgForMovement("Left Forward");
}
void goLeftBw()
{
  resetOutput();
  setOnRelayForBw();
  setOnRelayForLf();
  msgForMovement("Left Backward");
}
void goRightFw()
{
  resetOutput();
  setOnRelayForFw();
  setOnRelayForRg();
  msgForMovement("Right Forward");
}
void goRightBw()
{
  resetOutput();
  setOnRelayForBw();
  setOnRelayForRg();
  msgForMovement("Rigth Backward");
}
void goNone()
{
  resetOutput();

  setOnMainLed();
  setOffMainLed();
  msgForMovement("No movement");
}

/*/////////////////////////////////////////*/ /*For Testing mobility of car*/
void mainfunctionOfRC()
{
  Serial.println("Reset out Put");
  resetOutput();

  Serial.println("Forward");
  goForward();
  delay(nDelay);

  Serial.println("None");
  goNone();
  delay(nDelay);

  Serial.println("Backward");
  goBackward();
  delay(nDelay);

  Serial.println("None");
  goNone();
  delay(nDelay);

  Serial.println("Left Forward");
  goLeftFw();
  delay(nDelay);

  Serial.println("None");
  goNone();
  delay(nDelay);

  Serial.println("Left Backward");
  goLeftBw();
  delay(nDelay);

  Serial.println("None");
  goNone();
  delay(nDelay);

  Serial.println("Right Forward");
  goRightFw();
  delay(nDelay);

  Serial.println("None");
  goNone();
  delay(nDelay);

  Serial.println("Righ Backward");
  goRightBw();
  delay(nDelay);

  Serial.println("None");
  goNone();
  delay(nDelay);
}

/*/////////////////////////////////////////*/ /*For Testing sensors of car*/
void testSensorts(int mTriger)
{
  int tempEcho;

  switch (mTriger)
  {
  case trigFront:
    tempEcho = echoFront;
    break;
  case trigFrontLeft:
    tempEcho = echoFrontLeft;
    break;
  case trigFrontRight:
    tempEcho = echoFrontRight;
    break;
  case trigLeft:
    tempEcho = echoLeft;
    break;
  case trigRight:
    tempEcho = echoRight;
    break;
  }
  digitalWrite(mTriger, LOW);
  delayMicroseconds(delToNeutralSensor);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(mTriger, HIGH);
  delayMicroseconds(delToWaitRespondSensor);
  digitalWrite(mTriger, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(tempEcho, HIGH);
  // Calculating the distance
  long distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Triger: ");
  Serial.println(mTriger);
  Serial.println("__________");
}