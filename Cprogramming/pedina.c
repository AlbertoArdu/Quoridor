/*File di implementazione della libreria per la gestione della Pedina*/

#include "Pedina.h"

/*Funzione che inizializza la Pedina con il colore e la posizione iniziale*/
void initPedina(U8 i, U8 j, char color, Pedina *p){
	p->color = color;
	p->iprec = i;
	p->i = i;
	p->jprec = j;
	p->j = j;
	p->lPath=255;
	p->minPath[0]=NULL;
}

/*Funzione che sposta la Pedina nelle coordinate specificate*/
void muoviPedina(Pedina *p, U8 i, U8 j, U8 griglia[64][64]){
	
	rimuoviPedina(*p, griglia);
	p->i = i;
	p->j = j;
	disegnaPedina(*p, griglia);
	
}

/*Funzione che disegna la Pedina sulla griglia di gioco*/
void disegnaPedina(Pedina p, U8 griglia[64][64]){
	U8 ib, jb;
	U8 i, j;
	
	ib = 7*p.i + 2;
	jb = 7*p.j + 2;
	
	for(i = 0; i<4; i++)
	{
		for(j = 0; j<4; j++){
			
			if( (i ==0 && j==0) || (i==3 && j==0) || (i==0 && j ==3) || (i==3 && j ==3)){	
				griglia[i+ib][j+jb] = 0;
			} else if(p.color == 'b' && i>0 && i<3 && j>0 && j<3)
			{
				//Creo il buco per la Pedina bianca
				griglia[i+ib][j+jb] = 0;
			} else{
				griglia[i+ib][j+jb] = 255;
			}
		}
	}
}

/*Funzione che rimuove la Pedina dalla griglia di gioco*/
void rimuoviPedina(Pedina p, U8 griglia[64][64]){
	U8 ib, jb;
	U8 i, j;
	
	ib = 7*p.i + 2;
	jb = 7*p.j + 2;
	
	for(i = 0; i<4; i++)
	{
		for(j = 0; j<4; j++){
				griglia[i+ib][j+jb] = 0;
		}
	}
}
