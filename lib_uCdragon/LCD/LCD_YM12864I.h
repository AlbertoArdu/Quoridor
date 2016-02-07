/***************************************************
*LCD_YM12864I.h
***************************************************/
#include "../def.h"
/**************************************************/
//	assign LCD_ENABLE  =   ( (!nGCS2) && (!ADDR[21]) && (!ADDR[22]) && ( ADDR[23]) ) ;
//ADR1  :  R/S
//ADR2  :  R/W
//ADR3  :  CS1
//ADR4  :  CS2
/**************************************************/
#define Lcd_Left_Command_Write   (*(volatile U8 *)0x82800068)
#define Lcd_Right_Command_Write  (*(volatile U8 *)0x82800070)

#define Lcd_Left_Status_Read	 (*(volatile U8 *)0x8280006c)
#define Lcd_Right_Status_Read    (*(volatile U8 *)0x82800074)

#define Lcd_Left_Data_Write 	 (*(volatile U8 *)0x8280006a)
#define Lcd_Right_Data_Write	 (*(volatile U8 *)0x82800072)

#define Lcd_Left_Data_Read  	 (*(volatile U8 *)0x8280006e)
#define Lcd_Right_Data_Read 	 (*(volatile U8 *)0x82800076)
	

/**************************************************/
extern void Lcd_Clear_Left( void );
extern void Lcd_Clear_Right( void );
extern void Lcd_Clear( void ) ;
extern void Lcd_Init( void ) ;
extern void Lcd_Display_Hanzi_16X16( U8 B , U8 X , U8 Y , U8* S ) ;
extern void Lcd_Display_Pic_128X64( U8* S ) ;
extern void Lcd_Display_Wave( U8 x , U8 y ) ;
extern void Lcd12864_Test( void ) ;
extern void lcdStart( void );

//Funzioni Utilizzate per stampare direttamente sul display
extern void Lcd_Command_Left( U8 a );
extern void Lcd_Command_Right( U8 a );
extern void Lcd_Data_Left_WR( U8 a );
extern void Lcd_Data_Right_WR( U8 a );

//Funzioni di stampa personalizzate
extern void lcdPrintSx(U8 sx[64][64]);
extern void lcdPrintDx(U8 dx[64][64]);
//Funzione che stampa solo la parte alta dello schermo sx
extern void lcdPrintSxUP(U8 sx[64][64]);

