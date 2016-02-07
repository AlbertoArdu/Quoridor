//*******************************************************************************************
//*    			   																																							*
//*******************************************************************************************
//* Author: Politecnico di Torino																														*
//*******************************************************************************************
//*******************************************************************************************
//
//*******************************************************************************************

#include "../lib_uCdragon/LPC2294.h"
#include "../lib_uCdragon/board.h"
#include "../lib_uCdragon/def.h"
#include "../lib_uCdragon/LCD/LCD_YM12864I.h"
#include "../lib_uCdragon/timers/timer.h"


#include "gestioneStampaStringhe.h"
#include "gioco.h"
#include "uart.h"


//#define Teraterm
#define TIME_MAX 20

/*Funzione che inizializza la griglia di gioco alla partenza*/
void creaGriglia(U8 griglia[64][64]);

/*Funzione che inizializza le info*/
void infoInit(U8 info[64][64]);

/*Funzione che disegna i muri a disposizione*/
void muriInfo(U8 info[64][64]);

/*FUNZIONI CHE CONTROLLANO I PULSANTI*/
void s2_control(void);
void s3_control(void);
void s4_control(void);
void s5_control(void);
void s6_control(void);
void s7_control(void);
void s8_control(void);
void s9_control(void);

void decisioneColore(void);

void controlla_vittoria(void);

void avantiPedina(void);

void inviaMossa(U8 *);

/*Definizione Variabili Globali*/
volatile U8 automatico=255;

U8 info[64][64]; //Lato sx del display

Muro m; //Muro in posizionamento


U8 Pedina_Muro = 0; //Controllo Pedina(0) Muro(1);
Gioco gioco1;
volatile U8 vittoria=0;			//1-> io ; 2-> avversario;
//Poiché la premuta di s9 è asincrona tengo una variabile che memorizza la mossa
//fino a questo punto e quando viene premuto viene scritta sull'uart;
//la prima mossa è il punto in cui inizio il gioco
U8 mossa_corrente[5]={'p',8,4,255,255};			
U8 mossa_ricevuta[5];
/*conto*/
U8 contoRovescia=0;

/*VARIABILI DI STATO DEI PULSANTI*/
U8 s2_state = 0;
U8 s3_state = 0;
U8 s4_state = 0;
U8 s5_state = 0;
U8 s6_state = 0;
U8 s7_state = 0;
U8 s8_state = 0;
U8 s9_state = 0;

//Buffer
Buffer b_ric, b_inv;
volatile char mio_colore=0;
U8 mio_turno=0;
U8 n_turni=0;  //conta il numero di turni
U8 timeout_mossa=TIME_MAX;

volatile char flagInt9=1;

char messaggio1[] ="Premi il  ";
char messaggio2[] ="bottone s4";
char messaggio3[] ="Sei nero  ";
char messaggio4[] ="Sei bianco";
char messaggio5[] ="VAI       ";
char messaggio6[] ="ASPETTA   ";
char messaggio7[] ="Hai vinto ";
char messaggio8[] ="Hai perso ";
char messaggio9[] ="Errore    ";
char messaggio11[]="  mossa   ";
char messaggio12[]="  errata  ";
char messaggio13[]="S2        ";
char messaggio14[]="automatico";
char messaggio15[]="s5        ";
char messaggio16[]="manuale   ";
char svuotaRiga[]= "          ";


int main(){

	//INIZIALIZZAZIONI VARIE
	{
	TargetResetInit();
	PortInit();
	ExternalInterruptInit();
	LedSet(0xFF);		//Spengo i led
	init_timer();

	//richiamo la funzione che si occupa di creare la griglia
	creaGriglia(gioco1.griglia);

	
#ifndef NO_LCD
	//Inizializzo il display
	lcdStart();
	Lcd_Clear();
	
#endif
	
	//Inizializzo il buffer dell'uart
	bufferInit(&b_ric);
	settaFaseIniziale(&b_ric,1);
	bufferInit(&b_inv);
	initUart();	
}
	//Inizia il gioco!!
	//Decidi se auto o manuale

	puts(messaggio13,2);
	puts(messaggio14,3);
	puts(messaggio15,5);
	puts(messaggio16,6);

	do{
		PCON = 0x01;
	}while(automatico==255);
	
	//Patteggiamento colore

		puts(svuotaRiga, 3);
		puts(svuotaRiga, 2);

	{
	do{		//Tutti e due inviamo 'b'
		puts(messaggio1, 5);
		puts(messaggio2, 6);

		mio_colore=0;
		while(mio_colore==0 || mio_colore=='p')
			PCON = 0x01;						//se non è ancora stato deciso o non è stato confermato aspetta
	}while(mio_colore=='e');

	settaFaseIniziale(&b_ric,0);
	
#ifndef NO_LCD
	Lcd_Clear_Left();
#endif


	if(mio_colore=='n')
		puts(messaggio3, 7);
	if(mio_colore=='b'){
		puts(messaggio4, 7);

		mio_turno=1;
	}
	}
	//Inizializza la struttura del gioco
	init_gioco(&gioco1,mio_colore);
	//Posiziono Pedine
	disegnaPedina(gioco1.mia, gioco1.griglia);
	disegnaPedina(gioco1.avv, gioco1.griglia);
	//Funzione che crea sx display
	infoInit(info);
#ifndef NO_LCD
	lcdPrintDx(gioco1.griglia);
	lcdPrintSxUP(info);
#endif	
	init_countdown(TIME_MAX);



	while(vittoria==0){
		if(mio_turno){
			puts(messaggio5,3);
			if(automatico){ // ******* DECISIONE MOSSA ********* //
				if((gioco1.mia.lPath=percorso_minimo(&gioco1,&(gioco1.mia),0))==255)
					; // non trova un percorso
				if((gioco1.avv.lPath=percorso_minimo(&gioco1,&(gioco1.avv),8))==255)
					; // non trova un perscorso
				
				if(gioco1.mia.lPath<gioco1.avv.lPath || n_turni<2 || gioco1.num_miei_muri<=0){
					avantiPedina();
				}else{ // decidi il muro
					scegli_muro(&gioco1,&m,&timeout_mossa);
					if(m.i==255){
						avantiPedina();
					}else{
						mossa_corrente[0]='b';
						mossa_corrente[1]=m.i;
						mossa_corrente[2]=m.j;
						mossa_corrente[3]=m.orientamento;
						mossa_corrente[4]=255;	
						piazzaMuro(&m,gioco1.griglia);
					}
				}
				//************* INVIO MOSSA **************//
				if(controlla_mossa(&gioco1,mossa_corrente,mio_turno)==0){
					while(timeout_mossa>19) PCON=0x01;
					inviaMossa(mossa_corrente);
					muriInfo(info);
					#ifndef NO_LCD
					lcdPrintSxUP(info);
					#endif
				}
			}
		}else{
			puts(messaggio6,3);

		}

		
	
		
		PCON = 0x01;				//idle state
	}
	
	//FINE GIOCO

	if(vittoria==1)
		puts(messaggio7,3);
	if(vittoria==2)
		puts(messaggio8,3);
	if(vittoria==255){
		puts(messaggio9,2);
	}


	while(1)
		PCON = 0x01;
	
	return 0;
}

/*********************************************************************
*Function   :IRQ_Exception(void)
*Object     :IRQ exception interrupt handle function
*Description:No
*Parameter	:void
*Return		:No
**********************************************************************/
__irq void IRQ_Handler( void )
{

	if(VICIRQStatus == 0x00020000){
			if(s9_state==0){
				s9_state++;
				EXTINT = 0x08;
				VICIntEnClr = 0x00020000;
				PINSEL1 = 0x05454400;
				flagInt9=0;
			}
	}
}

/*********************************************************************
*Function   :FIQ_Exception(void)
*Object     :FIQ exception interrupt handle function
*Description:No
*Parameter	:void
*Return		:No
**********************************************************************/
__irq void FIQ_Handler( void ){
	U8 i;
	//Controllo sorgente interrupt*/
	if(VICFIQStatus & 0x00000010)
	{
		//Interrupt del timer 0
		reset_intr_timer0(1);

		if(b_ric.faseIniziale==0){
			if(contoRovescia==25){ //Conta i 40ms
				if(timeout_mossa!=0){	//Conta TIME_MAX s
					timeout_mossa--;
					dec_countdown();
				}else{						//Timer scaduto!!
					if(mio_turno)
						vittoria=2;
					else
						vittoria=1;
				}
				contoRovescia =0;
			}else contoRovescia++;
		}
		if(automatico!=1){
			s2_control();
			s3_control();		
			s5_control();
			s6_control();
			s7_control();
			s8_control();
			s9_control();
		}
		if(mio_colore==0)
			s4_control();
		
	}
	
	if(VICFIQStatus & 0x00000040){ //Interrupt dell'uart0
		//Interrupt della Porta Uart
		
		//Controllo se Ricezione
		if(U0IIR & 0x00000004){
			leggiUart(&b_ric);
			if(bufferPronto(&b_ric)){
				//Interpreto Comando ricevuto e richiamo funzioni di controllo
				
				if(b_ric.faseIniziale)
					decisioneColore();
				else{
					if(!mio_turno){
						for(i=0;i<5 ;i++){
							mossa_ricevuta[i]=prelevaBuffer(&b_ric);
							if(mossa_ricevuta[i]==255)break;
						}
						if(mossa_ricevuta[0]=='e'){		//mossa non riconosciuta dall'avversario
							vittoria=255;
							return;
						}
						
						//Stampo su LCD la mossa ricevuta
						#ifndef Teraterm
						mossa_ricevuta[1]+='0';
						mossa_ricevuta[2]+='0';
						#endif
				
						for(i=0;mossa_ricevuta[i]!=255 && i<5 ;i++){
							putcXY(mossa_ricevuta[i],i*6,6);
						}
						
						mossa_ricevuta[1]-='0';
						mossa_ricevuta[2]-='0';
						
						if(mio_colore == 'n'){
							if(mossa_ricevuta[0]=='p'){
								mossa_ricevuta[1] = 8 - mossa_ricevuta[1];
								mossa_ricevuta[2] = 8 - mossa_ricevuta[2];
							}else{
								mossa_ricevuta[1] = 7 - mossa_ricevuta[1];
								mossa_ricevuta[2] = 7 - mossa_ricevuta[2];
							}
						}

						if(controlla_mossa(&gioco1,mossa_ricevuta,mio_turno)==0){
#ifndef NO_LCD
							lcdPrintDx(gioco1.griglia);
#endif
							controlla_vittoria();
							mio_turno=1;
							contoRovescia=0;
							timeout_mossa=TIME_MAX;
							Pedina_Muro=0;
							mossa_corrente[0]='p';
							mossa_corrente[1]=gioco1.mia.iprec;
							mossa_corrente[2]=gioco1.mia.jprec;
							mossa_corrente[3]=255;

							restart_countdown();
						}else{
							scriviUartChar('e');
							vittoria=255;
							
						}
					}
				}
			}
		}
	}
	if(vittoria!=0){
		//Disabilito Interrupt
		VICIntEnClr = 0x00020050;
	}
}

//ritorna: 0-> nessuno ha ancora vinto; 1-> vinco io; 2-> vince avv;
void controlla_vittoria(void){
	if(gioco1.mia.iprec==0)
		vittoria=1;
	if(gioco1.avv.iprec==8)
		vittoria=2;
}
/*FUNZIONI CHE CONTROLLANO I PULSANTI*/
void s2_control(){
	//Controllo se Pulsante 2 premuto
		if(!(IO2PIN & 0x00010000)){
			if(s2_state == 0){
				//Prima premuta
				s2_state++;
			} else if(s2_state == 1){
				//Seconda premuta
				//AZIONE -> cambio orientamento del Muro
				//ATTENZIONE: una volta sviluppato il controllo del gioco è necessario controllare prima se mossa fattibile
				if(automatico!=255){
					if(mio_turno){
						if(Pedina_Muro){
							if(m.orientamento == 'v'){
								orientaMuro(&m, 'o', gioco1.griglia);
	#ifndef NO_LCD
								lcdPrintDx(gioco1.griglia);
	#endif
								mossa_corrente[3]='o';
							}
							else{
								orientaMuro(&m, 'v', gioco1.griglia);
	#ifndef NO_LCD
								lcdPrintDx(gioco1.griglia);
	#endif
								mossa_corrente[3]='v';
							}
						}
					}
				}else{
					automatico=1;
				}
				s2_state++;
			}
		} else{
			s2_state = 0;
		}
}

void s3_control(){
	//Controllo se Pulsante 3 premuto
		if(!(IO2PIN & 0x00020000)){
			if(s3_state == 0){
				//Prima premuta
				s3_state++;
			} else if(s3_state == 1){
				//Seconda premuta
				//AZIONE -> sposto la mia Pedina di una posizione in su
				//ATTENZIONE: una volta sviluppato il controllo del gioco è necessario controllare prima se mossa fattibile
				if(mio_turno){
					if(!Pedina_Muro){
						if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].su!=NULL){
							if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].su->occupata==&(gioco1.avv)){
								if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].su->su!=NULL){
									muoviPedina(&(gioco1.mia), gioco1.mia.i-2, gioco1.mia.j, gioco1.griglia);
									mossa_corrente[1]=gioco1.mia.i;
									mossa_corrente[2] = gioco1.mia.j;

									puts(svuotaRiga,5);
									puts(svuotaRiga,6);	
						
								}else{

									puts(messaggio11,5);
									puts(messaggio12,6);		//Non si può andare su

								}
							} else{
								muoviPedina(&(gioco1.mia), gioco1.mia.i-1, gioco1.mia.j, gioco1.griglia);
								mossa_corrente[1] = gioco1.mia.i;
								mossa_corrente[2] = gioco1.mia.j;
			
								puts(svuotaRiga,5);
								puts(svuotaRiga,6);	
					
							}
						}else{
							puts(messaggio11,5);
							puts(messaggio12,6);		//Non si può andare su
						}
					} else {
						if(m.i>0){
							muoviMuro(&m, m.i-1, m.j, gioco1.griglia);
							mossa_corrente[1] = m.i;
							mossa_corrente[2] = m.j;
						}
					}
#ifndef NO_LCD					
					lcdPrintDx(gioco1.griglia);
#endif
				}
				
				s3_state++;
			}
		} else{
			s3_state = 0;
		}
}



void s4_control(){
	//Controllo se Pulsante 3 premuto
		if(!(IO2PIN & 0x00040000)){
			if(s4_state == 0){
				//Prima premuta
				s4_state++;
			} else if(s4_state == 1){
				//Seconda premuta
				//AZIONE ->controlla se è arrivato 'b' altrimenti lo invia 'b' sull'uart
				if(inserisciBuffer(&b_inv,'b')==1){
					scriviUart(&b_inv);
					mio_colore='p';				//Inviato ma non confermato
				}
				s4_state++;
			}
		} else{
			s4_state = 0;
		}
}

void s5_control(){
	//Controllo se Pulsante 5 premuto
		if(!(IO2PIN & 0x00080000)){
			if(s5_state == 0){
				//Prima premuta
				s5_state++;
			} else if(s5_state == 1){
				//Seconda premuta
				//AZIONE -> Scambio Pedina/Muro
				if(automatico!=255){
					if(mio_turno){
						if(!Pedina_Muro && gioco1.num_miei_muri>0){
							initMuro(7, 0, 'o', &m);
							mossa_corrente[0]='b';
							mossa_corrente[1]=7;
							mossa_corrente[2]=0;
							mossa_corrente[3]='o';						
							disegnaMuro(&m, gioco1.griglia);
							muoviPedina(&gioco1.mia,gioco1.mia.iprec,gioco1.mia.jprec,gioco1.griglia);
							Pedina_Muro = 1;
						} else{
							mossa_corrente[0]='p';
							mossa_corrente[3]=255;
							rimuoviMuro(&m, gioco1.griglia);
							Pedina_Muro = 0;
							
						}
	#ifndef NO_LCD
						lcdPrintDx(gioco1.griglia);
	#endif			
					}
				} else{
					automatico=0;
				}
				s5_state++;
			}
		} else{
			s5_state = 0;
		}
}

void s6_control(){
	//Controllo se Pulsante 6 premuto
		if(!(IO2PIN & 0x00100000)){
			if(s6_state == 0){
				//Prima premuta
				s6_state++;
			} else if(s6_state == 1){
				//Seconda premuta
				//AZIONE -> Sposto la Pedina o il Muro a sinistra di una posizione
				//ATTENZIONE: una volta sviluppato il controllo del gioco è necessario controllare prima se mossa fattibile
				if(mio_turno){
					if(!Pedina_Muro){
						if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].sinistra != NULL){
							if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].sinistra->occupata==&(gioco1.avv)){
								if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].sinistra->sinistra != NULL){
									muoviPedina(&(gioco1.mia), gioco1.mia.i, gioco1.mia.j-2, gioco1.griglia);
									mossa_corrente[1]=gioco1.mia.i;
									mossa_corrente[2] = gioco1.mia.j;									
									puts(svuotaRiga,5);
									puts(svuotaRiga,6);
								}else{		
									puts(messaggio11,5);
									puts(messaggio12,6);		//Non si può andare a sinistra			
								}
							} else{
								muoviPedina(&(gioco1.mia), gioco1.mia.i, gioco1.mia.j-1, gioco1.griglia);
								mossa_corrente[1]=gioco1.mia.i;
								mossa_corrente[2] = gioco1.mia.j;		
								puts(svuotaRiga,5);
								puts(svuotaRiga,6);		
							}
						}else{		
							puts(messaggio11,5);
							puts(messaggio12,6);		//Non si può andare a sinistra
						}
					}else{
						if(m.j>0){
							muoviMuro(&m, m.i, m.j-1, gioco1.griglia);
							mossa_corrente[1] = m.i;
							mossa_corrente[2] = m.j;
						}
					}
#ifndef NO_LCD
					lcdPrintDx(gioco1.griglia);
#endif
				}
				s6_state++;
			}
		} else{
			s6_state = 0;
		}
}

void s7_control(){
	//Controllo se Pulsante 7 premuto
		if(!(IO2PIN & 0x00200000)){
			if(s7_state == 0){
				//Prima premuta
				s7_state++;
			} else if(s7_state == 1){
				//Seconda premuta
				//AZIONE -> Sposto la mia Pedina o il Muro di una posizione in giu
				//ATTENZIONE: una volta sviluppato il controllo del gioco è necessario controllare prima se mossa fattibile
				if(mio_turno){
					if(!Pedina_Muro){
						if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].giu!=NULL){
							if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].giu->occupata==&(gioco1.avv)){
								if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].giu->giu != NULL){
									muoviPedina(&(gioco1.mia), gioco1.mia.i+2, gioco1.mia.j, gioco1.griglia);
									mossa_corrente[1]=gioco1.mia.i;
									mossa_corrente[2] = gioco1.mia.j;
									puts(svuotaRiga,5);
									puts(svuotaRiga,6);						
								}else{		
									puts(messaggio11,5);
									puts(messaggio12,6);		//Non si può andare giu				
								}
							} else{
								muoviPedina(&(gioco1.mia), gioco1.mia.i+1, gioco1.mia.j, gioco1.griglia);
								mossa_corrente[1]=gioco1.mia.i;
								mossa_corrente[2] = gioco1.mia.j;		
								puts(svuotaRiga,5);
								puts(svuotaRiga,6);		
							}
						}else{		
							puts(messaggio11,5);
							puts(messaggio12,6);		//Non si può andare giu
						}
					}else{ 
						if(m.i<7){
							muoviMuro(&m, m.i+1, m.j, gioco1.griglia);
							mossa_corrente[1] = m.i;
							mossa_corrente[2] = m.j;
						}
					}
#ifndef NO_LCD
						lcdPrintDx(gioco1.griglia);
#endif			
				}
				s7_state++;
			}
		} else{
			s7_state = 0;
		}
}

void s8_control(){
	//Controllo se Pulsante 8 premuto
		if(!(IO2PIN & 0x00400000)){
			if(s8_state == 0){
				//Prima premuta
				s8_state++;
			} else if(s8_state == 1){
				//Seconda premuta
				//AZIONE -> Sposto una medina di una posizione a destra
				//ATTENZIONE: una volta sviluppato il controllo del gioco è necessario controllare prima se mossa fattibile
				if(mio_turno){
					if(!Pedina_Muro){
						if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].destra!=NULL){
							if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].destra->occupata==&(gioco1.avv)){
								if(gioco1.scacchiera[gioco1.mia.i][gioco1.mia.j].destra->destra!= NULL){
									muoviPedina(&(gioco1.mia), gioco1.mia.i, gioco1.mia.j+2, gioco1.griglia);
									mossa_corrente[2]=gioco1.mia.j;
									mossa_corrente[1] = gioco1.mia.i;
									puts(svuotaRiga,5);
									puts(svuotaRiga,6);		
								}else{	
									puts(messaggio11,5);
									puts(messaggio12,6);		//Non si può andare destra
								}
							} else{
								muoviPedina(&(gioco1.mia), gioco1.mia.i, gioco1.mia.j+1, gioco1.griglia);
								mossa_corrente[2]=gioco1.mia.j;
								mossa_corrente[1] = gioco1.mia.i;		
								puts(svuotaRiga,5);
								puts(svuotaRiga,6);						
							}
						}else{		
							puts(messaggio11,5);
							puts(messaggio12,6);		//Non si può andare destra
						}
					}else{
						if(m.j<7){
							muoviMuro(&m, m.i, m.j+1, gioco1.griglia);
							mossa_corrente[1] = m.i;
							mossa_corrente[2] = m.j;
						}
					}
#ifndef NO_LCD
					lcdPrintDx(gioco1.griglia);
#endif			
				}
				s8_state++;
			}
		} else{
			s8_state = 0;
		}
}

void s9_control(){
	//Controllo se Pulsante 9 premuto
	if(!flagInt9){
		if(!(IO0PIN & 0x40000000)){
			if(s9_state == 1){
				s9_state++;
				//AZIONE -> Invio sull'uart mossa_corrente
				if(mio_turno){
					if(controlla_mossa(&gioco1,mossa_corrente,mio_turno)==0){
						inviaMossa(mossa_corrente);
						controlla_vittoria();
						mio_turno=0;
						if(Pedina_Muro){
							piazzaMuro(&m,gioco1.griglia);
							muriInfo(info);
							lcdPrintSxUP(info);
						}
						contoRovescia=0;
						timeout_mossa=TIME_MAX;
						restart_countdown();
					}else{
						puts(messaggio11,5);
						puts(messaggio12,6);
					}
				}
			}else if(s9_state == 2){
					s9_state++;
			}
			
		}else{
			s9_state = 0;
			PINSEL1 = 0x25454400;
			VICIntEnable = 0x00020050;
			flagInt9=1;
		}
	}
}

/*Funzione che inizializza la griglia di gioco alla partenza*/
void creaGriglia(U8 griglia[64][64]){

	//Variabili di ciclo
	int i, j, x, y;

	for (x = 0; x<9; x++){

			for (y = 0; y<9; y++){

				for (i = 0; i<7; i++){

					for (j = 0; j<7; j++){

						if (i == 0 || j == 0){
							griglia[i+x*7][j+y*7] = 255;
						} else {
							griglia[i+x*7][j+y*7] = 0;
						}
					} //j

				} //i

			} //y

	} //x

	//completo la griglia con i bordi di dx
	for(i = 0; i<64; i++){
		griglia[i][63] = 255;
		griglia[63][i] = 255;
	}

}


/*Funzione che inizializza le info*/
void infoInit(U8 info[64][64]){
	U8 i, j;

	for(i=0; i<64; i++){
		for(j=0; j<64; j++){
			info[i][j] = 0;
		}
	}

	//Disegno Muri Disponibili
	muriInfo(info);
}

/*Funzione che disegna i muri a disposizione*/
void muriInfo(U8 info[64][64]){
	U8 i, j, k;

	//Azzero la parte dei muri
	for(i=0; i<14; i++){
		for(j=0; j<41; j++){
			info[i][j] = 0;
		}
	}

	//Disegno i muri ancora disponibili
	for(k=0; k<gioco1.num_miei_muri; k++){
		for(i=0; i<14; i++){
			for(j=0; j<4; j++){
				if(j==0 || i==0){
					info[i][j+k*4] = 0;
				} else{
					info[i][j+k*4] = 255;
				}
			}
		}
	}
}
//Funzione che controlla la decisione del colore
void decisioneColore(void){
	U8 c;
	if(b_ric.faseIniziale && mio_colore==0){		//Non ho ancora inviato nulla
		c=prelevaBuffer(&b_ric);
		if(c=='b'){
			mio_colore='n';
			//inserisciBuffer(&b_inv,mio_colore);
			scriviUartChar('n');
		}
		else if(c=='n')
			mio_colore='b';
	}else if(b_ric.faseIniziale && mio_colore=='p'){	//Inviato 'b' ma non confermato
			c=prelevaBuffer(&b_ric);
			if(c=='b'){
				mio_colore='e';
			} else if(c<255){													//se mi manda qualsiasi cosa io sono bianco
				mio_colore='b';
			}
		}
	b_ric.pronto=0;
}

void avantiPedina(void){
	Casella *c;
	c=gioco1.mia.minPath[0];
	mossa_corrente[0]='p';
	mossa_corrente[1]=c->i;
	mossa_corrente[2]=c->j;
	mossa_corrente[3]=255;
	return;
}

void inviaMossa(U8 *mossa){
	int i;	
	if(mio_colore == 'n'){
		if(mossa[0]=='p'){
			mossa[1] = 8 - mossa[1];
			mossa[2] = 8 - mossa[2];
		}else{
			mossa[1] = 7 - mossa[1];
			mossa[2] = 7 - mossa[2];
		}
	}
	#ifdef Teraterm
	mossa[1]+='0';
	mossa[2]+='0';
	#endif
	for(i=0;i<5 && mossa[i]!=255;i++){
		inserisciBuffer(&b_inv,mossa[i]);
	}
	scriviUart(&b_inv);
	n_turni++;
	controlla_vittoria();
	mio_turno=0;
	
	#ifndef NO_LCD
	lcdPrintDx(gioco1.griglia);
	#endif
	#ifndef Teraterm
	mossa[1]+='0';
	mossa[2]+='0';
	#endif
	for(i=0;mossa[i]!=255 && i<5 ;i++){
		putcXY(mossa[i],i*6,5);
	}
	
}
