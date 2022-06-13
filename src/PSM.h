#ifndef PSM_h
#define PSM_h

#include "Arduino.h"

class PSM
{
public:
	PSM(unsigned char sensePin, unsigned char controlPin, unsigned int range, int mode = RISING);

	void set(unsigned int value);

	long getCounter();
	void resetCounter();

	void stopAfter(long counter);
    
    unsigned int cps();
	unsigned long getLastMillis();

private:
	static inline void onInterrupt();
	void calculateSkip();
	void updateControl();

	unsigned char _sensePin;
	unsigned char _controlPin;
	unsigned int _range;
	volatile unsigned int _value;
	volatile unsigned int _a;
	volatile bool _skip;
	volatile long _counter;
	volatile long _stopAfter;
	volatile unsigned long _lastMillis;
};

extern PSM *_thePSM;

#endif
