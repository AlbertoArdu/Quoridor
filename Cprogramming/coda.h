
#define MAX_CODA 82
#define NULL (void *)0
typedef struct coda Coda;

struct coda{
	volatile unsigned char testa, coda;
	void* elenco[MAX_CODA];
};

void initCoda(Coda *c);
void* getItem(Coda *c);
void putItem(Coda *c, void* item);
int isEmpty(Coda *c);
int isFull(Coda *c);
void* popItem(Coda *c);
