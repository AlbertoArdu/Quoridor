/*Implementazione della libreria per la gestione dei muri*/

#include "Muro.h"

U8 backup[3][14];

/*Funzione che inizializza il Muro con coordinate e orientamento iniziale*/
void initMuro(U8 i, U8 j, char orientamento, Muro *m){
	m->i = i;
	m->j = j;
	m->orientamento = orientamento;
}

/*Funzione che sposta il Muro nelle coordinate specificate*/
void muoviMuro(Muro *m, U8 i, U8 j, U8 griglia[64][64]){
	rimuoviMuro(m, griglia);
	m->i= i;
	m->j = j;
	disegnaMuro(m, griglia);
	
}

/*Funzione che si occupa di cmbiare l'orientamento del Muro*/
void orientaMuro(Muro *m, char orientamento, U8 griglia[64][64]){
	rimuoviMuro(m, griglia);
	m->orientamento = orientamento;
	disegnaMuro(m, griglia);
}

/*Funzione che disegna il Muro sulla griglia di gioco*/
void disegnaMuro(Muro *m, U8 griglia[64][64]){
	U8 ib, jb;
	U8 i, j;
		
	if(m->orientamento == 'o'){
		//Muro orizzontale
		
		ib = 7+7*m->i;
		jb = 7*m->j;
		
		//Salvo Backup
		for(i = 0; i<3; i++){
			for(j=0; j<14; j++){
				backup[i][j] = griglia[ib-1+i][jb+j];
			}
		}
		//Disegno Muro
		for (i = 1; i<14; i++){
			griglia[ib+1][jb+i] = 255;
			griglia[ib-1][jb+i] = 255;
		}
	} else if(m->orientamento == 'v'){
		//Muro verticale
		
		ib = 7*m->i;
		jb = 7+7*m->j;
		
		//Salvo Backup
		for(i = 0; i<14; i++){
			for(j=0; j<3; j++){
				backup[j][i] = griglia[ib+i][jb+j-1];
			}
		}
		//Disegno Muro
		for (i = 1; i<14; i++){
			griglia[ib+i][jb+1] = 255;
			griglia[ib+i][jb-1] = 255;
		}
	}
	
}

/*Funzione che rimuove il Muro dalla griglia di gioco*/
void rimuoviMuro(Muro *m, U8 griglia[64][64]){
	U8 ib, jb;
	U8 i, j;
	
	if(m->orientamento == 'v'){
		ib = 7*(m->i);
		jb = 7+7*(m->j);
		
		//Muro verticale
		//Ripristino Backup
		for(i = 0; i<14; i++){
			for(j=0; j<3; j++){
				griglia[ib+i][jb+j-1] = backup[j][i];
      }
    }
		
	} else if(m->orientamento == 'o'){
		ib = 7+7*(m->i);
		jb = 7*(m->j);
		
		//Muro orizzontale
		//Ripristino Backup
		for(i = 0; i<3; i++){
			for(j=1; j<14; j++){
				griglia[ib-1+i][jb+j] = backup[i][j];
			}
		}
	}	
}

void piazzaMuro(Muro *m,U8 griglia[64][64]){
	U8 ib,jb,i,j;
	
	if(m->orientamento == 'o'){
		//Muro orizzontale
		
		ib = 7+7*m->i;
		jb = 7*m->j;
		
		//Salvo Backup
		for(i = 0; i<3; i++){
			for(j=0; j<14; j++){
				backup[i][j] = griglia[ib-1+i][jb+j];
			}
		}
	} else if(m->orientamento == 'v'){
		//Muro verticale
		
		ib = 7*m->i;
		jb = 7+7*m->j;
		
		//Salvo Backup
		for(i = 0; i<14; i++){
			for(j=0; j<3; j++){
				backup[j][i] = griglia[ib+i][jb+j-1];
			}
		}
	}

}
