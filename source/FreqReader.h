/*****************************************************************************
* 
*   Copyright Photonic Ltd. (C) 2012
*   All rights reserved.
*
*   Module Name:    FreqReader.h
*   Author:
*   Description:    Identify laser frequncy. 
*   
*   Author:  Huang Yu Qi
*   Date: 06/22/2012
*
******************************************************************************/

#ifndef FREQ_READER_H
#define FREQ_READER_H

#include "Typedefs.h"

/// Basic APIs
void FreqReaderInitHW();
void FreqReaderEnable( eBOOL argEnable );

/// Application APIs
void InitFreqReader();
void ResetFreqReaderParam();

tUINT16 GetCurFreqCount();

#endif  // end of FREQ_READER_H