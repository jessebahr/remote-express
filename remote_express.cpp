#include "remote_express.h"

void defaultNumKeyCB(int num){}
void defaultNumKeyHoldCB(int num, unsigned long micros){}
void defaultKeyCB(unsigned long key){}
void defaultKeyHoldCB(unsigned long key, unsigned long micros){}

RemoteExpress::RemoteExpress(int pin) : irrecv(pin){
	lookupKeysSize = 0;
	for (int i = 0; i < REMOTE_EXPRESS_NUM_KEYS_COUNT; i++){
		numkeys[i] = 0;
	}

	numkeyCB = defaultNumKeyCB;
	numkeyHoldCB = defaultNumKeyHoldCB;
}

void RemoteExpress::setup(){
	irrecv.enableIRIn();
}

int RemoteExpress::loop(){
	int result = 0;
	if (irrecv.decode(&remoteResults)){
		if (remoteResults.value == REPEAT){
			handleRepeat();
		}
		else{
			lastCode = remoteResults.value;
			lastCodeTime = millis();
			result = handlePress(remoteResults.value);
		}

		irrecv.resume();
	}

	return result;
}

void RemoteExpress::handleRepeat(){
	unsigned long time = millis();
	if (time >= nextHoldTime){
		unsigned long timeSincePress = time - lastCodeTime;
		if (lastPressNumkey < 0){
			if (lastPressIndex < 0)
				return;

			(* lookupHoldCB[lastPressIndex])(lookupKeys[lastPressIndex], timeSincePress);
		}
		else{
			(* numkeyHoldCB)(lastPressNumkey, timeSincePress);
		}

		nextHoldTime = time + holdEveryMicros;
	}
}

int RemoteExpress::handlePress(unsigned long value){
	lastPressNumkey = -1;
	lastPressIndex = -1;
	int numkey = lookupNumkey(value);
	if (numkey < 0){
		int cbIndex = lookupKey(value);
		if (cbIndex < 0)
			return 0;

		lastPressIndex = cbIndex;
		holdEveryMicros = lookupHoldEveryMicros[cbIndex];
		nextHoldTime = lastCodeTime + holdEveryMicros;
		(*lookupCB[cbIndex])(value);
		return 1;
	}
	else{
		lastPressNumkey = numkey;
		holdEveryMicros = numkeyHoldEveryMicros;
		nextHoldTime = lastCodeTime + holdEveryMicros;
		(* numkeyCB)(numkey);
		return 1;
	}
}

void RemoteExpress::onpress(unsigned long code, void (*cb) (unsigned long code)){
	int i = lookupKey(code);
	if (i < 0){
		i = addLookupKey(code);
	}

	lookupCB[i] = cb;
}

void RemoteExpress::onhold(unsigned long code, void (*cb) (unsigned long code, unsigned long microsSincePress)){
	onhold(code, 0, cb);
}

void RemoteExpress::onhold(unsigned long code, unsigned long everyMicro, void (*cb) (unsigned long code, unsigned long microsSincePress)){
	int i = lookupKey(code);
	if (i < 0){
		i = addLookupKey(code);
	}

	lookupHoldEveryMicros[i] = everyMicro;
	lookupHoldCB[i] = cb;
}


void RemoteExpress::setNum(int number, unsigned long code){
	numkeys[number] = code;
}

void RemoteExpress::setNums(unsigned long codes[REMOTE_EXPRESS_NUM_KEYS_COUNT]){
	for (int i = 0; i < REMOTE_EXPRESS_NUM_KEYS_COUNT; i++){
		setNum(i, codes[i]);
	}
}

void RemoteExpress::onnumpress(void (*cb) (int num)){
	numkeyCB = cb;
}

void RemoteExpress::onnumhold(void (*cb) (int num, unsigned long microsSincePress)){
	onnumhold(0, cb);
}

void RemoteExpress::onnumhold(unsigned long everyMicro, void (*cb) (int num, unsigned long microsSincePress)){
	numkeyHoldEveryMicros = everyMicro;
	numkeyHoldCB = cb;
}

int RemoteExpress::lookupNumkey(unsigned long key){
	for (int i = 0; i < REMOTE_EXPRESS_NUM_KEYS_COUNT; i++){
		if (numkeys[i] == key)
			return i;
	}

	return -1;
}

int RemoteExpress::lookupKey(unsigned long key){
	for (int i = 0; i < lookupKeysSize; i++){
		if (lookupKeys[i] == key)
			return i;
	}

	return -1;
}
int RemoteExpress::addLookupKey(unsigned long key){
	if (lookupKeysSize >= REMOTE_EXPRESS_KEYS_SIZE)
		--lookupKeysSize; //use the last slot if we're out

	lookupKeys[lookupKeysSize] = key;
	lookupCB[lookupKeysSize] = defaultKeyCB;
	lookupHoldCB[lookupKeysSize] = defaultKeyHoldCB;
	lookupHoldEveryMicros[lookupKeysSize] = 0x10000;

	return lookupKeysSize++;
}
