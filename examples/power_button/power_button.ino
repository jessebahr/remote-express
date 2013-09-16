
#include <IRremote.h>
#include <remote_express.h>

int irPin = 2;
RemoteExpress irx(irPin);


int ledPin = 5;
boolean lightOn = false;

void toggleLight(unsigned long key){
  Serial.println("got it!");
  lightOn = !lightOn;
}

void setup(){
  irx.onpress(0x736ED82E, toggleLight);
  irx.setup();
  
  
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop(){
  int buttons = irx.loop();
  
  if (lightOn){
    analogWrite(ledPin, 255);
  }
  else{
    analogWrite(ledPin, 127);
  }
}
