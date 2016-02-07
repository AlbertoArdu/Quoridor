
#include "../lib_uCdragon/LPC2294.h"
#include "../lib_uCdragon/board.h"
#include "../lib_uCdragon/def.h"

#define ELEMENTS 5

typedef struct buffer{
	U8 elements[ELEMENTS];
	U8 attesi;
	U8 pronto;
	U8 testa, coda;
    U8 faseIniziale;
} Buffer;


/*Funzione che inizializza il Buffer*/
void bufferInit(Buffer *b);

/*Funzione che si occupa di leggere i dati in arrivo e metterli nel buffer: al primo 
interrupt legge la prima lettera sa quanti caratteri deve aspettare.
Quando sono ricevuti tutti i caratteri imposta pronto il buffer che potrà quindi essere letto*/
void leggiUart(Buffer *b);

/*Funzione per scrivere su porta Uart: invia tramite la porta uart un carattere per volta
fino ad esaurire il buffer, per poi sbloccarlo e renderlo di nuovo disponibile*/
void scriviUart(Buffer *b);

/*Funzione che Inizializza La porta UART*/
void initUart(void);

/*Funzione che inserisce un elemento nel Buffer, ritorna 0 se Buffer pieno 1 se Ok*/
U8 inserisciBuffer(Buffer *b, U8 e);

/*Funzione che preleva un elemento dal Buffer, ritorna 255 se Buffer Vuoto*/
U8 prelevaBuffer(Buffer *b);

/*Funzione che ritorna 0 se buffer non pronto 1 se buffer pronto da leggere*/
U8 bufferPronto(Buffer *b);

/*Funzione che setta se la fase di gioco: 1 iniziale, 0 altre fasi*/
void settaFaseIniziale(Buffer *b, U8 iniziale);

/*Funzione che dice se ci si trova nella fase iniziale*/
U8 faseBuffer(Buffer *b);

/*Funzione per scrivere su porta Uart: invia tramite la porta uart un carattere per volta
 fino ad esaurire il buffer, per poi sbloccarlo e renderlo di nuovo disponibile*/
void scriviUartChar(U8 e);




