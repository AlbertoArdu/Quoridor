#include "coda.h"

void initCoda(Coda *c){
	c->testa=0;
	c->coda=0;		
}

void* getItem(Coda *c){
	c->testa++;
	c->testa%=MAX_CODA;
	return c->elenco[c->testa];
}

void putItem(Coda *c, void* item){
	c->coda++;
	c->coda%=MAX_CODA;
	c->elenco[c->coda]=item;
}

int isEmpty(Coda *c){
	return c->testa==c->coda;
}

int isFull(Coda *c){
	return c->testa==((c->coda+1)%MAX_CODA);
}

void* popItem(Coda *c){			//simulo il comportamento di uno stack
	void *item;
	item=c->elenco[c->coda];
	c->coda--;
	if(c->coda==255)
		c->coda=MAX_CODA;
	return item;
}
