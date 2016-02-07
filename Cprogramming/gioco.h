
#include "../lib_uCdragon/LPC2294.h"
#include "../lib_uCdragon/board.h"
#include "../lib_uCdragon/def.h"
#include "../lib_uCdragon/LCD/LCD_YM12864I.h"
#include "../lib_uCdragon/timers/timer.h"

#include "pedina.h"
#include "muro.h"
#include "gestioneStampaStringhe.h"

typedef struct casella Casella;
	
struct casella{
	Pedina *occupata;
	Casella *su, *giu, *destra, *sinistra;
	U8 i,j;
	U8 visitato;
	U8 pre;
	Casella* st;
	
};

typedef struct gioco{
	Casella scacchiera[9][9];
	Muro	elenco_muri[20];
	Pedina mia, avv;
	U8 num_muri;
	U8 num_miei_muri; //MOD ho aggiunto quanti muri può mettere ancora l'avversario e io, questo perchè devo controllare anche
	U8 num_avv_muri;	//MOD	che non tenti di fregarmi!
	U8 griglia[64][64];
} Gioco;

void init_gioco(Gioco *g,char mio_col);
int controlla_pedina(Gioco *g, Pedina *attuale,U8 i,U8 j);
int set_pedina(Gioco *g,Pedina *attuale,U8 i, U8 j);

int controlla_mossa(Gioco *g,U8 *movimento, U8 turno);		//data la stringa che descrive la mossa controlla se si può fare

int controllo_muro(Gioco *g,U8 i,U8 j,char o,U8 turno);
int set_muro(Gioco *g,U8 i,U8 j,char o,U8 turno); //controlla se può piazzarlo con controllo_muro se può lo piazza in g->elenco_muri e scacchiera e decrementa il numero dei muri

U8 percorso_minimo(Gioco *g, Pedina *p, int r);			//calcola il percorso minimo per la pedina p per arrivare alla riga r
																											//Ritorna la lunghezza minima
int percorsoValido(Gioco *g, Pedina *p);			//controlla se il percorso della pedina è valido

void scegli_muro(Gioco *g, Muro *m, U8 *time);
void visitaAmpiezza(Gioco *g, Casella *item);
