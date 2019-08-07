
#if !defined GDATA_H
#define GDATA_H

#include "Typedefs.h"

#include "LamdaOption.h"
#include "RangeOption.h"

#ifdef EXTERN
  #undef EXTERN
#endif
#ifdef AT
  #undef AT
#endif
#ifdef EQU
  #undef EQU
#endif
#ifdef SLASH
  #undef SLASH
#endif

#ifdef GDATA_C
  #define EXTERN
  #define AT( location ) _at_ ( location )
  #define EQU =
  #define EQU_ML = 
  #define EQU_MLEND
  #define CONST const
  #define SBIT sbit
#else
  #define EXTERN extern
  #define AT( location )
  #define SLASH /
  #define EQU ; /SLASH
  #define EQU_ML ; SLASH*
  #define EQU_MLEND *SLASH
  #define CONST
  #define SBIT bit
#endif

/// Power state
typedef enum
{
    MeasureModePower = 0,
    MeasureModeFreq  = 1,
   
    MeasureModedB 	= 0,
    MeasureModedBm  = 1,

    MeasureModeFreqNormal  = 0,	///< increase per 100hz
    MeasureModeFreqOur  = 1,	///< 270, 330,1k,2k
    MeasureModeUnknow
} eMeasureMode;

/// Power Unit
typedef enum
{
    PowerUnitdB   = 0,
    PowerUnitdBm  = 1,
} ePowerUnit;

typedef enum
{
	BattaryStateStart = 0,
	BattaryNormal = BattaryStateStart,
	BattaryMiddle,
	BattaryLow,
	BattaryStateTypeOfValues
}eBattaryState;

/// Fiber Power state
typedef enum
{
    PowerValid = 0,
    PowerLow,
    PowerHigh, 
    PowerSampling,
    PowerInvalid
} ePowerState;

#define FreqUnknow (-1)
typedef struct  
{
    eMeasureMode    myMeasureMode;
    ePowerState     myPowerState;
	ePowerUnit      myPowerUnit;
    tUINT8           myCurLamda;
    eBattaryState   myBattState;
}stSystemState;

EXTERN stSystemState gSystemState;
EXTERN tUINT8 sInCalibrationMode;

EXTERN data tUINT8 TH0_BYTE;                  	// Timer0 counter_Hbyte loader
EXTERN data tUINT8 TL0_BYTE;                  	// Timer0 counter_Lbyte loader

#define  HAS_FREQ 1


#if 1
#define  RANGE_MAX 	4
#define  LAMDA_MAX  4

/// Optical Detector convert ratio(=P/A)
typedef struct
{
	float 		k;	/// Power/current
	float		b;
	float		darkCurrent;
}tagLamdaCoeff;

/// Range's current Coefficient
typedef struct 
{
	float k;
	float b;
	tUINT16    offsetADC; //< max ADC   
}tagRangeCoeff;

typedef struct 
{
	tUINT8 		   myCurRange;
	tagRangeCoeff  myRangeCoeff[RANGE_MAX];        
}tagCoefficient;


EXTERN tagLamdaCoeff  xdata	 sLamdaCoeff[LAMDA_MAX];
EXTERN tagCoefficient data  sRangeCoeff;
#endif

//#define HAS_UART
#ifdef HAS_UART

	#define  UART0_BUF_LENGTH 40
	EXTERN unsigned char idata UART0_Tx_Buff[UART0_BUF_LENGTH];		//暂存要发送的数据包
	EXTERN unsigned char idata UART0_Tx_Flag;
	EXTERN unsigned char idata UART0_Tx_Count;		// 发送的数据字节计数器
	EXTERN unsigned char idata UART0_Tx_Bytes;		// 应发送的数据字节数
#endif

/// Temp data for argument parameters to save the RAM space
EXTERN tUINT32	argVal;
EXTERN tUINT8	argValFlag;

//reliability
EXTERN idata tUINT16   SoftWDCount;            	// softdog counter
EXTERN idata eBOOL     TriggerWatchDog;        	// watchdog


#endif // ndef GDATA_H