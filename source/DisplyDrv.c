/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    DisplayDrv.c
*   Author:
*   Description:    Display on screen/LEDs.. API. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/23/2012
*
******************************************************************************/

#include "C8051F350.h"
#include "DisplayDrv.h"


//     A
//    ---
// F | G | B
//    ---
// E |   | C
//    --- . DP
//     D
// D7 D6 D5 D4 D3 D2 D1 D0
 
tUINT8 code Led_Show[]=
{
	0x7E     , //Code0   
	0x42     , //Code1   
	0x37     , //Code2   
	0x67     , //Code3   
	0x4B     , //Code4   
	0x6D     , //Code5   
	0x7D     , //Code6   
	0x46     , //Code7   
	0x7F     , //Code8   
	0x6F     , //Code9   
	
	0x01     , //Code -   
	0x00     , //Code 全灭   
	0x5B     , //CodeH   
	0x38     , //CodeL   
	0x3D     , //CodeE   
	0x79     , //Code b   
	0x1D      //CodeF
};

eBOOL idata sbEnableDrv = 0;     


//P0
sbit HC164_SCK = P0^0; //74HC164的时钟信号
sbit HC164_RCK = P0^1; //74HC164的LOCK时钟信号
sbit HC164_SS  = P0^2; //74HC164的Enable信号
sbit HC164_DIN = P0^3; //74HC164的数据输入

#define ON  0
#define OFF 1

/*-----------------------------------------------------------------------------------*\
 *
 *  HardWare initilization section.
 *
 *  Use 74HC164 to send data to LED, the order is 
 *  Used HW resource:
 *  - P0.0:  SCLK 
 *  - P0.1:  RCLK 
 *  - P0.2:  SS
 *  - P0.3:  SDATA
 *
\*-----------------------------------------------------------------------------------*/

void vDisplayDrvInitHW()
{
	/// Conig P0 & P0SKIP
  	P0MDIN  |= 0x0f;
  	P0MDOUT |= 0x0f; // Pushpull 
	P0SKIP  |= 0x0f;
}

void vDisplayDrvEnable( eBOOL argEnable )
{
	sbEnableDrv = argEnable;
	if ( sbEnableDrv == FALSE )
	{
		vDisp_Off();
	}
	else
	{
		HC164_SS = 0;
	}
}


/*-----------------------------------------------------------------------------------*\
 *
 *  API section.
 *
\*-----------------------------------------------------------------------------------*/

void vInitDisplayDrv( )
{
	sbEnableDrv = TRUE;
}

void vDisp_Off()
{
	HC164_SS = 1;
}


//-----------------------------------------------------------------------------
//
// 功能:数码管显示子程序
//
//-----------------------------------------------------------------------------
#define SHOW_DOT(val) val = (val | 0x80)

void vDisp_Digit_1(tUINT8 temp, tUINT8 bShowDot )
{
    tUINT8 data i;
    tUINT8 data temp1;

    temp1 = Led_Show[temp];
	if ( bShowDot && temp < 10 ) /// show dot only when show digital number
	{
		SHOW_DOT(temp1);
	}

   for(i = 0;i < 8;i++ )
    {
        HC164_SCK = 0;
        if( (temp1 & 0x80) == 0x80 )
            HC164_DIN = 0;
        else
            HC164_DIN = 1;
        HC164_SCK = 1;
        temp1 <<=1;
    }
}

void vDisp_Digit_10(tUINT8 temp, tUINT8 bShowDot )
{
    tUINT8 data i;
    tUINT8 data temp1;
  
    temp1 = Led_Show[temp]; 
	if ( bShowDot && temp < 10 ) /// show dot only when show digital number
	{
		SHOW_DOT(temp1);
	}
    for(i = 0;i < 8;i++ )
    {
        HC164_SCK = 0;
        if( (temp1 & 0x80) == 0x80 )
            HC164_DIN = 0;
        else
            HC164_DIN = 1;
        HC164_SCK = 1;
        temp1 <<=1;
    }
}

void vDisp_Digit_Signal( tUINT8 temp )
{
	vDisp_Digit_1( temp, 0 );
}

//-----------------------------------------------------------------------------
//
// 显示1个2位数的十进制数
//
//-----------------------------------------------------------------------------
void vDisp_DEC(tUINT8 temp)
{
}


//-----------------------------------------------------------------------------
//
// 功能:LED显示子程序
//
//-----------------------------------------------------------------------------
void vDisp_LEDs(tUINT8  temp )
{
    tUINT8 data i,temp1;

    temp1 = temp;
    for(i = 0;i < 8;i++ )
    {
        HC164_SCK = 0;
        if( (temp1 & 0x80) == 0x80 )
            HC164_DIN = 0;
        else
            HC164_DIN = 1;
        HC164_SCK = 1;
        temp1 <<=1;
    }
}

void vDisp_Trigger_Update()
{
    tUINT16 data i,j;

	HC164_RCK = 0;

	/// delay
    for (i = 0; i< 4; i++)
    {
		for ( j = 0; j < 2450; j++ );
    }
	
	HC164_RCK = 1;
}