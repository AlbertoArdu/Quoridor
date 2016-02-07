#include "uart.h"

#define DLAB  0x00000080
#define U0LCR_val 0x00000003
#define U0IER_val 0x00000001
#define U0DLL_val 0x60
#define U0DLM_val 0x00


/*Funzione che inizializza il Buffer*/
void bufferInit(Buffer *b){
    
    b->attesi = 0;
    b->pronto = 0; //No
    b->coda = 0;
    b->testa = 0;
    b->faseIniziale = 1;
}

/*Funzione che inserisce un elemento nel Buffer, ritorna 0 se Buffer pieno, 1 se Ok*/
U8 inserisciBuffer(Buffer *b, U8 e){
        
    if(b->coda == b->testa-1 || (b->coda == ELEMENTS-1 && b->testa == 0))
        return 0;		//Pieno
    
    b->elements[b->coda] = e;
    b->coda++;
		b->coda %= ELEMENTS;
    
    return 1;
}

/*Funzione che preleva un elemento dal Buffer, ritorna 255 se Buffer Vuoto*/
U8 prelevaBuffer(Buffer *b){
  
		U8 e;
	
		
	
    if(b->coda == b->testa){
        b->pronto = 0;
        return 255;
    }
    e = b->elements[b->testa++];
		b->testa %= ELEMENTS;
    return e;
}

/*Funzione che ritorna 0 se buffer non pronto 1 se buffer pronto da leggere*/
U8 bufferPronto(Buffer *b){
    return b->pronto;
}

/*Funzione che Inizializza La porta UART*/
void initUart(){
    
    VICIntEnable=0x00020050;
    VICIntSelect=0x00000050;
    
    U0LCR = DLAB;
    U0DLL = U0DLL_val;
    U0DLM = U0DLM_val;
    U0LCR = U0LCR_val;
    U0IER = U0IER_val;
    
}

/*Funzione che si occupa di leggere i dati in arrivo e metterli nel buffer: al primo
 interrupt blocca il buffer e leggendo la prima lettera sa quanti caratteri deve aspettare.
 Quando sono ricevuti tutti i caratteri sblocca il buffer che potrˆ quindi essere letto*/
void leggiUart(Buffer *b){
    
    U8 e;
    
    //Controllo se Buffer pu˜ ricevere dati
    if(b->pronto != 0)
        return;
    
    //Leggo carattere
    e = U0RBR;
    
    if(b->faseIniziale && e == 'b'){
        b->attesi=1;
    }else{
			//Se attesi uguale 0 devo leggere il primo carattere per sapere quanti caratteri aspettare
			if(b->attesi == 0){
					switch(e){
							case 'p':
									b->attesi=3;
									break;
							case 'b':
									b->attesi=4;
									break;
							case 'e':
									b->attesi=1;
									break;
							case 'n':
									b->attesi=1;
									break;
							default:
									return;
					}
			}
		}
		
    inserisciBuffer(b, e);
    b->attesi--;
    
    if(b->attesi == 0)
        b->pronto = 1;
}


/*Funzione per scrivere su porta Uart: Scrive sulla UART tutti i caratteri del buffer*/
void scriviUart(Buffer *b){
    
    U8 e;
    while((e = prelevaBuffer(b)) != 255){
        
        while(!(U0LSR & 0x00000020));
        U0THR = e;
    }
    
}

/*Funzione per scrivere su porta Uart: invia tramite la porta uart un carattere per volta
 fino ad esaurire il buffer, per poi sbloccarlo e renderlo di nuovo disponibile*/
void scriviUartChar(U8 e){
    
    U0THR = e;
}

/*Funzione che setta se la fase di gioco: 1 iniziale, 0 altre fasi*/
void settaFaseIniziale(Buffer *b, U8 iniziale){
    b->faseIniziale = iniziale;
}

/*Funzione che dice se ci si trova nella fase iniziale*/
U8 faseBuffer(Buffer *b){
    return b->faseIniziale;
}
