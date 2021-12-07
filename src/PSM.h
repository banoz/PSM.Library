#ifndef PSM_h
#define PSM_h

#include "Arduino.h"

class PSM
{
public:
	PSM(unsigned char sensePin, unsigned char controlPin, unsigned int range);

	void set(unsigned int value);

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
};

extern PSM *_thePSM;

#endif
