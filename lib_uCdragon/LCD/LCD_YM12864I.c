/**************************************************
*File :LCD_YM12864I.c
*Autor:GongJun
*Date :2002-9-30 10:21
*Modifier:GongJun
*Date:2005-2-19 19:35
*Description:128*64 LCM£¨YM12864I£©driver
*Version:2.0
***************************************************/
#include "../config.h"
#include "../board.h"
#include "LCD_YM12864I.h"


/***************************************************
*Function   :void Lcd_Command_Left(U8 a)
*Object     :Write command to the left half of display area.
*Description:No
*Parameter	:U8 a :command byte
*Return		:No
***************************************************/
void Lcd_Command_Left( U8 a )
{
	while ( ( Lcd_Left_Status_Read & 0x80 ) == 0x80 );//busy wait
	Lcd_Left_Command_Write = a;				          //write command byte
}

/***************************************************
*Function   :void Lcd_Command_Right(U8 a)
*Object     :Write command to the right half of display area
*Description:No
*Parameter  :U8 a : command byte
*Return		:No
***************************************************/
void Lcd_Command_Right( U8 a )
{
	while ( ( Lcd_Right_Status_Read & 0x80 ) == 0x80 );//busy wait
	Lcd_Right_Command_Write = a;					   //write command byte
}

/***************************************************
*Function   :void Lcd_Data_Left_WR(U8 a)
*Object     :Write data to the left half of display area.
*Description:No
*Parameter	:U8 a:Displaying data
*Return		:No
***************************************************/
void Lcd_Data_Left_WR( U8 a )
{
	while ( ( Lcd_Left_Status_Read & 0x80 ) == 0x80 );//busy wait
	Lcd_Left_Data_Write = a;				//write data
}

/***************************************************
*Function   :void Lcd_Data_Right_WR(U8 a)
*Object     :Write data to the right half of display area.
*Description:No
*Parameter	:U8 a:Displaying data
*Return		:No
***************************************************/
void Lcd_Data_Right_WR( U8 a )
{
	while ( ( Lcd_Right_Status_Read & 0x80 ) == 0x80 );	//busy wait
	Lcd_Right_Data_Write = a;				//write data
}

/***************************************************
*Function   :U8 Lcd_Data_Left_RD(void)
*Object     :Read data from the left half of display area.
*Description:No
*Parameter	:No
*Return		:return data from the left half of display area.
			 U8:unsigned char
***************************************************/
U8 Lcd_Data_Left_RD( void )
{
	U8 b ;

	while ( ( Lcd_Left_Status_Read & 0x80 ) == 0x80 );//busy wait
	b = Lcd_Left_Data_Read ;				//read data

	return( b ) ;
}

/***************************************************
*Function	:U8 Lcd_Data_Right_RD(void)
*Object		:Read data from the right half of display area
*Description:No
*Parameter	:No
*Return 	:return data from the left half of display area.
***************************************************/
U8 Lcd_Data_Right_RD( void )
{
	U8 b ;

	while ( ( Lcd_Right_Status_Read & 0x80 ) == 0x80 );	//busy wait
	b = Lcd_Right_Data_Read ;				//read data

	return( b ) ;
}

/***************************************************
*Function	:void Lcd_Clear(void)
*Object		:Clear LCD screen
*Description:No
*Parameter	:No
*Return		:No
***************************************************/
void Lcd_Clear( void )
{
	U8 a, b;

	for ( a = 0; a < 8; a++ )			//Clear RAM space of display
	{
		Lcd_Command_Left( 0xb8 | a );	//Set row address of the left half in display area
		Lcd_Command_Right( 0xb8 | a );  //Set row address of the left half in display area
		Lcd_Command_Left( 0x40 );		//Set tier address of the left half in display area
		Lcd_Command_Right( 0x40 );	    //Set tier address of the left half in display area
		b = 64;
		while ( b )
		{
			Lcd_Data_Left_WR( 0x00 );	//Write 0 on the left half of display area
			Lcd_Data_Right_WR( 0x00 );	//Write 0 on the right half of display area.
			b = b - 1;
		}
	}
}
void Lcd_Clear_Right( void )
{
	U8 a, b;

	for ( a = 0; a < 8; a++ )			//Clear RAM space of display
	{
		Lcd_Command_Right( 0xb8 | a );  //Set row address of the left half in display area
		Lcd_Command_Right( 0x40 );	    //Set tier address of the left half in display area
		b = 64;
		while ( b )
		{
			Lcd_Data_Right_WR( 0x00 );	//Write 0 on the right half of display area.
			b = b - 1;
		}
	}
}
void Lcd_Clear_Left( void )
{
	U8 a, b;

	for ( a = 0; a < 8; a++ )			//Clear RAM space of display
	{
		Lcd_Command_Left( 0xb8 | a );	//Set row address of the left half in display area
		Lcd_Command_Left( 0x40 );		//Set tier address of the left half in display area
		b = 64;
		while ( b )
		{
			Lcd_Data_Left_WR( 0x00 );	//Write 0 on the left half of display area.
			b = b - 1;
		}
	}
}

/***************************************************
*Function	:void Lcd_Init(void)
*Object     :Init LCM
*Description:No
*Parameter	:No
*Return 	:No
***************************************************/
void Lcd_Init( void )
{
	int timeout=10000;			//overtime control
	Lcd_Command_Left( 0xc0 );	//Set the start row address on the left half
	Lcd_Command_Right( 0xc0 );	//Set the start row address on the right half

	do
	{
		Lcd_Command_Left( 0x3f );
		timeout--;
	}
	while ( (( Lcd_Left_Status_Read & 0x20 ) == 0x20) && (timeout!=0) );//if display and overtiem is not,wait
	timeout=10000;
	do
	{
		Lcd_Command_Right( 0x3f );//Open the left and right half of display area.
		timeout--;
	}
	while ( (( Lcd_Right_Status_Read & 0x20 ) == 0x20) && (timeout!=0) );//if display and overtiem is not,wait

}

/***************************************************
*Function	:void Lcd_Init(void)
*Object		:Open or Close backlight of LCD
*Description:No
*Parameter	:m: 1: open 0:close
*Return		:No
***************************************************/
void Lcd_Backlight( U8 m )
{
	if ( m == TRUE )
		IO1CLR = IO1CLR | ( 1 << 23 ) ;
	else
		IO1SET = IO1SET | ( 1 << 23 ) ;
}


/***************************************************
*Function	:void Lcd_Display_A(S)
*Object 	:Display a 128*64 picture on the LCD screen
*Description:No
*Parameter	:*S:Picture data arry
*Return		:No
***************************************************/
void Lcd_Display_Pic_128X64( U8* S )
{
	U8 a, b; U32 c = 0;
	for ( a = 0; a < 8; a++ )
	{
		Lcd_Command_Left( 0xb8 | a );
		Lcd_Command_Left( 0x40 );	//Display the left picture on the left half of display area.

		for ( b = 0; b < 64; b++ )
		{
			Lcd_Data_Left_WR( S[c] );c = c + 1;
		}

		Lcd_Command_Right( 0xb8 | a );
		Lcd_Command_Right( 0x40 );	//Display the right picture on the right half of display area.

		for ( b = 0; b < 64; b++ )
		{
			Lcd_Data_Right_WR( S[c] );c = c + 1;
		}
	}
}

/***************************************************
*Function	:LcdStart
*Object     :start lcd
*Description:No
*Parameter	:No
*Return		:No
***************************************************/
void lcdStart( void )
{
	Lcd_Init();		       //LCD Init
	Lcd_Clear();		   //Clear screen
	Lcd_Backlight( TRUE ) ;//Open Backlight
	//Lcd_Display_Pic_128X64( LPC229X_Infor ) ;		// Display 128x64 picture
}

/***************************************************
*Function	:lcdPrint
*Object     :no
*Description:print matrix on display
*Parameter	:matrici sx e dx
*Return		:No
***************************************************/
void lcdPrintSx(U8 sx[64][64]){

	U8 i, j, k;
	U8 parola;

	for(i = 0; i<8; i++)
	{
		//Creo e stampo parola schermo sx
		Lcd_Command_Left( 0xb8 | i );
		Lcd_Command_Left( 0x40 );	//Display the left picture on the left half of display area.
		for(j = 0; j<64; j++)
		{
			parola = 0;
			for(k = 0; k<8; k++)
			{
				if(sx[8*i+k][j] == 255)
				{
					//Setto a 1 il bit corrispondente
					parola |= 1 << k;

				} else if (sx[8*i+k][j] == 0)
				{
					//Setto a 0 il bit corrispondente
					parola &= ~(1 << k);
				}
			}
			Lcd_Data_Left_WR(parola);
		}

	}
}

void lcdPrintSxUP(U8 sx[64][64]){

	U8 i, j, k;
	U8 parola;

	for(i = 0; i<2; i++)
	{
		//Creo e stampo parola schermo sx
		Lcd_Command_Left( 0xb8 | i );
		Lcd_Command_Left( 0x40 );	//Display the left picture on the left half of display area.
		for(j = 0; j<64; j++)
		{
			parola = 0;
			for(k = 0; k<8; k++)
			{
				if(sx[8*i+k][j] == 255)
				{
					//Setto a 1 il bit corrispondente
					parola |= 1 << k;

				} else if (sx[8*i+k][j] == 0)
				{
					//Setto a 0 il bit corrispondente
					parola &= ~(1 << k);
				}
			}
			Lcd_Data_Left_WR(parola);
		}

	}
}

void lcdPrintDx(U8 dx[64][64]){

	U8 i, j, k;
	U8 parola;

	for(i = 0; i<8; i++)
	{
		//Creo e stampo parola schermo dx
		Lcd_Command_Right( 0xb8 | i );
		Lcd_Command_Right( 0x40 );	//Display the right picture on the right half of display area.

		for(j=0; j<64; j++){

			parola = 0;
			for(k = 0; k<8; k++)
			{
				if(dx[8*i+k][j] == 255)
				{
					//Setto a 1 il bit corrispondente
					parola |= 1 << k;

				} else if (dx[8*i+k][j] == 0)
				{
					//Setto a 0 il bit corrispondente
					parola &= ~(1 << k);
				}
			}
			Lcd_Data_Right_WR(parola);
		}
	}
}



