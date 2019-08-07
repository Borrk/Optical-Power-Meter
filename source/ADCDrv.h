/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Display.h
*   Author:
*   Description:    ADC Driver API declaration. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/22/2012
*
******************************************************************************/

#ifndef ADCDRV_H
#define ADCDRV_H

#include "Typedefs.h"

/// ADC0CF: ADC0 Configuration
typedef enum {
	ADC_FILTER_SINC3 = 0,
	ADC_FILTER_FAST
}eADCFilter;

///  ADC0MD: ADC0 Mode
typedef enum {
	ADC_MODE_Idle = 0,
	ADC_MODE_Calibrate_Full = 1,
	ADC_MODE_Conversion_Single = 2,
	ADC_MODE_Conversion_Continuous = 3,
	ADC_MODE_Internal_Calibrate_Offset = 4,
	ADC_MODE_Internal_Calibrate_Gain = 5,
	ADC_MODE_System_Calibrate_Offset = 6,
	ADC_MODE_System_Calibrate_Gain = 7
}eADCMode;

/// ADC0MUX: ADC0 Analog Multiplexer Control
typedef enum {
   ADC_AIN0_0 = 0,
   ADC_AIN0_1 = 1,
   ADC_AIN0_2 = 2,
   ADC_AIN0_3 = 3,
   ADC_AIN0_4 = 4,
   ADC_AIN0_5 = 5,
   ADC_AIN0_6 = 6,
   ADC_AIN0_7 = 7,
   ADC_AIN0_TEMPE = 15,
   ADC_AGND   = 8
} eADCMux;

typedef void  (*ADCProc)( );


/// Basic APIs
void vADC0DrvInitHW();

/// Application APIs
void vInitADC0Drv( );

void vResetCompleteFlag( );  // Clear completation flag
void vADC0EnableInt( eBOOL argEnable );
void vSetADC0MUX( eADCMux aMuxP, eADCMux  aMuxN );  // Set ADC0 MUX

void vSetADC0Filter(eADCFilter filter ); // Set ADC0 filter.
eADCFilter vGetADC0Filter();

void vADC0SetMode(eADCMode mode);  // Set ADC work mode
eADCMode vADC0GetMode();  // Get ADC work mode

void vSetADCCallback( ADCProc proc );

#endif  // end of ADCDRV_H