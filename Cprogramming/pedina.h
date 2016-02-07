/*Header della libreria per la gestione delle pedine*/

#include "../lib_uCdragon/def.h"
#define DIMPATH 50
/*Struttura della variabile Pedina*/
typedef struct Pedina{
	char color;
	U8 i, j;
	U8 iprec,jprec;
	void* minPath[DIMPATH];
	U8 lPath;
} Pedina;

/*Funzione che inizializza la Pedina con il colore e la posizione iniziale*/
void initPedina(U8 i, U8 j, char color, Pedina *p);

/*Funzione che sposta la Pedina nelle coordinate specificate*/
void muoviPedina(Pedina *p, U8 i, U8 j, U8 griglia[64][64]);

/*Funzione che disegna la Pedina sulla griglia di gioco*/
void disegnaPedina(Pedina p, U8 griglia[64][64]);

/*Funzione che rimuove la Pedina dalla griglia di gioco*/
void rimuoviPedina(Pedina p, U8 griglia[64][64]);
