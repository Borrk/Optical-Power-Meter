#if !defined EXTFUNCDECLARE_H
#define EXTFUNCDECLARE_H
//Main.c
void vSendBoardInfo(void);
void vInitData(void);

//Init.c
//void vInitICReg(void);


//PD.c

//public
//void vPrintErrorCodes(void);



// isr.c
// function prototypes
// NOTE: functions are used in Startup.A51!!!
/*
void vInt_INT0(void);       // interrupt 0;
void vInt_TIM0(void);       // interrupt 1;
void vInt_INT1(void);       // interrupt 2;
void vInt_TIM1(void);       // interrupt 3;
void vInt_UART0(void);      // interrupt 4;
void vInt_TIM2(void);       // interrupt 5;
void vInt_SPI(void);        // interrupt 6;
void vInt_SMBUS(void);      // interrupt 7;
  							// interrupt 8;
  							// interrupt 9;
void vInt_ADC0_EOC(void);   // interrupt 10;
void vInt_PCA(void);    	// interrupt 11;      
void vInt_CMP0(void);  		// interrupt 12;
  							// interrupt 13;
void vInt_TIM3(void);       // interrupt 14;
*/

#endif //ndef EXTFUNCDECLARE_H