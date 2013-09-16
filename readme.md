Remote Express

A quick-and-dirty Arduino library for easily setting up a infrared remote control reader via callbacks

Dependencies:
* Arduino
* Arduino-IRremote library: https://github.com/shirriff/Arduino-IRremote


Installation
============
* Find your Arduino libraries directory (For Windows, it's probably C:\Program Files (x86)\Arduino\libraries\)
* Make a remote_express directory underneath it
* Download this project as a .zip
* Extract the .zip into the remote_express directory
* Follow the directions to install the Arduino-IRremote library - https://github.com/shirriff/Arduino-IRremote

Use
===
You can check out the examples, but here's a short example:

```c++
#include <IRremote.h> //yes, you need to include this. First. I know, I know.
#include <remote_express.h>

#define CODE_POWER_BUTTON 0x736ED82E

void myCallback(unsigned long keycode){
	//called when the power button is pressed
}


RemoteExpress irx(2); //the input pin of the IR receiver

void setup(){
	irx.onpress(CODE_POWER_BUTTON, myCallback); //setup a callback
	irx.setup(); //should be called last
}

void loop(){
	irx.loop(); //calls myCallback if power button pressed (not held)
}

```

You can get the keycodes and wire it up like in this tutorial: (todo)

Setting up Callbacks
====================
Each keycode can only have 1 of each type of callback (1 press and 1 hold). If multiple calls are made for the same keycode, the last one will be kept and the first dropped. Also, if a keycode's been set as a number, its regular press/hold will not be called (since the number callback will be called).

onpress(code, cb)
-----------------
Setup a press handler for a keycode. The function cb will be called whenever the keycode is pressed.

_Arguments:_
* unsigned long code - The keycode (see the tutorial above to get this)
* void (*cb) (unsigned long code) - The function to call, i.e. void cb(unsigned long code){}

onhold(code, everyMicro, cb)
----------------------------
Setup a handler for holding down the button signified by the given keycode. The function cb will be called as often as "everyMicro" while a holding-down code is being received. 

_Arguments:_
* unsigned long code - The keycode
* unsigned long everyMicro - How often to call the callback function in microseconds. Note that this is a lower limit.
* void (*cb) (unsigned long code, unsigned long ms) - The function to call whenever the key is being held down. The callback will be called with the keycode that was held (the original, not the repeater), and the number of microseconds since it was originally pressed. ex: , i.e. void cb(unsigned long code, unsigned long ms){}

onhold(code, cb)
----------------
Alias for onhold(code, 0, cb) - call the function every time the repeat signal is received after code is received.

onnumpress(cb)
--------------
Setup a handler for when one of the number keys is pressed. See Setting up number keys below for more info.

_Arguments:_
* void (*cb) (int num) - The function to call when one of the number keys is pressed. Which number key (0-9) was pressed is passed in. ex: void cb(int numberKey){}

onnumhold(everyMicro, cb)
-------------------------
Setup a handler for holding down one of the number buttons. The function cb will be called as often as "everyMicro" while holding the number key.

_Arguments:_
* unsigned long everyMicro - How often to call the callback function in microseconds. Note that this is a lower limit.
* void (*cb) (int num, unsigned long microsSincePress) - The function to call when a number key is being held down. The callback will be called with the 0-9 number of the key that was held originally, and the number of microseconds since it was originally pressed. ex: , i.e. void cb(int num, unsigned long ms){}

onnumhold(cb)
-------------
Alias for onnumhold(0, cb) - call the callback function every time the repeat signal is received.


Setting the Number Keys
=======================
There are 10 number keys, 0 through 9. You'll need to tell Remote Express which keycodes correspond to number keys. There are two methods of setting the number keys: individually and all at once via an array:

```c++
//set up individually
irx.setNum(0, 0x12345670); //the "0" key's code is 0x12345670
irx.setNum(1, 0x12345671);
// .  .  .
irx.setNum(9, 0x12345679);

//...or set up once
unsigned long numberKeys[10] = {0x12345670, 0x12345671, . . ., 0x12345679};
irx.setNums(numberKeys); //array index == key number

```

To Do
=====
* Give it a proper hash for searching keycodes? (Currently just a simple O(n) search every time, but then again n is always small...)
* Allow for more than 1 press and hold handler per keycode
* Add a keymaster/gatekeeper reference (or has this been done to death?)
* Detecting button sequences 


