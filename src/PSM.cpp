#include "PSM.h"

PSM* _thePSM;

PSM::PSM(unsigned char sensePin, unsigned char controlPin, unsigned int range, int mode, unsigned char divider, unsigned char interruptMinTimeDiff) {
  _thePSM = this;

  pinMode(sensePin, INPUT_PULLUP);
  PSM::_sensePin = sensePin;

  pinMode(controlPin, OUTPUT);
  PSM::_controlPin = controlPin;

  PSM::_divider = divider > 0 ? divider : 1;

  uint32_t interruptNum = digitalPinToInterrupt(PSM::_sensePin);

  if (interruptNum != NOT_AN_INTERRUPT) {
    attachInterrupt(interruptNum, onZCInterrupt, mode);
  }

  PSM::_range = range;
  PSM::_interruptMinTimeDiff = interruptMinTimeDiff;
}

void onPSMInterrupt() __attribute__((weak));
void onPSMInterrupt() {}

void PSM::onZCInterrupt(void) {
  if (_thePSM->_interruptMinTimeDiff > 0 && millis() - _thePSM->_interruptMinTimeDiff < _thePSM->_lastMillis) {
    if (millis() >= _thePSM->_lastMillis) {
      return;
    }
  }

  _thePSM->_lastMillis = millis();

  onPSMInterrupt();

  _thePSM->calculateSkipFromZC();

  if (_thePSM->_psmIntervalTimerInitialized) {
    _thePSM->_psmIntervalTimer->setCount(0);
    _thePSM->_psmIntervalTimer->resume();
  }
}

void PSM::onPSMTimerInterrupt(void) {
  _thePSM->_psmIntervalTimer->pause();
  _thePSM->updateControl(true);
}

void PSM::set(unsigned int value) {
  if (value < PSM::_range) {
    PSM::_value = value;
  }
  else {
    PSM::_value = PSM::_range;
  }
}

long PSM::getCounter(void) {
  return PSM::_counter;
}

void PSM::resetCounter(void) {
  PSM::_counter = 0;
}

void PSM::stopAfter(long counter) {
  PSM::_stopAfter = counter;
}

void PSM::calculateSkipFromZC(void) {
  if (_thePSM->_dividerCounter >= _thePSM->_divider) {
    _thePSM->_dividerCounter -= _thePSM->_divider;
    _thePSM->calculateSkip();
  }
  else {
    _thePSM->_dividerCounter++;
  }
  _thePSM->updateControl(false);
}

void PSM::calculateSkip(void) {
  PSM::_a += PSM::_value;

  if (PSM::_a >= PSM::_range) {
    PSM::_a -= PSM::_range;
    PSM::_skip = false;
  }
  else {
    PSM::_skip = true;
  }

  if (PSM::_a > PSM::_range) {
    PSM::_a = 0;
    PSM::_skip = false;
  }

  if (!PSM::_skip) {
    PSM::_counter++;
  }

  if (!PSM::_skip
    && PSM::_stopAfter > 0
    && PSM::_counter > PSM::_stopAfter) {
    PSM::_skip = true;
  }
}

void PSM::updateControl(bool forceDisable) {
  if (forceDisable || PSM::_skip) {
    digitalWrite(PSM::_controlPin, LOW);
  }
  else {
    digitalWrite(PSM::_controlPin, HIGH);
  }
}

unsigned int PSM::cps(void) {
  unsigned int range = PSM::_range;
  unsigned int value = PSM::_value;
  unsigned char divider = PSM::_divider;

  PSM::_range = 0xFFFF;
  PSM::_value = 1;
  PSM::_a = 0;
  PSM::_divider = 1;
  PSM::_skip = true;

  unsigned long stopAt = millis() + 1000;

  while (millis() < stopAt) {
    delay(0);
  }

  unsigned int result = PSM::_a;

  PSM::_range = range;
  PSM::_value = value;
  PSM::_a = 0;
  PSM::_divider = divider;

  return result;
}

unsigned long PSM::getLastMillis(void) {
  return PSM::_lastMillis;
}

unsigned char PSM::getDivider(void) {
  return PSM::_divider;
}

void PSM::setDivider(unsigned char divider) {
  PSM::_divider = divider > 0 ? divider : 1;
}

void PSM::shiftDividerCounter(char value) {
  PSM::_dividerCounter += value;
}

void PSM::initTimer(uint8_t freq, TIM_TypeDef* timerInstance) {
  uint32_t us = 6000u;
  if (freq > 55u) {
    us = 5500u;
  }

  PSM::_psmIntervalTimer = new HardwareTimer(timerInstance);
  PSM::_psmIntervalTimer->setOverflow(us, MICROSEC_FORMAT);
  PSM::_psmIntervalTimer->setInterruptPriority(0, 0);
  PSM::_psmIntervalTimer->attachInterrupt(onPSMTimerInterrupt);

  PSM::_psmIntervalTimerInitialized = true;
}
