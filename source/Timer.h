/*****************************************************************************
* 
*   Copyright Photonic Ltd. (C) 2012
*   All rights reserved.
*
*   Module Name:    Timer.h
*   Author:
*   Description:    Time(system tick) and Timer. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/22/2012
*
******************************************************************************/

#ifndef TIMER_H
#define TIMER_H

#include "Typedefs.h"

/// Basic APIs
void TimerInitHW();

/// Application APIs
void InitTimers();
void ResetTimerParam();

eBOOL tTimeout(tUINT16 wInterval, tUINT16 *pwTime);
tUINT16 wGetTimer(void);
void vWait_us( tUINT16 bCnt );
void vWait_ms( tUINT16 bCnt);


#endif  // end of TIMER_H