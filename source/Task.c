#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Hw.h"
#include "Gdata.h"
#include "FakeFunc.h"

#include "Task.h"
#include "KeyDrv.h"
#include "ADCDrv.h"
#include "Timer.h"
#include "FreqReader.h"
#include "Display.h"
#include "Battary.h"
#include "PD.h"

#define  BATT_LOW  1500

// support user adjust the displayed dBm value
#define  POWER_SUPPORT_USER_OFFSET 

#ifdef  POWER_SUPPORT_USER_OFFSET
tUINT8  sUseOffset = 0;
float   sOffsetPower = 0;
#endif

tINT8 sCurTask = 0;
stTaskInfo xdata sScheduleTable[] =
{ 
	TaskPower, 0,			ChPower,
	TaskBatt, 0,			ChBatt,
//	TaskKey, KEY_MODE,		ChKeyMode,
	TaskKey, KEY_1,			ChKey1,
//	TaskKey, KEY_2,			ChKey2,
//	TaskKey, KEY_3,			ChKey3,
	TaskKey, KEY_4,			ChKey4
};

extern void vInitICReg(void);
extern void InitUart(void);

eBOOL   InitSystemState();

void   nShowLeds();
void   nSwitchLamda( tUINT8 lamda );
tUINT8 PowerProc( );
tUINT8 BattProc( );
tUINT8 KeyProc( );

tUINT8  IsUpdateNow() { return (gSystemState.myBattState != BattaryLow); }

void  InitTask()
{
	sCurTask = 0;

    vInitICReg();
	InitScreen();
//	InitBattary();
//	InitSpeaker();

	InitSystemState();

#ifdef HAS_UART
	InitUart();
#endif

	/// Init PD 
	InitPD();
	InitFreqReader();
         
	nShowLeds();

	/// Set ADC callbak
	vSetADCCallback( OnSamplingDone);	
}


eBOOL   InitSystemState()
{
    gSystemState.myBattState = BattaryNormal;
    gSystemState.myCurLamda  = 0;
    gSystemState.myMeasureMode = MeasureModePower; //MeasureModeFreqOur;
    gSystemState.myPowerUnit   = PowerUnitdBm;
    gSystemState.myPowerState  = PowerInvalid;

	return TRUE;
}


void  StartTasks( eBOOL bStart )
{
	/// Start Sampling
	if ( bStart )
	{
		sCurTask = 0;
		SetADC0Channel( sScheduleTable[sCurTask].uChannel );
		vWait_ms(20);
		vADC0EnableInt( TRUE );
		vResetCompleteFlag();
		vADC0SetMode( ADC_MODE_Conversion_Single );	
		nSwitchLamda( gSystemState.myCurLamda );
	}
	else
	{
		vADC0SetMode( ADC_MODE_Idle );	
	}
}

void  OnSamplingDone( )
{
  	LONGDATA data rawValue;

	if ( AD0BUSY )
		vWait_ms(40);

	if ( !AD0INT ) 
		return;

	rawValue.Byte[Byte3] = 0x00;
	rawValue.Byte[Byte2] = (unsigned char)ADC0H;
	rawValue.Byte[Byte1] = (unsigned char)ADC0M;
	rawValue.Byte[Byte0] = (unsigned char)ADC0L;	
	
	/// Notify current task
	argVal = rawValue.result;
	argValFlag = sScheduleTable[sCurTask].uFlag;
	switch ( sScheduleTable[sCurTask].uTask )
	{
	case  TaskPower:
		PowerProc( ); //rawValue.result, sScheduleTable[sCurTask].uFlag  );
		break;
	
	case TaskBatt:
		BattProc( ); //rawValue.result, sScheduleTable[sCurTask].uFlag  );
		break;
		
	case TaskKey:
		KeyProc( ); //rawValue.result, sScheduleTable[sCurTask].uFlag  );
		break;
	}
	
	/// Switch task
	if ( ++sCurTask > sizeof(sScheduleTable) / sizeof(sScheduleTable[0]) )
	{
		sCurTask = 0;	
	}
	
	/// Start new Sampling
	SetADC0Channel( sScheduleTable[sCurTask].uChannel );
	vWait_ms(20);
	vResetCompleteFlag();
	vADC0SetMode( ADC_MODE_Conversion_Single );	
}


float myround( double x )
{
	if( x > 0.0 )
	    return (float)( x + 0.5 );
	else
	    return (float)( x - 0.5 );
}


/// Power Proc
#define DISPLAY_PWR_MIN	-60
void nShowInvalidPower( tUINT8 state, tUINT8 bUpdateLed )
{
	LedOff( LED_SIGNAL, 0 ); /// LED as signal bit
	DisplayLED( DIG_1, state, bUpdateLed );
}

void nShowPower()
{
	float power = 0;
	tUINT8 aUpdateLed = IsUpdateNow();

	/// if in sampling, don't update the screen
	if ( gSystemState.myPowerState == PowerSampling )
	{
		return;
	}

	DisplayLED( DIG_SIGNAL, CHAR_OFF, 0 );
	DisplayLED( DIG_1, 		CHAR_OFF, 0 );
	DisplayLED( DIG_10, 	CHAR_OFF, 0 );

	switch ( gSystemState.myPowerState )
	{
	case PowerInvalid:
		nShowInvalidPower( CHAR_OFF, aUpdateLed );
		break;
	case PowerLow:
		nShowInvalidPower( CHAR_LO, aUpdateLed );
		break;
	case PowerHigh:
		nShowInvalidPower( CHAR_HI, aUpdateLed );
		break;

	case PowerValid:
		power = GetPower();  /// unit: uW
		power = 10*log10( power );

		if ( gSystemState.myPowerUnit == MeasureModedBm )
		{						
#ifdef  POWER_SUPPORT_USER_OFFSET
			if ( sUseOffset == 1 ) 
				power += sOffsetPower;
#endif				
		}
		else
		{
			power = power - GetRefPower(); //10*log10( power/10000 ); /// temp 
		}

		if  ( power > DISPLAY_PWR_MIN )
			DisplayFloat( power, aUpdateLed ); 
		else
		{
			nShowInvalidPower( CHAR_LO, aUpdateLed );
		}

		break;
	}
}

void nShowLeds()
{
	tUINT8 aUpdatedLed = IsUpdateNow();

	LedOff( LED_ALL, 0 );
	
	/// Battary LED
	//if ( gSystemState.myBattState == BattaryLow )
	//{
	//	LedOff( LED_BATT, 0 );
	//}
	//else
	//{
	//	LedOn( LED_BATT, 0 );
	//}

	/// Power Unit LED
	if ( gSystemState.myPowerUnit == MeasureModedBm )
	{	
		LedOn( LED_UNIT_dBm, aUpdatedLed );
	}
	else
	{
		LedOn( LED_UNIT_dB, aUpdatedLed );
	}	
}

tUINT8 PowerProc( )
{
	gSystemState.myPowerState = PDOnSampling( argVal );
	
	if ( gSystemState.myMeasureMode != MeasureModePower )
	{
		return 1;
	}

	nShowPower();

	return 1;
}

/// Battary Proc
tUINT8 BattProc( ) 
{
	float power = argVal / 6710.0 * 2; 

	//if ( abs(sPrePower - power) < 100 )
	{
		if ( 10 < power && power < BATT_LOW )
		{
			SetDisplayType(Disp_DynamicDEC);
			gSystemState.myBattState = BattaryLow ;
		}
		else
		{
			SetDisplayType(Disp_Static);
			gSystemState.myBattState = BattaryNormal;
		}
	}

	//sPrePower = power;

	return 1;
}

/// Key Proc
tUINT8 KeyProc( )
{
	tUINT8 aPressedKey = 0;
	tUINT8 aUseOffset = 0;
	
	KeyDrvOnSampling( argVal, argValFlag );

	/// Check  key
	/// key1: calibration power switch
	/// key2-3: not used.
	/// key4: frequency mode.
	if (  vGetKeyStatus( KEY_1 ) )
	{
		sUseOffset = 1;

		/// switch to dBm Unit
		gSystemState.myPowerUnit = MeasureModedBm;
	
		/// Update Unit light
		LedOn( LED_UNIT_dBm, 0 );
		LedOff( LED_UNIT_dB, 1 );			
	}
	else
	{
		sUseOffset = 0;
	}

	if (  vGetKeyStatus( KEY_4 )    )
	{
		gSystemState.myMeasureMode = MeasureModeFreq;
	}
	else
	{
		gSystemState.myMeasureMode = MeasureModePower;
	}	
	
	return 1;
}

#ifdef HAS_FREQ 
/// For Freq. proc
tUINT16 sFreqElapse;
tINT8   sPreFreq = -1;

/// Frequency proc
tINT8 GetFreqId( tUINT16 freq )
{
	tINT8   aFreqID = -1;

	{
		if ( freq < 200 ) 
			aFreqID = -1;
		else if ( freq < 290 ) 
			aFreqID = 1;
		else if ( freq < 400 ) 
			aFreqID = 2;
		else if ( freq < 1200 )
			aFreqID = 3;
		else if ( freq < 2400 )
			aFreqID = 4;
	}

	return aFreqID;
}

tUINT8 FreqProc()
{
	tUINT16 aFreqCnt = 0;
	tINT8   aFreqID = 0;
	tINT16  aCurTick;
	tUINT8  aUpdate;

	aCurTick  = (wGetTimer() - sFreqElapse) * 2; /// convert to ms
	if ( aCurTick <= 0 )
		return 0;

	aFreqCnt = GetCurFreqCount();
	aFreqCnt = (tUINT16)(aFreqCnt *(1000.0 / aCurTick)+0.5);

	aFreqID = GetFreqId( aFreqCnt );

	if ( gSystemState.myMeasureMode == MeasureModeFreq )
	{
		if ( sPreFreq == 0 || sPreFreq == aFreqID )
		{

	   		aUpdate = IsUpdateNow();
			DisplayFreq( aFreqID, aUpdate );
		}
	}

	sPreFreq = aFreqID;

	ResetFreqReaderParam();
	sFreqElapse = wGetTimer();

	return 1;
}
#endif


void SetADC0Channel( tUINT8 ch )
{
	vSetADC0MUX( ch, ADC_AGND );
}

/*---------------------------------------------------------------------------
 *
 * Functional key monitor
 *
 *---------------------------------------------------------------------------*/
void nSwitchLamda( tUINT8 lamda )
{
	tUINT8  aUpdate;

	gSystemState.myCurLamda = lamda;
	if ( gSystemState.myCurLamda >= LAMDA_MAX )
		gSystemState.myCurLamda = 0;

	aUpdate = IsUpdateNow();

	LedOff( LED_LAMDA_ALL, 0 );
	if ( gSystemState.myCurLamda == 0 ) 
	{			
		LedOn( LED_LAMDA1, aUpdate );
	}
	else if ( gSystemState.myCurLamda == 1 )
	{			
		LedOn( LED_LAMDA2, aUpdate );
	}
	else if ( gSystemState.myCurLamda == 2 )
	{			
		LedOn( LED_LAMDA3, aUpdate );
	}
	else if ( gSystemState.myCurLamda == 3 )
	{			
		LedOn( LED_LAMDA4, aUpdate );
	}
}

/// this function should be call every 100ms
tUINT8 FuncKeyProc()
{
	static tUINT8 sKeyCntPower = 0;
	static tUINT8 sKeyCntUnit  = 0;
	static tUINT8 sKeyCntRef   = 0;

	tUINT8 aUpdatedLed = 0;

	/// Power/Ref key
	/// -Power/Ref key pressed longer than 10s, power off.
	/// -Power/Ref key pressed less than 10s,   set reference dBm.
	if ( KEY_F_POWER == 0 )
	{
		sKeyCntPower++;
		if ( sKeyCntPower > 30 )
		{
			/// display off
			vDisplayDrvEnable( FALSE );

			/// Power off
			KEY_F_POWER_EN = 1;
		}
	}
	else /// key released
	{
		if ( sKeyCntPower > 0 ) /// Set reference power dBm.
		{
			SetRefPower( GetPower() );
		}

		sKeyCntPower = 0;
	}

	/// Unit key
	if ( KEY_F_UNIT == 0 )
	{
		sKeyCntUnit++;
	}
	else /// key released or not pressed
	{
		/// released now, switch unit
		if ( sKeyCntUnit > 0 )
		{
			aUpdatedLed = IsUpdateNow();
#ifdef  POWER_SUPPORT_USER_OFFSET
			if ( sUseOffset == 1 ) ///
			{
				if ( sOffsetPower < 3 )
					sOffsetPower += 0.1;				
			}
			else
#endif
			{
				if ( gSystemState.myPowerUnit == MeasureModedB )
				{
					gSystemState.myPowerUnit = MeasureModedBm;
				
					/// Update Unit light
					LedOn( LED_UNIT_dBm, 0 );
					LedOff( LED_UNIT_dB, aUpdatedLed );
				}
				else
				{
					gSystemState.myPowerUnit = MeasureModedB;
				
					/// Update Unit light
					LedOn( LED_UNIT_dB, 0 );
					LedOff( LED_UNIT_dBm, aUpdatedLed );
				}
			}
		}

		sKeyCntUnit = 0;
	}

	/// Lamda key
	if ( KEY_F_LAMDA == 0 )
	{
		sKeyCntRef++;
	}
	else /// key released or not pressed
	{
		/// released now, switch unit
		if ( sKeyCntRef > 0 )
		{
#ifdef  POWER_SUPPORT_USER_OFFSET
			if ( sUseOffset == 1 ) ///
			{
				if ( sOffsetPower > -3 )
					sOffsetPower -= 0.1;				
			}
			else
#endif		
			{
				/// switch lamda
				nSwitchLamda( gSystemState.myCurLamda+1 );
			}
		}

		sKeyCntRef = 0;
	}

	return 1;
}


#ifdef HAS_UART
const char* xdata sLamdaStr[4] = 
{
	"1310 nm",
	"1490 nm",
	"1550 nm",
	"1625 nm"
};

void SendRangeInfo()
{
	float current = 0;
	char *pBuf = (char*)UART0_Tx_Buff;
	unsigned long adc = GetCurADC();


	pBuf[0] = 'R';
	pBuf[+1] = gSystemState.myCurLamda + '0';
	pBuf[2] = ' ';
	pBuf[3] = 0;
	UART0_Tx_Bytes = strlen(pBuf);
	pBuf = (char*)&UART0_Tx_Buff[UART0_Tx_Bytes];

	if ( adc > sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].offsetADC )
	{
		current = (adc-sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].offsetADC - sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].b) / sRangeCoeff.myRangeCoeff[sRangeCoeff.myCurRange].k;
		if ( current < 0 ) current = 0;
	}
	else
		current = 0;

	adc = GetCurADC();

	sprintf( pBuf, "ADC:%ld, I: %.3f", adc, current );

	UART0_Tx_Bytes = strlen(UART0_Tx_Buff);

	UART0_Tx_Buff[UART0_Tx_Bytes] = '\n';
	UART0_Tx_Buff[UART0_Tx_Bytes+1] = 0;

	UART0_Tx_Bytes = strlen(UART0_Tx_Buff);
	UART_CmdProcess();
}
#endif
