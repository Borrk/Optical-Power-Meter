#ifndef FAKEFUNC_H
#define FAKEFUNC_H


#define FOREVER while(1)
#define DisableGlobalInterrupts_() { EA = 0; }
#define EnableGlobalInterrupts_()  { EA = 1; }

/* 32.1ms interval. */ 
#define EnableWatchDog_() \
{ \
    DisableGlobalInterrupts_(); \
    PCA0CN    = 0x40; \
    PCA0MD   &= ~0x40; \
    PCA0MD    = 0x80; \
    PCA0L     = 0x40; \
    PCA0H     = 0x40; \
    PCA0CPL2  = 0xFF; \
    PCA0MD   |= 0x40; \
    EnableGlobalInterrupts_();\      
}

//½ûÖ¹¿´ÃÅ¹·
#define DisableWatchDog_() \
{ \
    DisableGlobalInterrupts_(); \
	PCA0MD &= ~0x40; \
    EnableGlobalInterrupts_();\      
}

#define RefreshWatchDog_() \
{ \
    PCA0CPH2  = 0xFF; \
}

#define FEED_DOG_() \
{\    
    DisableGlobalInterrupts_(); \
    if ( TriggerWatchDog == FALSE ) \
    {\
        SoftWDCount = 0;\
    }\
    EnableGlobalInterrupts_();\      
} 

#define DisableHeartBeatingInterrupts_() \
{ \
    ET0 = DISABLE; \
}

#define EnableHeartBeatingInterrupts_() \
{ \
    ET0 = ENABLE; \
}

#define LoadTimer0_100us_() \
{\
    DisableHeartBeatingInterrupts_();\
    TH0_BYTE = TIMER0_100us_HIGHBYTE;\
    TL0_BYTE = TIMER0_100us_LOWBYTE;\
	EnableHeartBeatingInterrupts_();\
}

#define RecoverTimer0_() \
{\
    DisableHeartBeatingInterrupts_();\
    TH0_BYTE = TIMER0_2ms_HIGHBYTE;\
    TL0_BYTE = TIMER0_2ms_LOWBYTE;\
	EnableHeartBeatingInterrupts_();\
}


#define THIS_IS_LTR( para )  ( para >= 'A' && para <= 'F' )
#define THIS_IS_NUM( para )  ( para >= '0' && para <= '9' )

#define IF_ODD_( num )   if ( (num - (num>>1<<1)) == 1 )
#define IF_EVEN_( num )  if ( (num - (num>>1<<1)) == 0 )





#endif