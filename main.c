#include <stdio.h>
#include <stdlib.h>
#define INF 0xFFFFFFFF
/********************** Struttura dati che salva il grafo ************************/
//struttura dati che salva un nodo del grafo
typedef struct Nodo{
    unsigned long int indice;           //indice nodo
    struct Arco *head;                  //lista di adicenza degli archi
}Nodo;
//struttura dati che salva un arco del grafo
typedef struct Arco{
    unsigned long int peso;         //peso arco
    struct Arco *next;              //puntatore all'arco successivo del nodo padre
    struct Nodo *nodo;             //puntatore al nodo a cui è collegato l'arco
}Arco;
/********************** Struttura dati che salva il MinHeap ************************/
typedef unsigned long int tipobaseValCP;
typedef unsigned long int tipobasePrCP;
//Struttura Nodo Coda di priorità
typedef struct{
    tipobaseValCP valore;                   //indice nodo inserito nella CP
    tipobasePrCP priorita;                  //priorita nodo (peso cammino minimo relativo)
    unsigned long int posizione;            //posizione all'interno della CP
}NodoCP;
//Struttura coda di priorità
typedef struct{
    unsigned long int capacita;             //dimensione massima CP
    unsigned long int dim;                  //numero di elementi aggiunti alla CP
    NodoCP* H;                              //Puntatore al MinHeap
}CP;
/********************** struttura dati per salvare un nodo dell'array topK ****************/
typedef struct{
    unsigned long int sommaCammini;
    unsigned long int indiceGrafo;
}NodoTopK;
/********************** Prototipi di funzione ************************/
unsigned long int AggiungiGrafo(unsigned long int d, Nodo* nodi);
void MakeCP(CP* p, unsigned long int n);
void scambio(NodoCP v[], unsigned long int i, unsigned long int j);
void InsertMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr);
tipobaseValCP DeleteMinCP(CP* p);
void DecreaseKeyMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr);
void minHeapRestore(NodoCP v[], unsigned long int i, unsigned long int n);
void Dijkstra(Nodo nodi[], unsigned long int sorg, unsigned long int d[], unsigned long int dim);
void ReleaseGraph(Nodo* nodi, unsigned long int dim);


int main() {
    unsigned long int D, K;
    unsigned long int i, indiceTop = 0, maxSomma = 0, indiceMax = 0;
    if(scanf("%lu %lu", &D, &K) == EOF){                               //Leggo dall'input d e k;
        return -1;
    }
    NodoTopK* topGrafi = (NodoTopK *) malloc(K*sizeof (NodoTopK));
    unsigned long int indiceGrafo = 0;
    char comando[13];
    Nodo* nodi = (Nodo*) calloc(D, sizeof (Nodo));

    while (scanf("%s", comando) != EOF){                //ciclo che legge tutte le istruzioni in input
        if(comando[0] == 'A') {                                //controllo se è la AggiungiGrafo
            unsigned long int somma = AggiungiGrafo(D, nodi);        //eseguo l'aggiunta del grafo e il calcolo dei cammini minimi
            if(indiceTop < K){
                topGrafi[indiceTop].sommaCammini = somma;
                topGrafi[indiceTop].indiceGrafo = indiceGrafo;
                indiceTop++;
            }else{
                maxSomma = 0;
                indiceMax = 0;
                for(i = 0; i < K; i++){
                    if(topGrafi[i].sommaCammini == maxSomma){
                        if(topGrafi[i].indiceGrafo > topGrafi[indiceMax].indiceGrafo){
                            maxSomma = topGrafi[i].sommaCammini;
                            indiceMax = i;
                        }
                    }else if(topGrafi[i].sommaCammini > maxSomma){
                        maxSomma = topGrafi[i].sommaCammini;
                        indiceMax = i;
                    }
                }
                if(somma < maxSomma){
                    topGrafi[indiceMax].indiceGrafo = indiceGrafo;
                    topGrafi[indiceMax].sommaCammini = somma;
                }
            }
            indiceGrafo++;
        }else if(comando[0] == 'T'){                                                 //altrimenti eseguo la TopK
            if(indiceTop < K){
                for(i = 0; i < indiceTop-1; i++) printf("%lu ", topGrafi[i].indiceGrafo);
                printf("%lu\n", topGrafi[i].indiceGrafo);
            }else{
                for(i = 0; i < K-1; i++) printf("%lu ", topGrafi[i].indiceGrafo);
                printf("%lu\n", topGrafi[i].indiceGrafo);
            }
        }
    }
    ReleaseGraph(nodi,D);
    return 0;
}
/********************** Funzioni che Gestiscono il Grafo ************************/
//funzione che aggiunge il Grafo
unsigned long int AggiungiGrafo(unsigned long int d, Nodo* nodi){
    Nodo* nodo;                                                         //creo un nodo temporaneo
    Arco* arco;                                                         //creo un arco temporaneo
    unsigned long int i,j;
    for(i = 0; i < d; i++){                                             //ciclo che scorre tutte le righe della matrice di adiacenza
        nodo = &(nodi[i]);                                              //il nodo di riferimento diventa quello con indice della riga appena letta
        nodo->indice = i;                                               //salvo l'indice del nodo
        if(nodo->head == NULL) nodo->head = (Arco*) malloc(sizeof (Arco));                     //creo la head del nodo
        arco = nodo->head;                                              //salvo la head come primo arco
        for(j = 0; j < d; j++){                                         //ciclo tutte le colonne della matrice di adiacenza
            arco->nodo = &(nodi[j]);                                    //collego l'arco da aggiungere al relativo nodo
            if(j == d-1) {
                if(scanf("%lu", &arco->peso) == EOF){            //leggo il peso dell'arco e lo salvo nel relativo arco
                    return INF;
                };
                arco->next = NULL;                                      //se è l'ultimo arco lo faccio puntare a NULL
            }else{
                if(scanf("%lu,", &arco->peso) == EOF){                  //leggo il peso dell'arco e lo salvo nel relativo arco
                    return INF;
                };
                if(arco->next == NULL) arco->next = malloc(sizeof (Arco));                      //se non è l'ultimo arco creo il successivo e lo collego all'arco corrente
                arco = arco->next;                                      //l'arco diventa il nuovo arco
            }
        }
    }
    unsigned long int *pesi = (unsigned long int*)malloc(d*sizeof (unsigned long int));             //creo l'array dei pesi dei cammini minimi che Dijkstra mi restituisce
    Dijkstra(nodi, 0,pesi,d);                                                                       //eseguo l'algoritmo di Dijkstra
    unsigned long int somma = 0;
    for(i = 1; i < d; i++){                                                                             //ciclo tutto l'array dei pesi
        if(pesi[i] != INF) {                                                                            //se il peso non è infinito allora lo aggiungo alla somma
            somma += pesi[i];
        }
    }
    free(pesi);                                                                                        //libero la memoria occupata dall'array dei pesi
    return somma;
}
//funzione che libera tutta la memoria del grafo
void ReleaseGraph(Nodo* nodi, unsigned long int dim){
    Nodo* nodo;
    Arco* tmp;
    Arco* arco;
    unsigned long int i;
    for(i = 0; i < dim;i++){
        nodo = &nodi[i];
        arco = nodo->head;
        while(arco != NULL){
            tmp = arco->next;
            free(arco);
            arco = tmp;
        }
    }
    free(nodi);
}
/********************** Funzioni che Gestiscono il MinHeap ************************/
//Funzione che crea la CP
void MakeCP(CP* p, unsigned long int n){
    p->capacita = n;                                            //imposta la capacità massima della CP a n
    p->dim = 0;                                                 //imposta il numero di elementi inseriti a 0
    p->H = (NodoCP*)malloc(n*sizeof (NodoCP));             //crea l'array che rappresenta la CP contenente n NodiCP
}
//funzione che effettua il cambio di posizione tra 2 elementi della CP
void scambio(NodoCP v[], unsigned long int i, unsigned long int j){
    NodoCP tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
    v[i].posizione = i;
    v[j].posizione = j;
}
//Funzione che inserisce un nuovo elemento nella CO
void InsertMinCP(CP* p, tipobaseValCP x, tipobasePrCP pr){
    NodoCP tmp;
    unsigned long int i;

    if(p->dim != p->capacita){          //Controllo se la CP non è piena
        i = p->dim;                     //salvo il numero di elementi attuale della CP
        p->dim = p->dim+1;              //incremento il numero di elementi presenti

        tmp.valore = x;                 //salvo l'indice nel nuovo elemento della CP
        tmp.priorita = pr;              //salvo la priorità nel nuovo elemento della CP
        tmp.posizione = i;              //salvo la posizione nel nuovo elemento della CP

        p->H[i] = tmp;                  //salvo il nodo nella Cp

        while(i > 0 && p->H[i].priorita < p->H[(i-1)/2].priorita){          //ciclo che controlla se la priorità del nodo è minore di quella del padre e controlla se sono arrivato alla root
            scambio(p->H, i, (i-1)/2);                                   //Eventualmente scambia il nodo con il padre
            i = (i-1)/2;                                                    //salva il nuovo indice del nodo
        }
    }
}
//funzione che sistema la CP rimettendo il minimo in prima posizione
void minHeapRestore(NodoCP v[], unsigned long int i, unsigned long int n){
    unsigned long int min = i;
    if (2*i+1 < n && v[2*i+1].priorita < v[min].priorita) min=2*i+1;        //controllo se il figlio sinistro può esistere nella CP e se la priorità del figlio sinistro è minore di quella del padre. Se lo è il nuovo minimo diventa il figlio sinistro
    if (2*i+2 < n && v[2*i+2].priorita < v[min].priorita) min=2*i+2;        //come il controllo precedente solo per il figlio destro
    if(i!=min){                                                             //se il minimo è cambiato effettuo uno scambio tra padre e figlio e continuo a sistemare la CP
        scambio(v,i,min);
        minHeapRestore(v,min,n);
    }
}
//Funzione che elimina l'elemento minimo dalla CP
tipobaseValCP DeleteMinCP(CP* p){
    //if(p->dim != 0){                        //controllo che la CP non sia vuota
        p->dim = p->dim-1;                  //decremento il numero di elementi all'interno della CP
        scambio(p->H, 0, p->dim);
        minHeapRestore(p->H, 0, p->dim);    //sistemo la CP
        return p->H[p->dim].valore;         //restituisco l'indice del nodo
    //}
    //return INF;
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
/********************** Algoritmo di Dijkstra ************************/
void Dijkstra(Nodo* nodi, unsigned long int sorg,unsigned long int d[], unsigned long int dim) {
    unsigned long int u;
    CP coda;
    Arco *arco;

    for (u = 0; u < dim; u++) d[u] = INF;
    d[sorg] = 0;
    MakeCP(&coda, dim);
    InsertMinCP(&coda, sorg, d[sorg]);
    while (coda.dim != 0) {
        u = DeleteMinCP(&coda);
        //if (u != INF) {
            arco = nodi[u].head;
            if (arco != NULL) {
                if (d[arco->nodo->indice] == INF && arco->peso != 0) {
                    InsertMinCP(&coda, arco->nodo->indice, d[u] + arco->peso);
                    d[arco->nodo->indice] = d[u] + arco->peso;
                } else if (d[u] + arco->peso < d[arco->nodo->indice] && arco->peso != 0) {
                    DecreaseKeyMinCP(&coda, arco->nodo->indice, d[u] + arco->peso);
                    d[arco->nodo->indice] = d[u] + arco->peso;
                }
                while (arco->next != NULL) {
                    if (d[arco->next->nodo->indice] == INF && arco->next->peso != 0) {
                        InsertMinCP(&coda, arco->next->nodo->indice, d[u] + arco->next->peso);
                        d[arco->next->nodo->indice] = d[u] + arco->next->peso;
                    } else if (d[u] + arco->next->peso < d[arco->next->nodo->indice] && arco->next->peso != 0) {
                        DecreaseKeyMinCP(&coda, arco->next->nodo->indice, d[u] + arco->next->peso);
                        d[arco->next->nodo->indice] = d[u] + arco->next->peso;
                    }
                    arco = arco->next;
                }
            }
        //}
    }
    free(coda.H);
}



