#include "gestioneStampaStringhe.h"

void itoa(unsigned char a,unsigned char b[2]);


U8 cursoreX=0,cursoreY=0,t,conto_attuale;

static const unsigned char numero[11][6]={
														{0x00,0x3C,0x42,0x42,0x3C,0x00},
														{0x00,0x08,0x04,0x7E,0x00,0x00},
														{0x00,0x64,0x52,0x4A,0x46,0x00},
														{0x00,0x4A,0x4A,0x4A,0x3C,0x00},
														{0x00,0x30,0x28,0x24,0x7E,0x00},
														{0x00,0x4E,0x4A,0x4A,0x32,0x00},
														{0x00,0x7E,0x4A,0x4A,0x7A,0x00},
														{0x00,0x02,0x72,0x0A,0x06,0x00},
														{0x00,0x7E,0x4A,0x4A,0x7E,0x00},
														{0x00,0x4E,0x4A,0x4A,0x7E,0x00},
														{0x00,0x60,0x10,0x08,0x06,0x00}
													};

													
static const unsigned char lettera[27][6]={
														{0x00,0x7C,0x12,0x12,0x7C,0x00},
														{0x00,0x7E,0x4A,0x4A,0x64,0x00},
														{0x00,0x3C,0x42,0x42,0x42,0x00},
														{0x00,0x7E,0x42,0x42,0x3C,0x00},
														{0x00,0x7E,0x4A,0x4A,0x4A,0x00},
														{0x00,0x7E,0x0A,0x0A,0x02,0x00},
														{0x00,0x3C,0x42,0x52,0x72,0x00},
														{0x00,0x7E,0x08,0x08,0x7E,0x00},
														{0x00,0x00,0x7E,0x00,0x00,0x00},
														{0x00,0x42,0x42,0x2E,0x02,0x00},
														{0x00,0x7E,0x08,0x24,0x62,0x00},
														{0x00,0x7E,0x40,0x40,0x40,0x00},
														{0x00,0x7E,0x04,0x08,0x04,0x7E},
														{0x00,0x7E,0x04,0x08,0x10,0x7E},
														{0x00,0x7E,0x42,0x42,0x7E,0x00},
														{0x00,0x7E,0x12,0x12,0x1E,0x00},
														{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}, 
														{0x00,0x7E,0x12,0x32,0x4C,0x00},
														{0x00,0x4E,0x4A,0x4A,0x7A,0x00},
														{0x00,0x02,0x02,0x7E,0x02,0x02},
														{0x00,0x7E,0x40,0x40,0x7E,0x00},
														{0x00,0x1E,0x20,0x40,0x20,0x1E},
														{0x00,0x7E,0x20,0x10,0x20,0x7E},
														{0x00,0x66,0x18,0x18,0x66,0x00},
														{0x00,0x02,0x04,0x78,0x04,0x02},
														{0x00,0x62,0x52,0x4A,0x46,0x00},
														{0x00,0x00,0x00,0x00,0x00,0x00}
};	


void itoa(unsigned char a,unsigned char b[2]){
	b[0]=a/10;
	b[1]=a%10;
	return;
}


int printXY(const U8 carattere[6],U8 pX,U8 pY){
	int i;
	if(pY>8 || pX>64)
		return -1;
#ifndef NO_LCD
	Lcd_Command_Left(0xb8 | pY);
	Lcd_Command_Left(0x40 | pX);
	for(i=0;i<6;i++)
		Lcd_Data_Left_WR(carattere[i]);
	#endif
	return 0;
}

int putcXY(char c,U8 X,U8 Y){
	if(c<='9' && c>='0'){
		c=c-'0';

		if(printXY(numero[c],X,Y)==0){
			X+=6;
			if(X>=60){
				X=0;
				Y++;
			}
			return 0;
		}
		else
			return -1;
	}
	if(c=='/'){
		if(printXY(numero[10],X,Y)==0){
			X+=6;
			if(X>=60){
				X=0;
				Y++;
			}
			return 0;
		}
		else
			return -1;
	}
	if(c<='Z' && c>='A')
		c=c-'A';
	else 
		if( c<='z' && c>='a')
			c=c-'a';
		else
			c=26;
		
	if(!printXY(lettera[c],X,Y)){
		X+=6;
		if(X>=60){
			X=0;
			Y++;
		}
		return 0;
	}
	else
		return -1;
}

int putc(char c){
	//carattere newline
	if(c==10){
		cursoreY=(cursoreY+1)%8;
		cursoreX+=6;
		if(cursoreX>=60){
			cursoreX=0;
			cursoreY++;
		}
		return 0;
	}
	//carattere carriage return
	if(c==13){
		cursoreX=0;
		return 0;
	}
	//numeri
	if(c<='9' && c>='0'){
		c=c-'0';
		if(printXY(numero[c],cursoreX,cursoreY)==0){
			cursoreX+=6;
			if(cursoreX>=60){
				cursoreX=0;
				cursoreY++;
			}
			return 0;
		}
		else
			return -1;
	}
	
	if(c<='Z' && c>='A'){
		c=c-'A';
	}else{
		if( c<='z' && c>='a')
			c=c-'a';
		else
			c=26;
	}
	if(!printXY(lettera[c],cursoreX,cursoreY)){
		cursoreX+=6;
		if(cursoreX>=60){
			cursoreX=0;
			cursoreY++;
		}
		return 0;
	}
	else
		return -1;
	
}

int puts(char *s,U8 riga){
	int i;
	cursoreY=(riga%8);
	cursoreX=0;
	for(i=0;i<10 && s[i]!=0;i++){
		putc(s[i]);
	}	
	return i--;
}

int init_countdown(U8 tempo){
	unsigned char stringa[6];
	unsigned char conto[2];
	int i;
	t=tempo;
	conto_attuale=tempo;
	itoa(tempo,conto);
	cursoreX=18;
	cursoreY=4;
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[0]][i];
	if(printXY(stringa,cursoreX,cursoreY)==-1)	
		return -1;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[1]][i];
	if(printXY(stringa,cursoreX+6,cursoreY)==-1)
		return -1;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[10][i];
	if(printXY(stringa,cursoreX+12,cursoreY)==-1)
		return -1;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[0]][i];
	if(printXY(stringa,cursoreX+18,cursoreY)==-1)
		return -1;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[1]][i];
	if(printXY(stringa,cursoreX+24,cursoreY)==-1)
		return -1;
	
	return 0;
}

int dec_countdown(){
	unsigned char stringa[6];
	unsigned char conto[2];
	int i;
	
	if(conto_attuale<=0)
		return -1;
	
	conto_attuale--;
	
	itoa(conto_attuale,conto);
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[0]][i];
	if(printXY(stringa,18,4)==-1)	
		return -1;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[1]][i];
	if(printXY(stringa,24,4)==-1)
		return -1;
	
	return 0;
}

int restart_countdown(){
	unsigned char stringa[6];
	unsigned char conto[2];
	int i;
	
	conto_attuale=t;
	
	itoa(t,conto);
	
	cursoreX=18;
	cursoreY=4;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[0]][i];
	if(printXY(stringa,cursoreX,cursoreY)==-1)	
		return -1;
	
	for(i=0;i<6;i++)
			stringa[i]=numero[conto[1]][i];
	if(printXY(stringa,cursoreX+6,cursoreY)==-1)
		return -1;
	
	return 0;
}
