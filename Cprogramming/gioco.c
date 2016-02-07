#include "gioco.h"
#include "uart.h"
#include "coda.h"
#include "random.h"

#define N 3
U8 flag=1;
Coda c;
Muro muriPossibili[10];


void init_gioco(Gioco *gioco1,char mio_col){
	int i,j;
	
	gioco1->num_muri=0;
	if(mio_col=='b')
		initPedina(0,4,'n',&(gioco1->avv));
	else
		initPedina(0,4,'b',&(gioco1->avv));
	
	initPedina(8,4,mio_col,&(gioco1->mia));
	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			if(i==0 && j==4)
				gioco1->scacchiera[i][j].occupata = &gioco1->avv;
			else if(i==8 && j==4)
				gioco1->scacchiera[i][j].occupata = &gioco1->mia;
			else
				gioco1->scacchiera[i][j].occupata=0;
			
			gioco1->scacchiera[i][j].su=(void*)0;
			gioco1->scacchiera[i][j].giu=(void*)0;
			gioco1->scacchiera[i][j].destra=(void*)0;
			gioco1->scacchiera[i][j].sinistra=(void*)0;
			gioco1->scacchiera[i][j].i=i;
			gioco1->scacchiera[i][j].j=j;
			
			if(i>0)
				gioco1->scacchiera[i][j].su=&(gioco1->scacchiera[i-1][j]);
			if(i<8)
				gioco1->scacchiera[i][j].giu=&(gioco1->scacchiera[i+1][j]);
			if(j>0)
				gioco1->scacchiera[i][j].sinistra=&(gioco1->scacchiera[i][j-1]);
			if(j<8)
				gioco1->scacchiera[i][j].destra=&(gioco1->scacchiera[i][j+1]);
		}
	}
	for(i=0;i<20;i++)
		gioco1->elenco_muri[i].i=255;
	gioco1->num_avv_muri=10;
	gioco1->num_miei_muri=10;

	
}

int set_pedina(Gioco *gioco1,Pedina *attuale,U8 i, U8 j){
	int c;
	
	c=controlla_pedina(gioco1,attuale, i, j);
	
	if(c!=0)
		return c;
	gioco1->scacchiera[attuale->iprec][attuale->jprec].occupata=(void*)0;
	gioco1->scacchiera[i][j].occupata=attuale;
	attuale->iprec=i;
	attuale->jprec=j;
	
	muoviPedina(attuale, i, j, gioco1->griglia);
	
	return 0;
}

int controlla_pedina(Gioco *gioco1,Pedina *attuale,U8 i,U8 j){

	if(attuale->iprec==i && attuale->jprec==j){
		return -3;			//Stessa posizione di partenza
	}
	
	//Fuori dalla scacchiera
	if(i>8 || j>8)
		return -3;
	
	if(gioco1->scacchiera[i][j].occupata!=(void*)0){
		return -2;			//Casella occupata
	}
	
	if(	gioco1->scacchiera[attuale->iprec][attuale->jprec].su==&(gioco1->scacchiera[i][j])  ||
			gioco1->scacchiera[attuale->iprec][attuale->jprec].giu==&(gioco1->scacchiera[i][j])  ||
			gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra==&(gioco1->scacchiera[i][j])  ||
			gioco1->scacchiera[attuale->iprec][attuale->jprec].destra==&(gioco1->scacchiera[i][j])
		){
		return 0;
	}
	if(gioco1->scacchiera[attuale->iprec][attuale->jprec].su!=NULL){
		if(gioco1->scacchiera[attuale->iprec][attuale->jprec].su->occupata!=NULL){
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].su->su==&(gioco1->scacchiera[i][j])){
				return 0;
			}
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].su->su == NULL){
				if(gioco1->scacchiera[attuale->iprec][attuale->jprec].su->sinistra==&(gioco1->scacchiera[i][j]) ||
					gioco1->scacchiera[attuale->iprec][attuale->jprec].su->destra==&(gioco1->scacchiera[i][j]))
					return 0;
			}
		}	
	}
	if(gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra!=NULL){
		if(gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra->occupata!=NULL){
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra->sinistra==&(gioco1->scacchiera[i][j])){
				return 0;
			}
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra->sinistra==NULL){
				if(gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra->su==&(gioco1->scacchiera[i][j]) || 
					(gioco1->scacchiera[attuale->iprec][attuale->jprec].sinistra->giu==&(gioco1->scacchiera[i][j])))
					return 0;
			}
		}
	}
	if(gioco1->scacchiera[attuale->iprec][attuale->jprec].destra!=NULL){
		if(gioco1->scacchiera[attuale->iprec][attuale->jprec].destra->occupata!=NULL){
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].destra->destra==&(gioco1->scacchiera[i][j])){
				return 0;
			}
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].destra->destra==NULL){
				if(gioco1->scacchiera[attuale->iprec][attuale->jprec].destra->su==&(gioco1->scacchiera[i][j]) ||
					gioco1->scacchiera[attuale->iprec][attuale->jprec].destra->giu==&(gioco1->scacchiera[i][j]))
					return 0;
			}
		}
	}
	if(gioco1->scacchiera[attuale->iprec][attuale->jprec].giu!=NULL){
		if(gioco1->scacchiera[attuale->iprec][attuale->jprec].giu->occupata!=NULL){
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].giu->giu==&(gioco1->scacchiera[i][j])){
				return 0;
			}
			if(gioco1->scacchiera[attuale->iprec][attuale->jprec].giu->giu==NULL){
				if(gioco1->scacchiera[attuale->iprec][attuale->jprec].giu->sinistra==&(gioco1->scacchiera[i][j]) ||
					gioco1->scacchiera[attuale->iprec][attuale->jprec].giu->destra==&(gioco1->scacchiera[i][j]))
					return 0;
			}
		}
	}	
						
#ifdef Teraterm
	//scriviUartChar('1');	
#endif
	return -1;		//Casella non accessibile dalla posizione precedente

}

/***************************************************
*Function   :int controlla_mossa(Gioco *gioco1, char *movimento)
*Object     :controlla se la mossa e' valida
*Description:Riceve la stringa che descrive lo spostamento e la verifica; 
							se possibile lo setta altrimenti manda -1
*Parameter  :
*Return		:0 -> OK; -1 -> Errore
***************************************************/
int controlla_mossa(Gioco *gioco1,U8 *movimento,U8 turno){

	if(movimento[0]=='p'){
		if(turno)
			return set_pedina(gioco1,&(gioco1->mia),movimento[1],movimento[2]);
		else
			return set_pedina(gioco1,&(gioco1->avv),movimento[1],movimento[2]);
	}else if(movimento[0]=='b'){
			return set_muro(gioco1,movimento[1],movimento[2],movimento[3],turno);
	}else
	#ifdef Teraterm
	//scriviUartChar('5');
	#endif	
	return -5;
}

/***************************************************
*Function   :int controllo_muro(Gioco *g,U8 i,U8 j,char o)
*Object     :controlla se posso posizionare il muro nella posizione (i,j) con orientamento o
*Description:No
*Parameter  :char o= 'o' -> orizzontale; 'v' -> verticale;
*Return		:	0 -> Mossa consentita; -1 -> i,j errati; 
						-2 -> giocatore non ha abbastanza muri! -3 posizione occupata da un altro muro
						-4 -> muro blocca il giocatore!
***************************************************/
int controllo_muro(Gioco *g,U8 i,U8 j,char o,U8 turno){
	U8 x,y;
	/**                                                                                                           
	*controllo1
	*il muro esce dalla scacchiera:	NB indici dei muri [0-7]
	*/
	int k;
	Muro m;
	
	U8 trovato1=0, trovato2=0;
	
  if(i>=8 || j>=8){		
		return -1;
	}
	/*
	*controllo2
	*se il giocatore ha ancora muri disponibili:
	*controllo_muro la chiamo quando voglio piazzare un muro (con turno=0) o quando voglio piazzare un muro
	*che ha stabilito l'avversario
	*/
	if(turno==1 && g->num_miei_muri==0){
		return -2;
	}
	if(turno==0 && g->num_avv_muri==0){
		return -2;
	}
	/**
	*controllo3
	*se nella stessa posizione i,j è presente un altro muro o se si toccano e hanno lo stesso orientamento
	*/

	for(k=0;k<20;k++){
		m=g->elenco_muri[k];
			if(m.i!=255){
				if(m.i==i && m.j==j){
					//scriviUartChar('3');
					return -3;
				}
				if(m.orientamento=='v'){
					if(m.i-1==i && m.j==j && o=='v'){
						//scriviUartChar('3');
						return -3;
					}
					if(m.i+1==i && m.j==j && o=='v'){
						//scriviUartChar('3');
						return -3;
					}
				}else{
					if(m.i==i && m.j-1==j && o=='o'){
							//scriviUartChar('3');
							return -3;
					}
					if(m.i==i && m.j+1==j && o=='o'){
							//scriviUartChar('3');
							return -3;
					}
				}
			}else{
				break;
			}
    }
	/**
	*controllo4
	*blocco il giocatore? (ricorsivo! #_#)
	*/
	//Pulisco tutte le visite
	for(x=0;x<9;x++)
		for(y=0;y<9;y++)
			g->scacchiera[x][y].visitato=0;
	
	if(o=='o'){
		g->scacchiera[i][j].giu=(void*)0;
		g->scacchiera[i][j+1].giu=(void*)0;
		g->scacchiera[i+1][j].su=(void*)0;
		g->scacchiera[i+1][j+1].su=(void*)0;
	}else if(o=='v'){
		g->scacchiera[i][j].destra=(void*)0;
		g->scacchiera[i+1][j].destra=(void*)0;
		g->scacchiera[i][j+1].sinistra=(void*)0;
		g->scacchiera[i+1][j+1].sinistra=(void*)0;
	}else{ //orientamento non valido
		return -1;
	}
	
	trovato1=percorso_minimo(g,&(g->avv),8);
	trovato2=percorso_minimo(g,&(g->mia),0);
	
	if(o=='o'){
			g->scacchiera[i][j].giu=&(g->scacchiera[i+1][j]);
			g->scacchiera[i][j+1].giu=&(g->scacchiera[i+1][j+1]);
			g->scacchiera[i+1][j].su=&(g->scacchiera[i][j]);
			g->scacchiera[i+1][j+1].su=&(g->scacchiera[i][j+1]);
	}else{
			g->scacchiera[i][j].destra=&(g->scacchiera[i][j+1]);
			g->scacchiera[i+1][j].destra=&(g->scacchiera[i+1][j+1]);
			g->scacchiera[i][j+1].sinistra=&(g->scacchiera[i][j]);
			g->scacchiera[i+1][j+1].sinistra=&(g->scacchiera[i+1][j]);
	}

	if(trovato1==255 || trovato2==255){
    return -4;
	}	

	return 0;
}


int set_muro(Gioco *g,U8 i,U8 j,char o,U8 turno){
	int k;
	int ris;
	

	/**
	*posizionamento muro lecito:
	*	1-metto a null i 4 puntatori nella matrice di caselle;
	* 1b-controllo se valido
	*	+	se no rimetto i link allo stato precedente
	*	2-setto il muro nel vettori di muri del gioco;
	*	3-disegno il muro nel gioco;
	*	4-decremento il numero di muri disponibili;
	*/

	ris=controllo_muro(g,i,j,o,turno);
	if(ris==0){
		if(o=='o'){
			g->scacchiera[i][j].giu=(void*)0;
			g->scacchiera[i][j+1].giu=(void*)0;
			g->scacchiera[i+1][j].su=(void*)0;
			g->scacchiera[i+1][j+1].su=(void*)0;
		}else if(o=='v'){
			g->scacchiera[i][j].destra=(void*)0;
			g->scacchiera[i+1][j].destra=(void*)0;
			g->scacchiera[i][j+1].sinistra=(void*)0;
			g->scacchiera[i+1][j+1].sinistra=(void*)0;
		}else{ //orientamento non valido
			return -1;
		}
	}else{
		return ris;
	}
		//1 NB il primo muro vuoto è nella posizione: 20-num_miei_muri-num_avv_muri
	k=20-(g->num_avv_muri)-(g->num_miei_muri);
	g->elenco_muri[k].i=i;
	g->elenco_muri[k].j=j;
	g->elenco_muri[k].orientamento=o;
	
	//3 disegno il muro
	disegnaMuro(&(g->elenco_muri[20-g->num_avv_muri-g->num_miei_muri]),g->griglia);
	//4
	if(turno==1)
		g->num_miei_muri--;
	else
		g->num_avv_muri--;
	
	return 0;
}


void visitaAmpiezza(Gioco *g, Casella *item){
	
	item->st=item;	
	putItem(&c,item);
	item->pre=0;
	
	while(!isEmpty(&c)){
		item = getItem(&c);
		item->pre=item->st->pre+1;
		if(item->sinistra!=NULL){
			if(item->sinistra->occupata==NULL || !flag){
				if(item->sinistra->pre==255){
					item->sinistra->st = item;
					item->sinistra->pre = item->pre+1;
					if(!isFull(&c))
						putItem(&c,item->sinistra);
				}
			}else if(item->sinistra->sinistra!=NULL){
				if(item->sinistra->sinistra->pre==255){
					item->sinistra->sinistra->st = item;
					item->sinistra->sinistra->pre = item->pre+1;
					if(!isFull(&c))
						putItem(&c,item->sinistra->sinistra);
				}
			}else{
				if(item->sinistra->su!=NULL && flag){
					if(item->sinistra->su->pre==255){
						item->sinistra->su->st = item;
						item->sinistra->su->pre = item->pre+1;
						if(!isFull(&c))
							putItem(&c,item->sinistra->su);
					}
				}
				if(item->sinistra->giu!=NULL && flag){
					if(item->sinistra->giu->pre==255){
						item->sinistra->giu->st = item;
						item->sinistra->giu->pre = item->pre+1;
						if(!isFull(&c))
							putItem(&c,item->sinistra->giu);
					}
				}
			}
		}
		if(item->su!=NULL){
			if(item->su->occupata==NULL || !flag){
				if(item->su->pre==255){
					item->su->st = item;
					item->su->pre = item->pre+1;
					if(!isFull(&c))
						putItem(&c,item->su);
				}
			}else if(item->su->su!=NULL){
				if(item->su->su->pre==255){
					item->su->su->st = item;
					item->su->su->pre = item->pre+1;
					if(!isFull(&c))
						putItem(&c,item->su->su);
				}
			}else{
				if(item->su->destra!=NULL && flag){
					if(item->su->destra->pre==255){
						item->su->destra->st = item;
						item->su->destra->pre = item->pre+1;
						if(!isFull(&c))
							putItem(&c,item->su->destra);
					}
				}
				if(item->su->sinistra!=NULL && flag){
					if(item->su->sinistra->pre==255){
						item->su->sinistra->st = item;
						item->su->sinistra->pre = item->pre+1;
						if(!isFull(&c))
							putItem(&c,item->su->sinistra);
					}
				}
			}
		}
		if(item->destra!=NULL){
			if(item->destra->occupata==NULL || !flag){
				if(item->destra->pre==255){
					item->destra->pre = item->pre+1;
					item->destra->st = item;
					if(!isFull(&c))
						putItem(&c,item->destra);
				}
			}else if(item->destra->destra!=NULL){
				if(item->destra->destra->pre==255){
					item->destra->destra->pre = item->pre+1;
					item->destra->destra->st = item;
					if(!isFull(&c))
						putItem(&c,item->destra->destra);
				}
			}else{
				if(item->destra->su!=NULL && flag){
					if(item->destra->su->pre==255){
						item->destra->su->pre = item->pre+1;
						item->destra->su->st = item;
						if(!isFull(&c))
							putItem(&c,item->destra->su);
					}
				}
				if(item->destra->giu!=NULL && flag){
					if(item->destra->giu->pre==255){
						item->destra->giu->pre = item->pre+1;
						item->destra->giu->st = item;
						if(!isFull(&c))
							putItem(&c,item->destra->giu);
					}
				}
			}
		}
		if(item->giu!=NULL){
			if(item->giu->occupata==NULL || !flag){
				if(item->giu->pre==255){
					item->giu->pre = item->pre+1;
					item->giu->st = item;
					if(!isFull(&c))
						putItem(&c,item->giu);
				}
			}else if(item->giu->giu!=NULL){
				if(item->giu->giu->pre==255){
					item->giu->giu->pre = item->pre+1;
					item->giu->giu->st = item;
					if(!isFull(&c))
						putItem(&c,item->giu->giu);
				}
			}else{
				if(item->giu->destra!=NULL && flag){
					if(item->giu->destra->pre==255){
						item->giu->destra->pre = item->pre+1;
						item->giu->destra->st = item;
						if(!isFull(&c))
							putItem(&c,item->giu->destra);
					}
				}
				if(item->giu->sinistra!=NULL && flag){
					if(item->giu->sinistra->pre==255){
						item->giu->sinistra->pre = item->pre+1;
						item->giu->sinistra->st = item;
						if(!isFull(&c))
							putItem(&c,item->giu->sinistra);
					}
				}
			}
		}
	}
}

U8 percorso_minimo(Gioco *g, Pedina *p, int r){			//calcola il percorso minimo per la pedina p per arrivare alla riga r
	U8 i, j, min=81, jmin=255;
	Casella *arrivo;						//Casella di arrivo con percorso minimo

	for(i=0;i<9;i++){
		for(j=0;j<9;j++){
			g->scacchiera[i][j].pre = 255;
			g->scacchiera[i][j].st = NULL;
		}
	}	
	
	if(g->avv.i+1==g->mia.i && g->avv.j==g->mia.j)
		flag=1;
	else if(g->avv.i-1==g->mia.i && g->avv.j==g->mia.j)
		flag=1;
	else if(g->avv.i==g->mia.i && g->avv.j+1==g->mia.j)
		flag=1;
	else if(g->avv.i==g->mia.i && g->avv.j-1==g->mia.j)
		flag=1;
	else flag=0;
		
	initCoda(&c);
	
	visitaAmpiezza(g,&(g->scacchiera[p->iprec][p->jprec]));
	
	for(j=0;j<9;j++){
		if(g->scacchiera[r][j].pre<min){
			jmin=j;
			min=g->scacchiera[r][j].pre;
		}
	}
	if(jmin>8)
		return 255;
	
	arrivo=&(g->scacchiera[r][jmin]);
	initCoda(&c);
	
	while(arrivo->i != p->iprec || arrivo->j !=p->jprec){
		putItem(&c,arrivo);
		arrivo=arrivo->st;
	}
	i=0;
	while(!isEmpty(&c) && i<DIMPATH-1){
		p->minPath[i]=popItem(&c);
		i++;
	}
	p->minPath[i]=NULL;
	return i;
}

short prova_muro(Gioco *g,U8 i,U8 j,U8 o){
	short diff=-83;
	
	if(controllo_muro(g,i,j,o,1)==0){
		
		if(o=='o'){
			g->scacchiera[i][j].giu=(void*)0;
			g->scacchiera[i][j+1].giu=(void*)0;
			g->scacchiera[i+1][j].su=(void*)0;
			g->scacchiera[i+1][j+1].su=(void*)0;
		}else{
			g->scacchiera[i][j].destra=(void*)0;
			g->scacchiera[i+1][j].destra=(void*)0;
			g->scacchiera[i][j+1].sinistra=(void*)0;
			g->scacchiera[i+1][j+1].sinistra=(void*)0;
		}

		g->avv.lPath = percorso_minimo(g,&(g->avv),8);
		g->mia.lPath = percorso_minimo(g,&(g->mia),0);

		if(g->avv.lPath!=255 && g->mia.lPath!=255){
			diff=g->avv.lPath-g->mia.lPath;
		}
		
		if(o=='o'){
			g->scacchiera[i][j].giu=&(g->scacchiera[i+1][j]);
			g->scacchiera[i][j+1].giu=&(g->scacchiera[i+1][j+1]);
			g->scacchiera[i+1][j].su=&(g->scacchiera[i][j]);
			g->scacchiera[i+1][j+1].su=&(g->scacchiera[i][j+1]);
		}else{
			g->scacchiera[i][j].destra=&(g->scacchiera[i][j+1]);
			g->scacchiera[i+1][j].destra=&(g->scacchiera[i+1][j+1]);
			g->scacchiera[i][j+1].sinistra=&(g->scacchiera[i][j]);
			g->scacchiera[i+1][j+1].sinistra=&(g->scacchiera[i+1][j]);
		}
		
	}

	return diff;
}




void scegli_muro(Gioco *g, Muro *m, U8 *time){
	U8 i, j, x,k=0;
	void *backupAvv[DIMPATH+1];
	short diffMax=(g->avv.lPath)-(g->mia.lPath), diffAtt;
	int randx;
	m->i = 255;
	muriPossibili[0].i = 255;

	backupAvv[0] = &(g->scacchiera[g->avv.i][g->avv.j]);
	
	//Salvo backup Percorso attuale dell'avverario
	for(i=1; g->avv.minPath[i-1] != NULL; i++)
	{
		backupAvv[i] = g->avv.minPath[i-1];
	}	
	
	backupAvv[i] = NULL;
	
	//Vedo scorrendo il backup quali sono le caselle che non fanno parte del mio percorso,
	//se ne trovo una provo a mettere il muro, calcolo il percorso dell'avversario, se Ë pi? lungo memorizzo questo muro
	//se il conto alla rovescia Ë ancora superiore a 5(?) provo a cercarne un altro.
	
	for(x = 1; backupAvv[x]!= NULL; x++){
		//Controllo di Avere Tempo
		if(*time < 5)
			return;
		i=((Casella*)(backupAvv[x]))->i;
		j=((Casella*)(backupAvv[x]))->j;
		
		//Controllo come l'aversario ci arriva a questa casella
		if(((Casella*)backupAvv[x-1])->su != NULL){
			if(backupAvv[x] == ((Casella*)backupAvv[x-1])->su || backupAvv[x] == ((Casella*)backupAvv[x-1])->su->su){
				diffAtt=prova_muro(g,i,j,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
					}
				}	
				diffAtt=prova_muro(g,i,j-1,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if (diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
					}
				}				
			}else if(backupAvv[x] == ((Casella*)backupAvv[x-1])->su->destra){
				diffAtt=prova_muro(g,i,j-1,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
					}
				}	
			}else if(backupAvv[x] == ((Casella*)backupAvv[x-1])->su->sinistra){
				diffAtt=prova_muro(g,i,j,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
					}
				}	
			}
		}
		if(((Casella*)backupAvv[x-1])->destra != NULL){
			if(backupAvv[x] == ((Casella*)backupAvv[x-1])->destra || backupAvv[x] == ((Casella*)backupAvv[x-1])->destra->destra){
				diffAtt=prova_muro(g,i-1,j-1,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if (diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
					}
				}	
				diffAtt=prova_muro(g,i,j-1,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
					}
				}
		} else if(backupAvv[x] == ((Casella*)backupAvv[x-1])->destra->su){
				diffAtt=prova_muro(g,i-1,j,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
					}
				}	
		
		}else if(backupAvv[x] == ((Casella*)backupAvv[x-1])->destra->giu){
				diffAtt=prova_muro(g,i-1,j-1,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
					}
				}
			}		
		}
		if(((Casella*)backupAvv[x-1])->sinistra != NULL){
			if(backupAvv[x] == ((Casella*)backupAvv[x-1])->sinistra || backupAvv[x] == ((Casella*)backupAvv[x-1])->sinistra->sinistra){
				diffAtt=prova_muro(g,i-1,j,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if (diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
					}
				}	
				diffAtt=prova_muro(g,i,j,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
					}
				}
		}else if( backupAvv[x] == ((Casella*)backupAvv[x-1])->sinistra->su){
				diffAtt=prova_muro(g,i,j,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';	
					}
				}	
		}else if( backupAvv[x] == ((Casella*)backupAvv[x-1])->sinistra->giu){
				diffAtt=prova_muro(g,i,j-1,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
					}
				}	
			}
		}
		if(((Casella*)backupAvv[x-1])->giu != NULL){
			if(backupAvv[x] == ((Casella*)backupAvv[x-1])->giu || backupAvv[x] == ((Casella*)backupAvv[x-1])->giu->giu){
				diffAtt=prova_muro(g,i-1,j,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='o';
					}
				}	
				diffAtt=prova_muro(g,i-1,j-1,'o');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
						diffMax=diffAtt;
					}else if (diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='o';
					}
				}	
			}else if(backupAvv[x] == ((Casella*)backupAvv[x-1])->giu->destra){
				diffAtt=prova_muro(g,i-1,j-1,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j-1;
						muriPossibili[k].orientamento='v';
					}
				}	
			}else if(backupAvv[x] == ((Casella*)backupAvv[x-1])->giu->sinistra){
				diffAtt=prova_muro(g,i-1,j-1,'v');
				if(diffAtt>-82){
					if(diffAtt>diffMax){
						k=0;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
						diffMax=diffAtt;
					}else if(diffAtt==diffMax && g->avv.i>=N){
						k++;
						muriPossibili[k].i=i-1;
						muriPossibili[k].j=j;
						muriPossibili[k].orientamento='v';
					}
				}
			}
		}
	}
	
	if(k==0){
		m->i = muriPossibili[k].i;
		m->j = muriPossibili[k].j;
		m->orientamento = muriPossibili[k].orientamento;
	} else{
		srand(T0TC);
		randx = rand();
		randx = randx%((int)(k+1));
		m->i = muriPossibili[randx].i;
		m->j = muriPossibili[randx].j;
		m->orientamento = muriPossibili[randx].orientamento;
	}
	
	return;
}
