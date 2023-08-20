# PSM.Library - Arduino library for PSM vibe pump control. 
_based on https://github.com/banoz/PSM_

__feature/timer__ branch addresses the TRIAC failure to close by sending two control impulses instead of one.

_signals on the graphs top-bottom:_
- _AC voltage_
- _AC zero-cross detector signal_
- _TRAIC control signal_
- _TRIAC output voltage_

This is the case when the TRIAC fails to close during the negative half-wave that drives the vibe pump despite the control signal being turned low. It happens when the control signal is turned low during the positive-to-negative half-wave transition. 

![TRIAC failed to close](https://github.com/banoz/banoz.github.io/blob/main/repository/img/PSM.Library/feature_timer/close_failed.png)

This is the case when the TRIAC closes successfully during the negative half-wave. It happens when the control signal is turned low during the negative-to-positive half-wave transition. 

![TRIAC successfully closes](https://github.com/banoz/banoz.github.io/blob/main/repository/img/PSM.Library/feature_timer/closed.png)

This is how the signals look after two short control impulses sent instead of a long one.

Positive-to-negative AC transition

![TRIAC successfully closes at positive-to-negative AC transition](https://github.com/banoz/banoz.github.io/blob/main/repository/img/PSM.Library/feature_timer/p1.png)

Negative-to-positive AC transition

![TRIAC successfully closes at negative-to-positive  AC transition](https://github.com/banoz/banoz.github.io/blob/main/repository/img/PSM.Library/feature_timer/p2.png)

In both cases the TRIAC successfully closes for the negative AC half-wave when the control signal is low

![TRIAC successfully closes](https://github.com/banoz/banoz.github.io/blob/main/repository/img/PSM.Library/feature_timer/p12.png)

The library worked successfully for 50Hz- and 60Hz AC supplies with a pulse width equal to 2/3 of AC half-wave width.
