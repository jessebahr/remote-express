#ifndef REMOTE_EXPRESS_H
#define REMOTE_EXPRESS_H

#include <arduino.h>
#include <IRremote.h>


#define REMOTE_EXPRESS_NUM_KEYS_COUNT 11
#define REMOTE_EXPRESS_KEYS_SIZE 100

class RemoteExpress{
public:
	/**
	 * create a RemoteExpress with the given pin as input from the infrared receiver.
	 */
    RemoteExpress(int pin);

	/**
	 * setup. Run this in the main setup() function, after adding all the press/hold handlers. 
	 */
	void setup();

	/**
	 * run, returning the number of button presses recorded.
	 */
	int loop();

	//callback registers
	/**
	 * Register the press handler for the given code, which will be called when the given keycode is received.
	 * code - the remote code to catch
	 * cb - a function of the form void cb(unsigned long code){} - will be called with the code
	 *   that initiated the call.
	 */
	void onpress(unsigned long code, void (*cb) (unsigned long code));
	
	/**
	 * Register the hold handler for the given code, which will be called every time the 
	 * "you're holding down a button" signal is received after receiving "code" (and no 
	 * other codes in between).
	 * code - the remote code to catch
	 * cb - a function of the form void cb(unsigned long code, unsigned long ms){} - will be 
	 *   called with the code that initiated the call, and the number of milliseconds since the
	 *   corresponding "press".
	 */
	void onhold(unsigned long code, void (*cb) (unsigned long code, unsigned long millisSincePress));

	/**
	 * Register the hold handler for the given code, which will be after everyMillis milliseconds 
	 * when the "you're holding down a button" signal is received after receiving "code" (and no 
	 * other codes in between),
	 * code - the remote code to catch
	 * everyMillis - how often to call the handler. Wait at least this many millisseconds between calls.
	 *   This delay also applies after the initial press.
	 * cb - a function of the form void cb(unsigned long code, unsigned long ms){} - will be 
	 *   called with the code that initiated the call, and the number of milliseconds since the
	 *   corresponding "press".
	 */
	void onhold(unsigned long code, unsigned long everyMillis, void (*cb) (unsigned long code, unsigned long millisSincePress));

	//number codes (these override onhold/onpress above)
	/**
	 * Set the given code as the given number key. Note that any code set this way will never
	 * receive a normal "onpress" or "onhold" handler, and will only receive the "onnum..."
	 * versions.
	 */
	void setNum(int number, unsigned long code);
	
	/**
	 * Set all the number keys at once, with the array index being the number of the key,
	 * and the array value being the code for that key. 
	 * See setNum(int, unsigned long)
	 */
	void setNums(unsigned long codes[REMOTE_EXPRESS_NUM_KEYS_COUNT]);

	/**
	 * register the handler for number key presses.
	 * cb - a function of the form void cb (int num){}. Will be called with the number key
	 * that was pressed.
	 */
	void onnumpress(void (*cb) (int num));
	
	/**
	 * Register the handler for number keys being held down.
	 * cb - a function of the form void cb (int num, unsigned long ms). Will be passed the 
	 *   number fo the key, and the milliseconds since the press.
	 */
	void onnumhold(void (*cb) (int num, unsigned long millisSincePress));

	/**
	 * Register the handler for number keys being held down. Only call when it's been held
	 * down for the given number of milliseconds.
	 * everyMillis - the number of milliseconds to wait between calls to the callback.
	 *   This is a lower bound; the calls may take a bit longer than this, depending on when
	 *   the repeat code is received.
	 * cb - a function of the form void cb (int num, unsigned long ms). Will be passed the 
	 *   number fo the key, and the milliseconds since the press.
	 */
	void onnumhold(unsigned long everyMillis, void (*cb) (int num, unsigned long millisSincePress));

private:
	IRrecv irrecv;
	decode_results remoteResults;
	unsigned long lastCode;
	//when the last code was received
	unsigned long lastCodeTime;
	//the codes for the number keys
	unsigned long numkeys[REMOTE_EXPRESS_NUM_KEYS_COUNT];
	//the number key last pressed, or -1 if none
	int lastPressNumkey;
	//the index of the last code pressed, or -1 if none
	int lastPressIndex;
	unsigned long holdEveryMicros;
	//the next time call a hold callback
	unsigned long nextHoldTime;

	//callbacks
	void (*numkeyCB) (int num);
	void (*numkeyHoldCB) (int num, unsigned long microsSincePress);
	unsigned long numkeyHoldEveryMicros;

	//todo make this a growing hash table or sort for quicker lookup
	unsigned long lookupKeys[REMOTE_EXPRESS_KEYS_SIZE];
	void (*lookupCB[REMOTE_EXPRESS_KEYS_SIZE]) (unsigned long code);
	void (*lookupHoldCB[REMOTE_EXPRESS_KEYS_SIZE]) (unsigned long code, unsigned long microsSincePress);
	unsigned long lookupHoldEveryMicros[REMOTE_EXPRESS_KEYS_SIZE];
	int lookupKeysSize;


	//helpers for the keys
	int lookupNumkey(unsigned long key);
	int lookupKey(unsigned long key);
	int addLookupKey(unsigned long key);

	//helpers for loop()
	void handleRepeat();
	int handlePress(unsigned long key);

};




#endif
