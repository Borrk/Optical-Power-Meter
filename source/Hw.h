#ifndef HW_H
#define HW_H

#include "Typedefs.h"
#include "C8051F350.h"

//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for 'F35x
//-----------------------------------------------------------------------------

sfr16 TMR2RL = 0xCA;                   // Timer2 reload value
sfr16 TMR2 = 0xCC;                     // Timer2 counter
sfr16 ADC0DEC = 0x9A;                  // ADC0 Decimation Ratio Register

//Reset
#define COLD_BUFFER_SIZE        0x10		//0x80
#define MMI_SOFTSTART_PATTERN   0x6688

typedef enum
{
    HARD_START = 0,
    SOFT_START,
    RESET_START,
    WATCHDOG_START
} eStartUpType;

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
//Timer 
//#define WATCHDOG_TIMEOUT	500

#define SYSCLK_HZ     24500000		// SYSCLK frequency in Hz
#define MDCLK         2457600       // Modulator clock in Hz (ideal is 2.4576 MHz)
#define OWR                60     	// Desired Output Word Rate in Hz
  
#define TIMER0_MS_CONSTANT      2
  
#define TIMER0_2ms_HIGHBYTE    ( (0x10000-TIMER0_MS_CONSTANT*SYSCLK_HZ/1000/12)/0x100 )   /* 0xAE = (65536-10*25000/12)/256; */
#define TIMER0_2ms_LOWBYTE     ( (0x10000-TIMER0_MS_CONSTANT*SYSCLK_HZ/1000/12)%0x100 )   /* 0x9F = (65536-10*25000/12)%256; */

#define TIMER0_100us_HIGHBYTE  ( (0x10000-SYSCLK_HZ/10000/12)/0x100 )   
#define TIMER0_100us_LOWBYTE   ( (0x10000-SYSCLK_HZ/10000/12)%0x100 )   

#define TIMER1_MS_CONSTANT      20
#define TIMER1_20ms_HIGHBYTE    ( (0x10000-TIMER1_MS_CONSTANT*SYSCLK_HZ/1000/12)/0x100 )   /* 0xAE = (65536-10*25000/12)/256; */
#define TIMER1_20ms_LOWBYTE     ( (0x10000-TIMER1_MS_CONSTANT*SYSCLK_HZ/1000/12)%0x100 )   /* 0x9F = (65536-10*25000/12)%256; */

//#define TIMER1_MS_CONSTANT      30
//#define TIMER1_30ms_HIGHBYTE    ( (0x10000-TIMER1_MS_CONSTANT*SYSCLK_HZ/1000/12)/0x100 )   /* 0xAE = (65536-10*25000/12)/256; */
//#define TIMER1_30ms_LOWBYTE     ( (0x10000-TIMER1_MS_CONSTANT*SYSCLK_HZ/1000/12)%0x100 )   /* 0x9F = (65536-10*25000/12)%256; */

#define HEART_BEATING_MS    TIMER0_MS_CONSTANT

#define TIMER_2ms           (    2 / HEART_BEATING_MS )
#define TIMER_4ms           (    4 / HEART_BEATING_MS )
#define TIMER_5ms           (    5 / HEART_BEATING_MS )
#define TIMER_10ms          (   10 / HEART_BEATING_MS )
#define TIMER_20ms          (   20 / HEART_BEATING_MS )
#define TIMER_40ms          (   40 / HEART_BEATING_MS )
#define TIMER_50ms          (   50 / HEART_BEATING_MS )
#define TIMER_100ms         (  100 / HEART_BEATING_MS )
#define TIMER_200ms         (  200 / HEART_BEATING_MS )
#define TIMER_400ms         (  400 / HEART_BEATING_MS )
#define TIMER_500ms         (  500 / HEART_BEATING_MS )
#define TIMER_800ms         (  800 / HEART_BEATING_MS )
#define TIMER_1s            ( 1000 / HEART_BEATING_MS )

//#define TIMER2_RELOAD	(0x10000-SYSCLK_HZ/10000/12)  //100us Reload value for Timer2
#define TIMER2_RELOAD	(0x10000-SYSCLK_HZ/8000/12)  //120us Reload value for Timer2

//#define ACTIVE_LED_INTERVAL   TIMER_1s
#define WATCH_DOG_TIMEOUT   TIMER_1s


typedef enum
{
   PD1 = 0,
   PD2, 
   MAX_PD_NUM,
   INVALID_PDNo
} ePD_No;


//MCU_ADC0
typedef enum                          
{
   MCU_AIN0_0 = 0,
   MCU_AIN0_1,
   MCU_AIN0_2,
   MCU_AIN0_3,
   MCU_AIN0_4,
   MCU_AIN0_5,
   MCU_AIN0_6,
   MCU_AIN0_7,
   MCU_AIN0_8 
} eMCUAIN0;


// With the Keil compiler and union byte addressing:
// [0] = bits 31-24, [1] =  bits 23-16, [2] = bits 15-8, [3] = bits 7-0
#define Byte3 0
#define Byte2 1
#define Byte1 2
#define Byte0 3
typedef union LONGDATA{    		// Access LONGDATA as an
   tUINT32 result;              // unsigned long variable or
   tUINT8  Byte[4];             // 4 unsigned byte variables
}LONGDATA;

#define MCU_ADC0_STAGE1 0xf00000
#define MCU_ADC0_STAGE2 1677720L
//#define VERF_2V_FA   2.500
//#define VREF_ADC0  VERF_2V_FA
#define ADC_VOL_MIN 10
#define ADC_VOL_MAX 2500
#define ADC_AVERAGE_BIT 3
#define ADC_AVERAGE_NUMBER ( 1 << ADC_AVERAGE_BIT )


// F = 270Hz --> T = 3.7037 ms
// F = 1kHz  --> T = 1.0000 ms
// F = 2kHz  --> T = 0.5000 ms
// F = 4kHz  --> T = 0.2500 ms = 250 us

// Timer1 1000ms
#define FRQ_270_MIN 240		//1000ms内有 270 个周期
#define FRQ_270_MAX 290		//1000ms内有 270 个周期
#define FRQ_330_MIN 291		//1000ms内有 330 个周期
#define FRQ_330_MAX 370		//1000ms内有 330 个周期
#define FRQ_1K_MIN 	800		//1000ms内有1000个周期
#define FRQ_1K_MAX 	1400	//1000ms内有1000个周期
#define FRQ_2K_MIN 	1700	//1000ms内有2000个周期
#define FRQ_2K_MAX 	2400	//1000ms内有2000个周期


//-----IO MAP ----

//PDs
sbit TIA1_R = P1^1;		// 跨阻放大器的跨阻值, 10K
sbit TIA2_R = P1^0;		// 跨阻放大器的跨阻值, 10M
sbit TIA3_R = P1^3;		// 跨阻放大器的跨阻值, 1M
sbit TIA4_R = P1^2;		// 跨阻放大器的跨阻值, 100K
sbit TIA1_L = P1^5;		// 跨阻放大器的跨阻值, 10K
sbit TIA2_L = P1^4;		// 跨阻放大器的跨阻值, 10M
sbit TIA3_L = P1^7;		// 跨阻放大器的跨阻值, 1M
sbit TIA4_L = P1^6;		// 跨阻放大器的跨阻值, 100K  

#endif  // ndef HW_H