#ifndef PSM_h
#define PSM_h

#include "Arduino.h"

class PSM
{
public:
  PSM(unsigned char sensePin, unsigned char controlPin, unsigned int range, int mode = RISING, unsigned char divider = 1, unsigned char interruptMinTimeDiff = 0);

  void initTimer(uint32_t freq);

  void set(unsigned int value);

  long getCounter(void);
  void resetCounter(void);

  void stopAfter(long counter);

  unsigned int cps(void);
  unsigned long getLastMillis(void);

  unsigned char getDivider(void);
  void setDivider(unsigned char divider = 1);
  void shiftDividerCounter(char value = 1);

private:
  static inline void onInterrupt(void);
  static inline void calculateSkipFromZC(void);
  static inline void calculateSkipFromTimer(void);
  void calculateSkip(void);
  void updateControl(void);

  unsigned char _sensePin;
  unsigned char _controlPin;
  unsigned int _range;
  unsigned char _divider;
  unsigned char _dividerCounter = 1;
  unsigned char _interruptMinTimeDiff;
  volatile unsigned int _value;
  volatile unsigned int _a;
  volatile bool _skip;
  volatile long _counter;
  volatile long _stopAfter;
  volatile unsigned long _lastMillis;

  bool _intervalTimerInitialized;
  HardwareTimer* _psmIntervalTimer;
};

extern PSM* _thePSM;

#endif
