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
const int distanceForFrontTrigSafeCall = 50;     //50; //cm
const int distanceForRightLeftTrigSafeCall = 20; //25; //cm
const int distanceForFrontTrig = 25;             //50; //cm
const int distanceForRightLeftTrig = 10;         //25; //cm
const int distanceForFrontTrigCloseCall = 12;    //25; //cm
const int distanceForRightLeftTrigCloseCall = 5; //12; //cm
long duration;
bool lastMoveWasBack;
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
  lastMoveWasBack = false;
  Serial.begin(9600); // Starts the serial communication
  mainInitAndDelay();
}

//Main method that execute in infinite loop
void loop()
{
  // mainfunctionOfRC();
  
  // algthmForDecision();
  
  // algorithmForDecision2();
  testMethod();
  //  for(int j = 2; j < 11; j = j+2){
  //    testSensorts(j);
  //  }
}

//Test method was created only for testing purpose
void testMethod() {

  if(readSensorDistance(trigFront)>20){
    goForward();
  } else {
    goNone();
  }
//goForward();
  
}

/*/////////////////////////////////////////*/ /*Read sensor*/
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

//Main Algorthm of project
void algthmForDecision()
{

  if (lastMoveWasBack || readSensorDistance(trigFront) < distanceForFrontTrig) //last move was Backward? AND
  {
    if (readSensorDistance(trigFrontLeft) > distanceForRightLeftTrig || readSensorDistance(trigFrontRight) > distanceForRightLeftTrig) // Is FL or FR > 30cm (distanceForRightLeftTrig)
    {
      if (readSensorDistance(trigFrontLeft) > readSensorDistance(trigFrontRight))
      {
        if (readSensorDistance(trigLeft) > distanceForRightLeftTrig)
        {
          goLeftFw();
          lastMoveWasBack = false;
        }
        else
        {
          goBackward();
          lastMoveWasBack = true;
        }
      }
      else
      {
        if (readSensorDistance(trigRight) > distanceForRightLeftTrig)
        {
          goRightFw();
          lastMoveWasBack = false;
        }
        else
        {
          goBackward();
          lastMoveWasBack = true;
        }
      }
    }
    else
    {
      goBackward();
      lastMoveWasBack = true;
    }
  }
  else
  {
    goForward();
  }
}

void algorithmForDecision2()
{
  while (readSensorDistance(trigFront) > distanceForFrontTrig && readSensorDistance(trigFrontLeft) > distanceForRightLeftTrig && readSensorDistance(trigFrontRight) > distanceForRightLeftTrig)
  {
    goForward();
    Serial.println("&&0   !!!!");
  }

  if (readSensorDistance(trigFront) < distanceForFrontTrig)
  {
    do
    {
      if (readSensorDistance(trigFrontLeft) < readSensorDistance(trigFrontRight))
      {
        goRightFw();
      }
      else
      {
        goLeftFw();
      }
      Serial.print("&&1   ");
      Serial.println(readSensorDistance(trigFrontLeft) < readSensorDistance(trigFrontRight));
    } while (readSensorDistance(trigFront) < distanceForFrontTrigSafeCall);
  }
  else if (readSensorDistance(trigFrontLeft) < distanceForRightLeftTrig)
  {
    if (readSensorDistance(trigFrontRight) > distanceForRightLeftTrigCloseCall)
    {
      do
      {
        goRightFw();
        Serial.print("&&2   ");
        Serial.println(readSensorDistance(trigFrontRight) > distanceForRightLeftTrigCloseCall);
      } while (readSensorDistance(trigFrontLeft) < distanceForRightLeftTrigSafeCall);
    }
    else
    {
      do
      {
        goLeftBw();
        Serial.print("&&3   ");
        Serial.println(readSensorDistance(trigFrontRight) > distanceForRightLeftTrigCloseCall);
      } while (readSensorDistance(trigFront) < distanceForFrontTrigCloseCall && readSensorDistance(trigFrontLeft) < distanceForRightLeftTrigCloseCall && readSensorDistance(trigFrontRight) < distanceForRightLeftTrigCloseCall);
    }
  }
  else
  {
    if (readSensorDistance(trigFrontLeft) > distanceForRightLeftTrigCloseCall)
    {
      do
      {
        goLeftFw();
        Serial.print("&&4   ");
        Serial.println(readSensorDistance(trigFrontLeft) > distanceForRightLeftTrigCloseCall);
      } while (readSensorDistance(trigFrontRight) < distanceForRightLeftTrigSafeCall);
    }
    else
    {
      do
      {
        goRightBw();
        Serial.print("&&5   ");
        Serial.println(readSensorDistance(trigFrontLeft) > distanceForRightLeftTrigCloseCall);
      } while (readSensorDistance(trigFront) < distanceForFrontTrigCloseCall && readSensorDistance(trigFrontLeft) < distanceForRightLeftTrigCloseCall && readSensorDistance(trigFrontRight) < distanceForRightLeftTrigCloseCall);
    }
  }
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
  Serial.println("__________");
}
/*/////////////////////////////////////////*/ /*Moving of RC*/
void goForward()
{
  resetOutput(); //38
  setOnRelayForFw();
//  delay(nDelay);
  msgForMovement("Forward");
}
void goBackward()
{
  resetOutput();
  setOnRelayForBw();
//  delay(nDelay);
  msgForMovement("Backward");
}

void goLeftFw()
{
  resetOutput();
  setOnRelayForFw();
  setOnRelayForLf();
//  delay(nDelay);
  msgForMovement("Left Forward");
}
void goLeftBw()
{
  resetOutput();
  setOnRelayForBw();
  setOnRelayForLf();
//  delay(nDelay);
  msgForMovement("Left Backward");
}
void goRightFw()
{
  resetOutput();
  setOnRelayForFw();
  setOnRelayForRg();
//  delay(nDelay);
  msgForMovement("Right Forward");
}
void goRightBw()
{
  resetOutput();
  setOnRelayForBw();
  setOnRelayForRg();
//  delay(nDelay);
  msgForMovement("Rigth Backward");
}
void goNone()
{
  resetOutput();

  setOnMainLed();
//  delay(nDelay);
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
  Serial.println("None");
  goNone();
  Serial.println("Backward");
  goBackward();
  Serial.println("None");
  goNone();
  Serial.println("Left Forward");
  goLeftFw();
  Serial.println("None");
  goNone();
  Serial.println("Left Backward");
  goLeftBw();
  Serial.println("None");
  goNone();
  Serial.println("Right Forward");
  goRightFw();
  Serial.println("None");
  goNone();
  Serial.println("Righ Backward");
  goRightBw();
  Serial.println("None");
  goNone();
}
