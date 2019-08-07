/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Battary.h
*   Author:
*   Description:    Battary API declaration. 
*   
*   Author:  Huang Yu Qi
*   Date: 04/01/2012
*
******************************************************************************/

#ifndef BATTARY_H
#define BATTARY_H

#include "Typedefs.h"

/// HW APIs
void  	BattaryInitHW();
void  	InitBattary( );


/// Application APIs
float 	GetBattaryPower();
void 	ResetBattaryParam();
tUINT8 	BattOnSampling( tUINT32 argVal );


#endif  // end of BATTARY_H