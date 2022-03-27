#include "Arduino.h"
#include "PSM.h"

PSM *_thePSM;

PSM::PSM(unsigned char sensePin, unsigned char controlPin, unsigned int range, int mode)
{
	_thePSM = this;

	pinMode(sensePin, INPUT_PULLUP);
	PSM::_sensePin = sensePin;

	pinMode(controlPin, OUTPUT);
	PSM::_controlPin = controlPin;

	attachInterrupt(digitalPinToInterrupt(PSM::_sensePin), onInterrupt, mode);

	PSM::_range = range;
}

void PSM::onInterrupt()
{
	_thePSM->calculateSkip();
}

void PSM::set(unsigned int value)
{
	if (value < PSM::_range)
	{
		PSM::_value = value;
	}
	else
	{
		PSM::_value = PSM::_range;
	}
}

long PSM::getCounter()
{
	return PSM::_counter;
}

void PSM::resetCounter()
{
	PSM::_counter = 0;
}

void PSM::stopAfter(long counter)
{
	PSM::_stopAfter = counter;
}

void PSM::calculateSkip()
{
	PSM::_lastMillis = millis();

	PSM::_a += PSM::_value;

	if (PSM::_a >= PSM::_range)
	{
		PSM::_a -= PSM::_range;
		PSM::_skip = false;
	}
	else
	{
		PSM::_skip = true;
	}

	if (PSM::_a > PSM::_range)
	{
		PSM::_a = 0;
		PSM::_skip = false;
	}

	if (!PSM::_skip)
	{
		PSM::_counter++;
	}

	if (!PSM::_skip 
		&& PSM::_stopAfter > 0
		&& PSM::_counter > PSM::_stopAfter)
	{
		PSM::_skip = true;
	}

	updateControl();
}

void PSM::updateControl()
{
	if (PSM::_skip)
	{
		digitalWrite(PSM::_controlPin, LOW);
	}
	else
	{
		digitalWrite(PSM::_controlPin, HIGH);
	}
}

unsigned int PSM::cps()
{
    unsigned int range = PSM::_range;
    unsigned int value = PSM::_value;
    
    PSM::_range = 0xFFFF;
    PSM::_value = 1;
    PSM::_a = 0;
    
    unsigned long stopAt = millis() + 1000;  
    
    while (millis() < stopAt)
    {
        delay(0);
    }
    
    unsigned int result = PSM::_a;
    
    PSM::_range = range;
    PSM::_value = value;
    PSM::_a = 0;
    
    return result;
}

long PSM::getLastMillis()
{
	return PSM::_lastMillis;
}
