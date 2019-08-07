/*****************************************************************************
* 
*   Copyright Photonic Ltd. (C) 2012
*   All rights reserved.
*
*   Module Name:    Task.h
*   Author:
*   Description:    Task management component. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/27/2012
*
******************************************************************************/

#ifndef TASK_H
#define TASK_H

#include "Typedefs.h"


/// ADC channels 
#define ChKey1   		ADC_AIN0_0
#define ChKey2   		ADC_AIN0_1
#define ChKey3   		ADC_AIN0_2
#define ChKey4   		ADC_AIN0_3
#define ChBatt   		ADC_AIN0_5
#define ChPower   		ADC_AIN0_6
   
typedef enum { TaskPower, TaskBatt, TaskKey }eTaskID;
typedef struct 
{
	tUINT8  uTask;
	tUINT8 uFlag;
	tUINT8 uChannel;
}stTaskInfo;

void SendRangeInfo();

/// Application APIs
void  InitTask();
void  StartTasks( eBOOL bStart );

tUINT8 FreqProc();
tUINT8 FuncKeyProc();

void SetADC0Channel( tUINT8 ch );  // Set ADC0 MUX

void SwitchCalibrationMode( tUINT8 bOn );

void  OnSamplingDone( );

#endif  // end of TASK_H