//Vars necessary to control the speed of the motor via the ESC.
const int pin_out_bw = 38;
const int pin_out_fw = 39;
const int pin_out_lf = 40;
const int pin_out_rg = 41;
/* 38     -goes front
   38+39  -goes back
   none   -neutral
   40     -left
   41     -right   
*/


const int nDelay = 1500; //milisec

void setup() {
  //Declaring LED pin as output
  pinMode(pin_out_bw, OUTPUT);
  pinMode(pin_out_fw, OUTPUT);
  pinMode(pin_out_lf, OUTPUT);
  pinMode(pin_out_rg, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  goBackward(nDelay);
  goNone(nDelay);
  goForward(nDelay);
  goNone(nDelay);
  goLeftFw(nDelay);
  goNone(nDelay);
  goLeftBw(nDelay);
  goNone(nDelay);
}


/*/////////////////////////////////////////*/
void setOnRelayForBw() {
  // digitalWrite(LED_BUILTIN, HIGH);
  analogWrite(pin_out_bw, 255);
}
void setOffRelayForBw() {
  // digitalWrite(LED_BUILTIN, LOW);
  analogWrite(pin_out_bw, 0);
}
void setOnRelayForFw() {
  analogWrite(pin_out_fw, 255);
}
void setOffRelayForFw() {
  analogWrite(pin_out_fw, 0);
}
void setOnRelayForLf() {
  analogWrite(pin_out_lf, 255);
}
void setOffRelayForLF() {
  analogWrite(pin_out_lf, 0);
}
void setOnRelayForRg() {
  analogWrite(pin_out_rg, 255);
}
void setOffRelayForRg() {
  analogWrite(pin_out_rg, 0);
}

void setOnMainLed(){
  digitalWrite(LED_BUILTIN, HIGH);
}
void setOffMainLed(){
  digitalWrite(LED_BUILTIN, LOW);
}
void resetOutput(){
  analogWrite(pin_out_fw, 0);
  analogWrite(pin_out_bw, 0);
  analogWrite(pin_out_lf, 0);
  analogWrite(pin_out_rg, 0);
}
/*/////////////////////////////////////////*/

void goForward(int mDelay){
  resetOutput(); //38
  setOnRelayForFw();
  delay(mDelay);
} 
void goBackward(int mDelay){
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39 == 38+39
  delay(mDelay);
}

void goLeftFw(int mDelay){
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForLf(); //40 == 38+40 
  delay(mDelay);
} 
void goLeftBw(int mDelay){
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39
  setOnRelayForLf(); //40 == 38+39+40
  delay(mDelay);
}
void goRightFw(int mDelay){
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //41 == 38+41
  delay(mDelay);
}
void goRightBw(int mDelay){
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39
  setOnRelayForRg(); //41 == 38+39+41
  delay(mDelay);
}
void goNone(int mDelay){
  resetOutput();
  
  setOnMainLed();
  delay(mDelay);
  setOffMainLed();
}
