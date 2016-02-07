/********************************************************************
*File :LPC229x_lib.c
*Autor:GongJun
*Date :
*Modifier:cgf
*Date:2005-8-5
*Description:some library function about target
*********************************************************************/
#include "config.h"
#include "board.h"

/*********************************************************************
*Function   :TargetResetInit( void )
*Object     :Target init
*Description:No
*Parameter	:No
*Return		:No
**********************************************************************/
void TargetResetInit( void )
{
#ifdef __DEBUG    
	MEMMAP = 0x3;   				//remap,external flash

#endif

#ifdef __OUT_CHIP    
	MEMMAP = 0x3;   				//remap,external flash
#endif

#ifdef __IN_CHIP	
	MEMMAP = 0x1;   				//remap,internal flash

#endif

	/* set clock */
	PLLCON = 1;
#if (Fpclk / (Fcclk / 4)) == 1
	VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
	VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
	VPBDIV = 1;
#endif

#if (Fcco / Fcclk) == 2
	PLLCFG = ( ( Fcclk / Fosc ) - 1 ) | ( 0 << 5 );
#endif
#if (Fcco / Fcclk) == 4
	PLLCFG = ( ( Fcclk / Fosc ) - 1 ) | ( 1 << 5 );
#endif
#if (Fcco / Fcclk) == 8
	PLLCFG = ( ( Fcclk / Fosc ) - 1 ) | ( 2 << 5 );
#endif
#if (Fcco / Fcclk) == 16
	PLLCFG = ( ( Fcclk / Fosc ) - 1 ) | ( 3 << 5 );
#endif
	PLLFEED = 0xaa;
	PLLFEED = 0x55;
	while ( ( PLLSTAT & ( 1 << 10 ) ) == 0 );
	PLLCON = 3;
	//PLLCFG=0x24;
	PLLFEED = 0xaa;
	PLLFEED = 0x55;

	/* set MAM */
	MAMCR = 0;
#if Fcclk < 20000000
	MAMTIM = 1;
#else
#if Fcclk < 40000000
	MAMTIM = 2;
#else
	MAMTIM = 3;
#endif
#endif
	MAMCR = 2;

	/*init VIC  */
	VICIntEnClr = 0xffffffff;
	VICVectAddr = 0;
	VICIntSelect = 0;

}

/*********************************************************************
*Function   :PortInit( void )
*Object     :Port init
*Description:No
*Parameter	:No
*Return		:No
**********************************************************************/
void PortInit( void )
{
	//*** 32bit PORT 0 P0.0--P0.15 GROUP function configuration_____________gongjun
	//Ports  :  P0.15    P0.14    P0.13    P0.12	P0.11    P0.10    P0.09    P0.08
	//Signal :  RI  	 CD 	  DTR     DSR      CTS  	RTS 	 RXD1     TXD1 
	//Binary :  01  	 01 	  01	   01   	01  	 01 	  01	   01
	//______________________________________________________________________________
	//Ports  :  P0.07    P0.06    P0.05    P0.04	P0.03    P0.02    P0.01    P0.00
	//Signal :  GPIO	 GPIO     GPIO     GPIO 	GPIO	 GPIO     RXD0     TXD0 
	//Binary :  00  	 00 	  00	   00   	00  	 00 	  01	   01
	//******************************************************************************
	PINSEL0 = ( 0x5555 << 16 ) | ( 0x0005 )  ;		

	//*** 32bit PORT 0 P0.0--P0.31 GROUP direction configuration_____________gongjun
	//Ports  :  P0.31    P0.30    P0.29    P0.28	P0.27    P0.26    P0.25    P0.24
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P0.23    P0.22    P0.21    P0.20	P0.19    P0.18    P0.17    P0.16
	//Signal :  I   	 I  	  o 	   I		o   	 I  	  I 	   I
	//Binary :  0   	 0  	  1 	   0		1   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P0.15    P0.14    P0.13    P0.12	P0.11    P0.10    P0.09    P0.08
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P0.07    P0.06    P0.05    P0.04	P0.03    P0.02    P0.01    P0.00
	//Signal :  I   	 o  	  o 	   o		I   	 o  	  I 	   o
	//Binary :  0   	 1  	  1 	   1		0   	 1  	  0 	   1
	//******************************************************************************
	IO0DIR = ( 0x00 << 24 ) | ( 0x28 << 16 ) | ( 0x00 << 8 ) | ( 0x75 )  ;		

	//*** 32bit PORT 0 P0.16--P0.31 GROUP function configuration_____________gongjun
	//Ports  :  P0.31    P0.30    P0.29    P0.28	P0.27    P0.26    P0.25    P0.24
	//Signal :  EINT3	 GPIO     AIN2     AIN1 	AIN0	 GPIO     RD1      TD2 
	//Binary :  10  	 00 	  01	   01   	01  	 00 	  01	   01
	//______________________________________________________________________________
	//Ports  :  P0.23    P0.22    P0.21    P0.20	P0.19    P0.18    P0.17    P0.16
	//Signal :  RD2 	GPIO	 PWM5     GPIO     GPIO 	GPIO	 GPIO     GPIO 
	//Binary :  01  	 00 	  01	   00   	00  	 00 	  00	   00
	//******************************************************************************
	PINSEL1 = ( 0x2545 << 16 ) | ( 0x0400 )  ;		// 设置P0.16--P0.31引脚功能

	//*** 32bit PORT 1, PORT 2, PORT 3 GROUP function configuration__________gongjun
	//Ports  :  RESERVER P3.22:2  P3.1
	//Signal :  GPIO	 ADDR2:23 ADDR1 
	//Binary :  00  	 111	   1
	//______________________________________________________________________________
	//Ports  :  P3.00    P2.31    P2.30    P2.29:28 RESERVER P3.24
	//Signal :  ADDR0   GPIO	 GPIO     GPIO     GPIO 	CS3 
	//Binary :  1   	0   	 0  	  0 	   00   	01
	//______________________________________________________________________________
	//Ports  :  P3.25    P3.23    RESERVER P3.26	RESERVER RESERVER P3.27
	//Signal :  CS2 	 ADDR23   GPIO     CS1  	GPIO	 GPIO     nWE 
	//Binary :  01  	 1  	  0 	   1		0   	 0  	  1
	//______________________________________________________________________________
	//Ports  :  P3.28    P3.29    P1.2.3   P1.25:16 P1.31:26 RESERVER RESERVER
	//Signal :  GPIO	 GPIO     GPIO     DEBUG	GPIO	 GPIO     GPIO
	//Binary :  0   	 0  	  01	   1		0   	 0  	  0
	//******************************************************************************
	PINSEL2 = ( 0xf << 24 ) | ( 0x81 << 16 ) | ( 0x69 << 8 ) | ( 0x1C )  ;		

	//*** 32bit PORT 0 P1.00--P1.31 GROUP direction configuration_____________gongjun
	//Ports  :  P1.31    P1.30    P1.29    P1.28	P1.27    P1.26    P1.25    P1.24
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   o
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   1
	//______________________________________________________________________________
	//Ports  :  P1.23    P1.22    P1.21    P1.20	P1.19    P1.18    P1.17    P1.16
	//Signal :  o   	 o  	  o 	   I		I   	 o  	  o 	   I
	//Binary :  1   	 1  	  1 	   0		0   	 1  	  1 	   0
	//______________________________________________________________________________
	//Ports  :  P1.15    P1.14    P1.13    P1.12	P1.11    P1.10    P1.09    P1.08
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P1.07    P1.06    P1.05    P1.04	P1.03    P1.02    P1.01    P1.00
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//******************************************************************************
	IO1DIR = ( 0x01 << 24 ) | ( 0xe6 << 16 ) | ( 0x00 << 8 ) | ( 0x00 ) ;		

	//*** 32bit PORT 2 GROUP direction configuration_____________gongjun
	//Ports  :  P2.31    P2.30    P2.29    P2.28	P2.27    P2.26    P2.25    P2.24
	//Signal :  o   	 o  	  o 	   o		o   	 I  	  I 	   I
	//Binary :  1   	 1  	  1 	   1		1   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P2.23    P2.22    P2.21    P2.20	P2.19    P2.18    P2.17    P2.16
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P2.15    P2.14    P2.13    P2.12	P2.11    P2.10    P2.09    P2.08
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P2.07    P2.06    P2.05    P2.04	P2.03    P2.02    P2.01    P2.00
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//******************************************************************************
	IO2DIR = ( 0xf8<<24)|(0x00<<16)|(0x00<<8)|(0x00) ;	
	

	//*** 32bit PORT 3 GROUP direction configuration_____________gongjun
	//Ports  :  P3.31    P3.30    P3.29    P3.28	P3.27    P3.26    P3.25    P3.24
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P3.23    P3.22    P3.21    P3.20	P3.19    P3.18    P3.17    P3.16
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P3.15    P3.14    P3.13    P3.12	P3.11    P3.10    P3.09    P3.08
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//______________________________________________________________________________
	//Ports  :  P3.07    P3.06    P3.05    P3.04	P3.03    P3.02    P3.01    P3.00
	//Signal :  I   	 I  	  I 	   I		I   	 I  	  I 	   I
	//Binary :  0   	 0  	  0 	   0		0   	 0  	  0 	   0
	//******************************************************************************
	IO3DIR = ( 0x00 << 24 ) | ( 0x00 << 16 ) | ( 0x00 << 8 ) | ( 0x00 )  ;
}

/*********************************************************************
*Function   :EXTINT_init()
*Object     :control which external INTR to enable
*Description:No
*Parameter	:No
*Return		:No
**********************************************************************/
void ExternalInterruptInit(void){
	VICIntEnable = 0x00020000;
	EXTWAKE = 0x8;
	EXTMODE = 0x8;
}

/*********************************************************************
*Function   :LedSet(U32 m)
*Object     :control which leds to light and dark
*Description:No
*Parameter	:U32 m: which leds 0=00000000 1=00000001 2=00000010 3=00000011
*Return		:No
**********************************************************************/
void LedSet( U32 m )
{
	if (!( m & 0x01 ))
		IO0SET = IO0SET | ( 1 << 2 ) ;
	else
		IO0CLR = IO0CLR | ( 1 << 2 ) ;

	if (!( m & 0x02 ))
		IO0SET = IO0SET | ( 1 << 4 ) ;
	else
		IO0CLR = IO0CLR | ( 1 << 4 ) ;

	if (!( m & 0x4 ))
		IO0SET = IO0SET | ( 1 << 5 ) ;
	else
		IO0CLR = IO0CLR | ( 1 << 5 ) ;

	if (!( m & 0x08 ))
		IO0SET = IO0SET | ( 1 << 6 ) ;
	else
		IO0CLR = IO0CLR | ( 1 << 6 ) ;

	if (!( m & 0x10 ))
		IO2SET = IO2SET | ( 1 << 28 ) ;
	else
		IO2CLR = IO2CLR | ( 1 << 28 ) ;

	if (!( m & 0x20 ))
		IO2SET = IO2SET | ( 1 << 29 ) ;
	else
		IO2CLR = IO2CLR | ( 1 << 29 ) ;

	if (!( m & 0x40 ))
		IO2SET = IO2SET | ( 1 << 30 ) ;
	else
		IO2CLR = IO2CLR | ( 1 << 30 ) ;

	if (!( m & 0x80 ))
		IO2SET = IO2SET | ( 1 << 31 ) ;
	else
		IO2CLR = IO2CLR | ( 1 << 31 ) ;
}







