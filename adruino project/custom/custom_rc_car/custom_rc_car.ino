const int pin_out_bw = 50;
const int pin_out_fw = 52;
const int pin_out_lf = 48;
const int pin_out_rg1 = 46;
const int pin_out_rg2 = 44;

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

const int nDelay = 500;               //milisec
const int nDelayForLed = 2000;               //milisec
const int delToNeutralSensor = 2;      //microsec - To reset the signal from sensor
const int delToWaitRespondSensor = 10; //microsec
int distance;
const int distanceForFrontTrig = 50; //cm
const int distanceForRightLeftTrig = 25; //cm
const int distanceStandarForSideLeftRight = 30; //cm
long duration;
const long fixedValToGiveDistance = 0.034 / 2; //Need to multiplied with duration to find distance

void setup()
{
  pinMode(pin_out_bw, OUTPUT);
  pinMode(pin_out_fw, OUTPUT);
  pinMode(pin_out_lf, OUTPUT);
  pinMode(pin_out_rg1, OUTPUT);
  pinMode(pin_out_rg2, OUTPUT);
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
  mainInitAndDelay(nDelay);
}
void loop()
{
  testSensorts(trigLeft);
  testSensorts(trigRight);
  testSensorts(trigFront);
  testSensorts(trigFrontLeft);
  testSensorts(trigFrontRight);

  // mainFunctionOfSensorsOfDrone();
}

/*/////////////////////////////////////////*//*Read sensor*/
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
  distance = duration * fixedValToGiveDistance;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
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
  if(distance < 200){
    Serial.print('\n');
    Serial.print("mTriger    ");
    Serial.print(mTriger);
    Serial.print(" distance  ");
    Serial.print(distance);
  }
  // Serial.println(distance);
}

void mainFunctionOfSensorsOfDrone()
{
  if(readSensorDistance(trigFront) < distanceForFrontTrig){
    if(readSensorDistance(trigFrontLeft) > readSensorDistance(trigFrontRight)){
      if(readSensorDistance(trigLeft) > distanceForRightLeftTrig){
        goLeftFw(nDelay);
      }else{
        mainInitAndDelay(nDelayForLed);
        goBackward(nDelay);
      }
    }else{
      if(readSensorDistance(trigRight) > distanceForRightLeftTrig){
        goRightFw(nDelay);
      }else{
        mainInitAndDelay(nDelayForLed);
        goBackward(nDelay);
      }
    }
  }else{
    goForward(nDelay);
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
  analogWrite(pin_out_rg1, 0);
  analogWrite(pin_out_rg2, 0);
}

void setOffRelayForRg()
{
  analogWrite(pin_out_rg1, 255);
  analogWrite(pin_out_rg2, 255);
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
  analogWrite(pin_out_rg1, 255);
  analogWrite(pin_out_rg2, 255);
}
void mainInitAndDelay(int mDelay)
{
  for (int i = 1; i <= 10; i++)
  {
    setOnMainLed();
    delay((nDelay / i) / 2);
    setOffMainLed();
    delay((nDelay / i) / 2);
  }
}

/*/////////////////////////////////////////*//*Moving of RC*/
void goForward(int mDelay)
{
  resetOutput(); //38
  setOnRelayForFw();
  delay(mDelay);
}
void goBackward(int mDelay)
{
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39 == 38+39
  delay(mDelay);
}

void goLeftFw(int mDelay)
{
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForLf(); //40 == 38+40
  delay(mDelay);
}
void goLeftBw(int mDelay)
{
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39
  setOnRelayForLf(); //40 == 38+39+40
  delay(mDelay);
}
void goRightFw(int mDelay)
{
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForRg();
  delay(mDelay);
}
void goRightBw(int mDelay)
{
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39
  setOnRelayForRg(); //41+42 == 38+39+41+42
  delay(mDelay);
}
void goNone(int mDelay)
{
  resetOutput();

  setOnMainLed();
  delay(mDelay);
  setOffMainLed();
}

/*/////////////////////////////////////////*//*For Testing mobility of car*/
void mainfunctionOfRC()
{
  resetOutput();
  goForward(nDelay);
  goNone(nDelay);
  goBackward(nDelay);
  goNone(nDelay);
  goLeftFw(nDelay);
  goNone(nDelay);
  goLeftBw(nDelay);
  goNone(nDelay);
  goRightFw(nDelay);
  goNone(nDelay);
  goRightBw(nDelay);
  goNone(nDelay);
}
