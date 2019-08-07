/*****************************************************************************
* 
*   Copyright (C) Brook 2011-2012
*   All rights reserved.
*
*   Module Name:    PD.c
*   Author:			Huang Yuqi
*   Description:    READ PD 
*   
*   Date: 9/11/2011
*
   	//                           Vref (mV)
   	//   measurement (mV) =   --------------- * result (bits)
   	//                       (2^24)-1 (bits)
   	//
   	//   measurement (mV) =  result (bits) / ((2^24)-1 (bits) / Vref (mV))
   	//
   	//
   	//   With a Vref (mV) of 2500:
   	//
   	//   measurement (mV) =  result (bits) / ((2^24)-1 / 2500)
   	//
   	//   measurement (mV) =  result (bits) / ((2^24)-1 / 2500)
   	//
   	//   measurement (mV) =  result (bits) / (16777215 / 2500)
   	//
   	//   measurement (mV) =  result (bits) / (6710)

//   	mV = PDHex[PD1] / 6710;        // Because of bounds issues, this
                                       	// calculation has been manipulated as
                                       	// shown above
                                       	// (i.e. 2500 (VREF) * 2^24 (ADC result)
                                       	// is greater than 2^32)
******************************************************************************/


#include <absacc.h>
#include <stdlib.h>
#include <math.h>
#include <intrins.h>	/// for write/read flash

#include "C8051F350.h"
#include "Gdata.h"
#include "PD.h"

/*=============================================
 *
 *  x = (ADC-ADCoffset) => current = Ka * x + Ba (Ka Unit: 1/mA)
 *  Power = Kc * current + Bc.
 *  
 */

#define POWER_DB_MIN -61
#define MAX_ADC_THRESHOLD 13420000L //16770000 //0xffffff
#define MIN_ADC_THRESHOLD 67100L //0x0000ff

sbit PD_RANGE1 = 0x93;	//P1.4
sbit PD_RANGE2 = 0x92;	//P1.5


float			idata sPower;
float			idata sdBmBase; /// for calculate dB value.
unsigned long	idata sCurADC;

/// Range1~4 <=> P1.3~P1.0
/// Range: 1k, 100k, 10m,100m
tUINT8 sRangeMask[RANGE_MAX] = 
{
	0x07, 0x0d, 0x0b, 0x0e
};

tUINT16 xdata sRangeThresholdMax[RANGE_MAX] = 
{
	1670,
	1670,
	1670, 
	1670
//	1542,
//	1432,
//	1342, 
//	1342
};
tUINT16  xdata sRangeThresholdMin[RANGE_MAX] = 
{
	1670,
	1670,
	16700,
	671
};


#define MaxThreshold(idx) (sRangeThresholdMax[idx]*10000L)
#define MinThreshold(idx) (sRangeThresholdMin[idx]*100L)

#define IsRangeLowest( ) 	(sRangeCoeff.myCurRange == (RANGE_MAX-1))
#define IsRangeHighest( ) 	(sRangeCoeff.myCurRange == 0)
void SwitchTIA( tUINT8 range )
{
	P1 = (P1 & 0xf0) | sRangeMask[range];
	sRangeCoeff.myCurRange = range;
}


/// switch range if necessary.
tUINT8 	SwitchRange( tUINT32 argVal )
{
	tUINT8 aState = PowerValid;

	if ( argVal >= MaxThreshold(sRangeCoeff.myCurRange) )
	//if ( argVal >= MAX_ADC_THRESHOLD )
	{
		if ( IsRangeHighest() )
			aState = PowerHigh;
		else
		{
			SwitchTIA(sRangeCoeff.myCurRange-1);
			aState = PowerInvalid;
		}
	}
	else if ( argVal < MinThreshold(sRangeCoeff.myCurRange) )
	//else if ( argVal < MIN_ADC_THRESHOLD )
	{
		if ( IsRangeLowest() )
			aState = PowerLow;
		else
		{
			SwitchTIA(sRangeCoeff.myCurRange+1);
			aState = PowerInvalid;
		}
	}

	return aState;
}

/// ADC = current * k + b
void InitPD()
{
	unsigned char i;

	{
		sLamdaCoeff[0].k = LAMDA_COEFF_1; 	/// 1310
		sLamdaCoeff[1].k = LAMDA_COEFF_2;   /// 1490
		sLamdaCoeff[2].k = LAMDA_COEFF_3;   /// 1550
		sLamdaCoeff[3].k = LAMDA_COEFF_4;   /// 1625, temp

		sRangeCoeff.myCurRange = 0;
	
		for ( i = 0; i < LAMDA_MAX; i++ )
		{
			sLamdaCoeff[i].b = 0.0;		
			sLamdaCoeff[i].darkCurrent = 0.0;		
		}
		
		sRangeCoeff.myRangeCoeff[0].k = RANGE_COEFF_K1;
		sRangeCoeff.myRangeCoeff[0].b = RANGE_COEFF_B1;
		sRangeCoeff.myRangeCoeff[0].offsetADC = RANGE_COEFF_ZERO_ADC1;

		sRangeCoeff.myRangeCoeff[1].k = RANGE_COEFF_K2;
		sRangeCoeff.myRangeCoeff[1].b = RANGE_COEFF_B2;
		sRangeCoeff.myRangeCoeff[1].offsetADC = RANGE_COEFF_ZERO_ADC2;

		sRangeCoeff.myRangeCoeff[2].k = RANGE_COEFF_K3;
		sRangeCoeff.myRangeCoeff[2].b = RANGE_COEFF_B3;
		sRangeCoeff.myRangeCoeff[2].offsetADC = RANGE_COEFF_ZERO_ADC3;

		sRangeCoeff.myRangeCoeff[3].k = RANGE_COEFF_K4;
		sRangeCoeff.myRangeCoeff[3].b = RANGE_COEFF_B4;
		sRangeCoeff.myRangeCoeff[3].offsetADC = RANGE_COEFF_ZERO_ADC4;
	}

	SwitchTIA(0);

	sPower = 0;
	sdBmBase = 0.0;
	sCurADC = 0;
}



tUINT8 	PDOnSampling( tUINT32 argVal )
{
	double current = 0;
	tINT32 adc;

	tUINT8 aPowerState = SwitchRange( argVal );
	if ( aPowerState == PowerInvalid ) /// here invalid means range adjusted, so ADC need re-sampling.
	{
		return PowerSampling;
	}

	sCurADC = argVal;

#if 0
	/// Power(uW) = adc * k + b
	adc = argVal - sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].offsetADC;
	if ( adc > MAX_ADC_THRESHOLD )
		return PowerHigh;

	if ( adc < MIN_ADC_THRESHOLD )
		return PowerLow;

	sPower = sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].k * adc + sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].b;
	if ( sPower <= 0.01 )
		return PowerInvalid;

#else	

	/// ADC = k x I + b
	adc = argVal - sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].offsetADC;
	if ( adc >= MaxThreshold(sRangeCoeff.myCurRange) )
		return PowerHigh;

	if ( adc < MinThreshold(sRangeCoeff.myCurRange) )
		return PowerLow;

	if ( abs(sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].k ) > 0.000001 ) ///< k!= 0
	{
		current = (adc - sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].b) / sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].k; 
	}
	else
	{
		return PowerInvalid;
	}
	
	//if ( abs(sLamdaCoeff[gSystemState.myCurLamda].k) < 0.0001 )
	{
		sPower = current * sLamdaCoeff[gSystemState.myCurLamda].k / 1000.0;

		if ( sPower <= 0.0000001 )
			return PowerLow;
	}
	//else
	//{
	//	return PowerInvalid;
	//}
#endif
	
	return PowerValid;
}

float GetPower()
{
	return sPower;
}

long GetCurADC()
{
	return sCurADC;
}

void SetRefPower( float refPower )
{
	sdBmBase = 10*log10( refPower ); // convert to db value
}

float GetRefPower()
{
	return sdBmBase;
}