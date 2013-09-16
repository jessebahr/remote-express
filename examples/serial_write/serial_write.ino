/**

Tests Remote Express with a universal remote set to a cable box. Writes out to the serial out when a button is received.

*/

#include <IRremote.h>
#include <remote_express.h>

int irPin = 2;
RemoteExpress irx(irPin);

void pressPower(unsigned long key){
  Serial.print("Power pressed: ");
  Serial.println(key, HEX);
}

void holdPower(unsigned long key, unsigned long ms){
  Serial.print("Power held: ");
  Serial.print(key, HEX);
  Serial.print(", held for ");
  Serial.println(ms/1000, DEC); 
}

void holdGuidePeriodic(unsigned long key, unsigned long ms){
  Serial.print("Guide held: ");
  Serial.print(key, HEX);
  Serial.print(", held for ");
  Serial.println(ms/1000, DEC); 
}

void pressMenu(unsigned long key){
  Serial.print("Menu pressed: ");
  Serial.println(key, HEX);
}

void pressNum(int num){
  Serial.print("Number pressed: ");
  Serial.println(num, DEC);
}

void holdNum(int num, unsigned long ms){
  Serial.print("Number held: ");
  Serial.print(num, DEC);
  Serial.print(", held for ");
  Serial.println(ms/1000, DEC); 
}


void setup(){
  Serial.begin(9600);
  
  irx.onpress(0x736ED82E, pressPower);
  irx.onhold(0x736ED82E, holdPower);

  irx.onhold(0xE9C74319, 1000, holdGuidePeriodic); //notify once per second
  
  irx.onpress(0x19EB3488, pressMenu);

  unsigned long numberKeys[10] = {
    0x6E56F924,
    0x92DF9279,
    0x87CDD0EF,
    0x37788763,
    0xA519853B,
    0x5CDD8FBD,
    0x42DD49,
    0x31BB009F,
    0x153F90A7,
    0x6872B60C
  };
  irx.setNums(numberKeys);
  irx.onnumpress(pressNum);
  irx.onnumhold(1000, holdNum);

  irx.setup();
}

void loop(){
  irx.loop();
}
