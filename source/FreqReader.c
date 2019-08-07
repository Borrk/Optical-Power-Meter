/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    FreqReader.c
*   Author:
*   Description:    Laser Frequency reader API. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/22/2012
*
******************************************************************************/

#include <absacc.h>
#include <stdlib.h>
#include <math.h>

#include "C8051F350.h"
#include "FreqReader.h"

xdata tUINT16  sFreqCnt = 0;

/*-----------------------------------------------------------------------------------*\
 *
 *  HardWare initilization section.
 *
 *  Used HW resource:
 *  - Comparator0 as frequency counter, positive:P1.4, negative:P1.5.
 *  - P1.4 as input frequency source
 *  - P1.5 as reference voltage.
 *
\*-----------------------------------------------------------------------------------*/

///  CPT0CN: Comparator0 Control
#define CP0EN 				0x80

#define CP0HYP1_5mv			0x04
#define CP0HYP1_10mv		0x08
#define CP0HYP1_20mv		0x0C

#define CP0HYN1_5mv			0x01
#define CP0HYN1_10mv		0x02
#define CP0HYN1_20mv		0x03

///  CPT0MD: Comparator0 Mode Selection
#define CP0RIE				0x20
#define CP0FIE				0x10
#define CP0MD_Fastest		0x00
#define CP0MD_Faster		0x01
#define CP0MD_Lower			0x02
#define CP0MD_Lowest		0x03

/// CPT0MX: Comparator0 MUX Selection 
///  IDA0CN: IDA0 Control
#define IDA0EN				0x80
#define IDA0CM_OnIDA0		0x70
#define IDA0CSC_IDA0		0x00
#define IDA0OMD_Dot25mA 0x00

/// EIE1: Extended Interrupt Enable 1
#define ECP0				0x20

#define CPT0CN_default (CP0EN | CP0HYP1_20mv | CP0HYN1_20mv)

//-----------------------------------------------------------------------------
// Initilize HW(Onchip IDAC0 Init).
//-----------------------------------------------------------------------------
void FreqReaderInitHW()
{
	/// Config CP0
	CPT0CN = CPT0CN_default;	// 0x8f
	CPT0MD = CP0RIE | CP0MD_Fastest; // 0x20;
	CPT0MX = 0x66; ///< positive:P1.4, negative:P1.5

	/// use external ref. vol

	/// Conig P1 & P1SKIP
  	P1MDIN  &= 0xcf; // P1.4, P1.5 as analog input.
	
	//no need to config P1MDOUT because P1.5&P1.4 configed as analog input.

	P1SKIP |= 0x30;	/// P1.4, P1.5 should be skipped.
	
	/// Enable CP0 interrupt.
	EIE1 |= ECP0;	
}

void FreqReaderEnable( eBOOL argEnable )
{
	tUINT8 tmp = CPT0CN;
	
	if ( argEnable )
	{
		CPT0CN = tmp | CP0EN;
	}
	else
	{
		CPT0CN = tmp & ~CP0EN;
	}
}

//-----------------------------------------------------------------------------
// Comparator 0 interrupt
//-----------------------------------------------------------------------------
void vInt_CPT0(void) interrupt 12
{
  CPT0CN = CPT0CN_default; /// clear CP0RIF, CP0RIF
	sFreqCnt++;
}


/*-----------------------------------------------------------------------------------*\
 *
 *  API section.
 *
\*-----------------------------------------------------------------------------------*/

void InitFreqReader()
{
	sFreqCnt = 0;
}

void ResetFreqReaderParam()
{
	sFreqCnt = 0;
}

tUINT16 GetCurFreqCount()
{
	return sFreqCnt;
}