/*Header della libreria che definisce la variabile Muro*/

#include "../lib_uCdragon/def.h"

/*Struttura della variabile Pedina*/
typedef struct Muro{
	char orientamento;
	U8 i, j;
} Muro;

/*Funzione che inizializza il Muro con coordinate e orientamento iniziale*/
void initMuro(U8 i, U8 j, char orientamento, Muro *m);

/*Funzione che sposta il Muro nelle coordinate specificate*/
void muoviMuro(Muro *m, U8 i, U8 j, U8 griglia[64][64]);

/*Funzione che si occupa di cmbiare l'orientamento del Muro*/
void orientaMuro(Muro *m, char orientamento, U8 griglia[64][64]);

/*Funzione che disegna il Muro sulla griglia di gioco*/
void disegnaMuro(Muro *m, U8 griglia[64][64]);

/*Funzione che rimuove il Muro dalla griglia di gioco*/
void rimuoviMuro(Muro *m, U8 griglia[64][64]);

/*Funzione che conferma il muro piazzato*/
void piazzaMuro(Muro *m,U8 griglia[64][64]);
