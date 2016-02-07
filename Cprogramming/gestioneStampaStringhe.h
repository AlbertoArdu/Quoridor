#include "../lib_uCdragon/LCD/LCD_YM12864I.h"
#include "../lib_uCdragon/def.h"
//#define NO_LCD		//Commentare quando si installa sulla scheda

int putc(char c);					 									// stampa un carettere
int printXY(const U8 carattere[6],U8 pX,U8 pY);		//stampa un pattern di 6x8 bit in posizione pX e pY
int init_countdown(U8 tempo);
int dec_countdown(void);
int restart_countdown(void);
int puts(char *s,U8 riga);													//stampa una stringa di 10 caratteri massimi e 
int putcXY(char c,U8 X,U8 Y);																						//ritorna il numero di caratteri stampati
