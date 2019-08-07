/*****************************************************************************
* 
*   Copyright Photonic Ltd. (C) 2012
*   All rights reserved.
*
*   Module Name:    PD.h
*   Author:
*   Description:    PD API. 
*   
*   Author:  Huang Yu Qi
*   Date: 04/01/2012
*
******************************************************************************/

#ifndef PD_H
#define PD_H

#include "Typedefs.h"
#include "Gdata.h"


/// Basic APIs
void InitPD();

float GetPower();
long  GetCurADC();
void  SetRefPower( float refPower );
float GetRefPower();

tUINT8 	PDOnSampling( tUINT32 argVal );

void WriteCoeffToFlash();

#endif  // end of PD_H