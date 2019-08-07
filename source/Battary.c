/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Battary.c
*   Author:
*   Description:    Battary API. 
*   
*   Author:  Huang Yu Qi
*   Date: 04/01/2012
*
******************************************************************************/

#include <intrins.h>

#include "Battary.h"

float  idata  sBattPower = 2.5;	// Battary power.

void 	InitBattary( )
{
	ResetBattaryParam();
}

float GetBattPower()
{
	return sBattPower;
}

void ResetBattaryParam()
{
	sBattPower = 2.5;
}

tUINT8 OnSampling( tUINT32 argVal )
{
	sBattPower = (argVal / 6710.0) / 1000;

	return 1;
}