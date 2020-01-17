
int speedRead;          //Vars necessary to control the speed of the motor via the ESC.
int speedRemap;
int forwardSpeed = 2000;
int reverseSpeed;
int speedCenter = 1500;
const int frontThreshold = 66;    //Thresholds for the different 'types' of sonar sensors -- front, front side, and side. These can be changed and are set in centimeters.
const int frontSideThreshold = 45;
const int sideThreshold = 10;

const int mainDelay = 1500;
const bool asd = false;
const int pin_out = 6;

/*START -- MAIN PROJECT */
void setup() {
  //Declaring LED pin as output
  pinMode(pin_out, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  backwardStart();
  backwardStop();
}
/*END -- MAIN PROJECT */

void backwardStart() {
  digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(pin_out, 250);
  delay(mainDelay);
}
void backwardStop() {
  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(pin_out, 0);
  delay(mainDelay);
}
