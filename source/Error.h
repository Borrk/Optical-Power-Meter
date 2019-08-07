/*****************************************************************************
* 
*   Copyright (C) 2004 Avanex Corporation
*   All rights reserved.
*
*   Module Name:    Error.h
*
*   Author:
*   Description:    Define error message.
*   
*   $Workfile: $
*   $Author: $
*   Date: $
*
******************************************************************************/
#ifndef ERRORS_H
#define ERRORS_H

//-----------------------------------------------------------------------------
//  HW Error
//-----------------------------------------------------------------------------

// hw error bit 			  

#define HW_ERR_EXT_FLASH_ENTRY  0x0001   
#define HW_ERR_EXT_FLASH_WRITE  0x0002   
#define HW_ERR_EXT_FLASH_BUSY   0x0004 							   

#define HW_ERR_INT_FLASH_ERASE  0x0008
#define HW_ERR_INT_FLASH_WRITE  0x0010

//#define HW_ERR_SWITCH_HWFAULT   0x0020

#define HW_ERR_ADC_MCU          0x0040   
//#define HW_ERR_MCU_SPI          0x0080
  
//#define HW_ERR_MODULE_INFO      0x0100
#define HW_ERR_TEMPERATURE      0x0200  
#define HW_ERR_OFFSET           0x0400

#define HW_ERR_PDA2D_CAL        0x0800
//#define HW_ERR_VOAA2D_CAL	    0x1000
//#define HW_ERR_VOAD2A_CAL	    0x2000	
#define HW_ERR_TIMEOUT_OSC      0x4000
#define HW_ERR_FIBER            0x8000



// Error Prompt
#define E_OK                0
#define E_FLASH_PROTECT     1
#define E_FLASH_ERASE       2
#define E_FLASH_WRITE       3
#define E_TIMEOUT_OSC       4
#define E_EXT_FL_BUSY       5
#define E_EXT_FL_VERIFY     6
//#define E_TIMEOUT_SPI       7
#define E_VALUE_INVALID     8	  // Invalid input value
//#define E_SWITCH_HWFAULT    9	  // Fail of Reading Feed Back Pin

#define E_A2D_CONVERT       10
#define E_INVALID_PD_TBL    11
#define E_FIBER    			12
//#define E_SW_SETTING        14	  // Switching To Target Path Fail
#define E_UNKNOW_COMMAND    15	  // Typed in invalid charcter

#define E_MCUADC0_CONVERT   19


#endif  //_ERRORS_H_




