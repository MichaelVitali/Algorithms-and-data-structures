#include <stdio.h>
#include <stdlib.h>
#define INF 0xFFFFFFFF

typedef struct Nodo{
    unsigned long int indice;
    struct Arco *head;
}Nodo;
typedef struct Arco{
    unsigned long int peso;
    struct Arco *next;              //puntatore all'arco successivo del nodo padre
    struct Nodo *nodo;             //puntatore al nodo a cui è collegato l'arco
}Arco;
//Strutture per il MinHeap
typedef unsigned long int tipobaseValCP;
typedef unsigned long int tipobasePrCP;
//Struttura Nodo Coda di priorità
typedef struct{
    tipobaseValCP valore;
    tipobasePrCP priorita;
    unsigned long int posizione;
}NodoCP;
//Struttura coda di priorità
typedef struct{
    unsigned long int capacita;
    unsigned long int dim;
    NodoCP* H;
}CP;

void AggiungiGrafo(unsigned long int d);
void stampaGrafo(unsigned long int d, Nodo* nodoPtr);
void MakeCP(CP* p, unsigned long int n);
void scambio(NodoCP v[], unsigned long int i, unsigned long int j);
void InsertMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr);
tipobaseValCP DeleteMinCP(CP* p);
void DecreaseKeyMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr);
void minHeapRestore(NodoCP v[], unsigned long int i, unsigned long int n);
void Dijkstra(Nodo nodi[], unsigned long int sorg, unsigned long int d[], unsigned long int dim);

int main() {
    unsigned long int D, K;
    scanf("%lu %lu", &D, &K);    //Leggo dall'input d e k;
    char comando[1];
    while (scanf("%s", comando) != EOF){
        if(comando[0] == 'A') {
            AggiungiGrafo(D);
        }else{
            //TopK
            printf("Sono nella Top\n");
        }
    }
}

void AggiungiGrafo(unsigned long int d){
    Nodo* nodi = (Nodo*) calloc(d, sizeof (Nodo));
    Nodo* nodo;
    Arco* arco;
    unsigned long int i,j;
    for(i = 0; i < d; i++){
        nodo = &(nodi[i]);
        nodo->indice = i;
        nodo->head = (Arco*) malloc(sizeof (Arco));
        arco = nodo->head;
        for(j = 0; j < d; j++){
            arco->nodo = &(nodi[j]);
            if(j == d-1) {
                scanf("%lu", &arco->peso);
                arco->next = NULL;
            }else{
                scanf("%lu,", &arco->peso);
                arco->next = malloc(sizeof (Arco));
                arco = arco->next;
            }
        }
    }
    //stampaGrafo(d,nodi);
    unsigned long int *pesi = (unsigned long int*)malloc(d*sizeof (unsigned long int));
    Dijkstra(nodi, 0,pesi,d);
    unsigned long int somma = 0;
    for(i = 1; i < d; i++){
        if(pesi[i] != INF) {
            somma += pesi[i];
            printf("Peso distanza nodo %lu: %lu\n", i, pesi[i]);
        }
    }
    free(pesi);
    printf("La somma dei cammini minimi è: %lu\n", somma);
}

void stampaGrafo(unsigned long int d, Nodo* nodoPtr){
    int i;
    for(i = 0; i < d; i++){
        Nodo nodo = nodoPtr[i];
        printf("Nodo %lu:\n", nodo.indice);
        Arco* arco = nodo.head;
        while(arco != NULL){
            printf("\tPeso: %lu\tNodo:%lu\n", arco->peso, arco->nodo->indice);
            arco = arco->next;
        }
    }
}
void MakeCP(CP* p, unsigned long int n){
    p->capacita = n;
    p->dim = 0;
    p->H = (NodoCP*)malloc(n*sizeof (NodoCP));
}
void scambio(NodoCP v[], unsigned long int i, unsigned long int j){
    NodoCP tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
    v[i].posizione = i;
    v[j].posizione = j;
}
void InsertMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr){
    NodoCP tmp;
    unsigned long int i;

    if(p->dim != p->capacita){          //Controllo se la CP non è piena
        i = p->dim;
        p->dim = p->dim+1;

        tmp.valore = x;
        tmp.priorita = pr;
        tmp.posizione = i;

        p->H[i] = tmp;

        while(i > 0 && p->H[i].priorita < p->H[(i-1)/2].priorita){
            scambio(p->H, i, (i-1)/2);
            i = (i-1)/2;
        }
    }
}
void minHeapRestore(NodoCP v[], unsigned long int i, unsigned long int n){
    unsigned long int min = i;
    if (2*i+1 < n && v[2*i+1].priorita < v[min].priorita) min=2*i+1;
    if (2*i+2 < n && v[2*i+2].priorita < v[min].priorita) min=2*i+2;
    if(i!=min){
        scambio(v,i,min);
        minHeapRestore(v,min,n);
    }
}
tipobaseValCP DeleteMinCP(CP* p){
    if(p->dim != 0){                        //controllo che la CP non sia vuota
        p->dim = p->dim-1;
        scambio(p->H, 0, p->dim);
        minHeapRestore(p->H, 0, p->dim);
        return p->H[p->dim].valore;
    }
}
void DecreaseKeyMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr){
    unsigned long int i;

    if(p->dim != 0){
        for(i = 0; i < p->dim; i++){
            if(p->H[i].valore == x){
                if(p->H[i].priorita > pr){
                    p->H[i].priorita = pr;
                    while(i > 0 && p->H[i].priorita < p->H[(i-1)/2].priorita){
                        scambio(p->H,i,(i-1)/2);
                        i = (i-1)/2;
                    }
                }
                break;
            }
        }
    }
}

void Dijkstra(Nodo* nodi, unsigned long int sorg,unsigned long int d[], unsigned long int dim){
    unsigned long int u;
    CP coda;
    Arco* arco;

    for(u = 0; u < dim; u++) d[u] = INF;
    d[sorg] = 0;
    MakeCP(&coda, dim);
    InsertMinCP(&coda, sorg, d[sorg]);
    while(coda.dim != 0){
        u = DeleteMinCP(&coda);
        arco = nodi[u].head;
        if(arco != NULL){
            if(d[arco->nodo->indice] == INF && arco->peso != 0 ){
                InsertMinCP(&coda, arco->nodo->indice, d[u]+arco->peso);
                d[arco->nodo->indice] = d[u]+arco->peso;
            }else if(d[u]+arco->peso < d[arco->nodo->indice] && arco->peso != 0){
                DecreaseKeyMinCP(&coda, arco->nodo->indice, d[u]+arco->peso);
                d[arco->nodo->indice] = d[u]+arco->peso;
            }
            while(arco->next != NULL){
                if(d[arco->next->nodo->indice] == INF && arco->next->peso != 0){
                    InsertMinCP(&coda, arco->next->nodo->indice, d[u]+arco->next->peso);
                    d[arco->next->nodo->indice] = d[u]+arco->next->peso;
                }else if(d[u]+arco->next->peso < d[arco->next->nodo->indice] && arco->next->peso != 0){
                    DecreaseKeyMinCP(&coda, arco->next->nodo->indice, d[u]+arco->next->peso);
                    d[arco->next->nodo->indice] = d[u] + arco->next->peso;
                }
                arco = arco->next;
            }
        }
    }
    free(coda.H);
}
