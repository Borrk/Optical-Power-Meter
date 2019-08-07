/*****************************************************************************
* 
*   Copyright(C) Brook 2012
*   All rights reserved.
*
*   Module Name:    Display.h
*   Author:
*   Description:    Display API declaration. 
*   
*   Author:  Huang Yu Qi
*   Date: 04/01/2012
*
******************************************************************************/

#ifndef DISPLAYDRV_H
#define DISPLAYDRV_H

#include "Typedefs.h"

#define LED_REFRESH 0

#define CHAR_HYPHON  	  10
#define CHAR_OFF          11
#define CHAR_HI			  12
#define CHAR_LO           13
#define CHAR_K            15

/// Basic APIs
void vDisplayDrvInitHW();
void vDisplayDrvEnable( eBOOL argEnable );
void vDisp_Off();

/// Application APIs
void vInitDisplayDrv( );

void vDisp_Digit_Signal(tUINT8 temp );  // ��ʾ����λ
void vDisp_Digit_1(tUINT8 temp, tUINT8 bShowDot );  // ��ʾ��λ����ʮ������
void vDisp_Digit_10(tUINT8 temp, tUINT8 bShowDot ); // ��ʾʮλ����ʮ������
void vDisp_DEC(tUINT8 temp);  // ��ʾ1��2λ����ʮ������
void vDisp_LEDs(tUINT8  temp ); // ����:LED��ʾ�ӳ���
void vDisp_Trigger_Update( ); // ����: Send the LED data to LED

#endif  // end of DISPLAYDRV_H