const int pin_out_bw = 50;
const int pin_out_fw = 52;
const int pin_out_lf = 48;
const int pin_out_rg1 = 46;
const int pin_out_rg2 = 44;
/* 50     -goes front
   50+52  -goes back
   none   -neutral
   48     -left
   46+44     -right   
*/
const int nDelay = 2000; //milisec

void setup() {
  //Declaring LED pin as output
  pinMode(pin_out_bw, OUTPUT);
  pinMode(pin_out_fw, OUTPUT);
  pinMode(pin_out_lf, OUTPUT);
  pinMode(pin_out_rg1, OUTPUT);
  pinMode(pin_out_rg2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  mainInitAndDelay(nDelay);
}
void loop() {
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
/*/////////////////////////////////////////*/
void setOnRelayForBw() {
  analogWrite(pin_out_bw, 255);
}
void setOffRelayForBw() {
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
  analogWrite(pin_out_rg1, 0);
  analogWrite(pin_out_rg2, 0);
}

void setOffRelayForRg() {
  analogWrite(pin_out_rg1, 255);
  analogWrite(pin_out_rg2, 255);
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
  analogWrite(pin_out_rg1, 255);
  analogWrite(pin_out_rg2, 255);
}
void mainInitAndDelay(int mDelay){
  for(int i=1; i<=10; i++){
    setOnMainLed();
    delay((nDelay/i)/2);
    setOffMainLed();
    delay((nDelay/i)/2);
  }
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
  setOnRelayForRg();
  delay(mDelay);
}
void goRightBw(int mDelay){
  resetOutput();
  setOnRelayForFw(); //38
  setOnRelayForBw(); //39
  setOnRelayForRg(); //41+42 == 38+39+41+42
  delay(mDelay);
}
void goNone(int mDelay){
  resetOutput();
  
  setOnMainLed();
  delay(mDelay);
  setOffMainLed();
}
