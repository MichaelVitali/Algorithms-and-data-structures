#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 0xFFFFFFFF

// strruttra che rappresenta il nodo della coda di priorità
struct NodoCP{
    unsigned long int riga;
    struct NodoCP *next;
} NodoCP;
//struttura utilizzata per la TopK
typedef struct{
    unsigned long int sommaCammini;
    unsigned long int indiceGrafo;
} NodoTopK;

//prototipi di funzione
unsigned long int AggiungiGrafo(unsigned long int **matrice, unsigned long int dim, unsigned long int *pesi);
void Dijkstra(unsigned long int **matrice, unsigned long int *pesi, unsigned long int dim);
unsigned long int estraiMin(struct NodoCP **primo, unsigned long int d[]);
void accoda(struct NodoCP **primo, unsigned long int riga);
void buildMaxHeap(NodoTopK *topGrafi, unsigned long int size);
void maxHeapify(NodoTopK heap[], unsigned long int i, unsigned long int size);

int main(){
    //Inizializzo le variabili necessarie
    unsigned long int D, K, i, indiceTop = 0, indiceGrafo = 0;
    char comando[15];

    //leggo D e K da input
    if (scanf("%lu %lu", &D, &K) == EOF){
        return -1;
    }

    //Creo la matrice di adiacenza dove salvo i dati letti in ingresso
    unsigned long int **matrice = (unsigned long int **)malloc(D * sizeof(unsigned long int *));
    for (i = 0; i < D; i++)
        matrice[i] = (unsigned long int *)malloc(D * sizeof(unsigned long int));
    unsigned long int *pesi = (unsigned long int *)malloc(D * sizeof(unsigned long int));                           //inizilizzo l'array utilizzato per la i pesi dei cammini minimi in Dijkstra
    NodoTopK *topGrafi = (NodoTopK *)malloc(K * sizeof(NodoTopK));                                                  //inizializzo l'array utilizzato per la TopK

    //ciclo fino quando non incontro la fine dell'input e leggo da input il comando
    while (fgets(comando, 15, stdin) != NULL){
        if (comando[0] == 'A')                                                                                          //controllo se il comando in ingresso è l'AggiungiGrafo
        {
            unsigned long int somma = AggiungiGrafo(matrice, D, pesi);                                                  //recupero la somma dei cammini minimi restituita da Dikstra
            if (indiceTop < K)                                                                                          //se l'indiceTop è minore della grandezza della TopK lo inserisco nella prima cella liber
            {
                //inserimento del nuovo grafo alla TopK
                topGrafi[indiceTop].indiceGrafo = indiceGrafo;
                topGrafi[indiceTop].sommaCammini = somma;
                indiceTop++;
            }else{
                //altrimenti se è il primo valore che supera la dimensione creo il maxHeap della topK
                if (indiceTop == K){
                    buildMaxHeap(topGrafi, K);
                    indiceTop++;
                }
                //se è la somma del nuovo elemento è minore di quella minima all'interno dell'array lo aggiungo e sistemo il maxHeap
                if (topGrafi[0].sommaCammini > somma){
                    //aggiungi il nodo alla TopK in testa
                    topGrafi[0].sommaCammini = somma;
                    topGrafi[0].indiceGrafo = indiceGrafo;
                    maxHeapify(topGrafi, 0, K);                     //sistemo il maxHeap
                }
            }
            indiceGrafo++;                                              //incremento l'indice del grafo
        }else if (comando[0] == 'T'){                                   //se il comando è la TopK stampo gli indici dei grafi
            if (indiceTop == 0)                                         //se non ci sono grafi non stampo nulla
                printf("\n");
            else if (indiceTop < K)                                     //se l'array non è pieno stampo solo le prime n posizioni inserite
            {
                for (i = 0; i < indiceTop - 1; i++)
                    printf("%lu ", topGrafi[i].indiceGrafo);
                printf("%lu\n", topGrafi[i].indiceGrafo);
            }else{                                                      //altrimenti lo stampo tutto
                for (i = 0; i < K - 1; i++)
                    printf("%lu ", topGrafi[i].indiceGrafo);
                printf("%lu\n", topGrafi[i].indiceGrafo);
            }
        }
    }
    return 0;
}

//funzione che legge il grafo da input e esegue Dijkstra. Restituisce la somma dei cammini minimi
unsigned long int AggiungiGrafo(unsigned long int **matrice, unsigned long int dim, unsigned long int *pesi)
{
    //inizializzo le variabili
    char *token = NULL;
    char *stringa = NULL;
    size_t len = 0;
    size_t line_size = 0;
    unsigned long int i, j, s = 0, peso, somma = 0;
    //ciclo tutte le righe della matrice
    for (i = 0; i < dim; i++){
        line_size = getline(&stringa, &len, stdin);                                                 //leggo una riga da input
        if (line_size == 0)                                                                         //se non ho letto niente restiuisco INF coe errore
            return INF;

        token = stringa;
        for (j = 0; j < dim; j++){                                                                   //altrimenti separo la riga letta in input
            peso = 0;
            //ciclo che splitta la stringa letta e converte i numeri letti in decimali
            while (*token >= '0' && *token <= '9')
            {
                peso = peso * 10 + (*token++ - '0');
            }
            //ciclo che toglie la , dall'input letto
            while (*token && *token++ != ',')
            {
            }
            //assegna il peso alla posizione i,j della matrice
            matrice[i][j] = peso;
            //if che serve a gestire il caso in cui tutta la prima riga è 0. se sto leggendo la prima riga aggiungo il peso alla somma
            if (i == 0)
                s += peso;
        }
    }
    free(stringa);                                                                  //libero la memoria della stringa letta
    if (s == 0)                                                                     //se la prima riga è tutta 0 restiuisco subito 0 come somma dei cammini
        return 0;
    Dijkstra(matrice, pesi, dim);                                                   //altrimenti eseguo l'algoritmo di dijkstra

    //ciclo che calcola la somma dei cammini minimi restituiti da Dijkstra
    for (i = 0; i < dim; i++){
        if (pesi[i] != INF)
            somma += pesi[i];
    }
    return somma;                                                                    //restituisco la somma dei cammini al main
}

//funzione che crea la coda di priorità gestitta mediante Lista di strutture
void accoda(struct NodoCP **primo, unsigned long int riga){
    struct NodoCP *p;
    p = malloc(sizeof(NodoCP));
    p->riga = riga;
    p->next = *primo;
    *primo = p;
}

//funzione che estrae la minima distanza all'interno della coda di priorità
unsigned long int estraiMin(struct NodoCP **primo, unsigned long int d[]){
    //inizializzo tutte le variabili utilizzate
    struct NodoCP *p, *pmin, *prec = NULL, *precmin = NULL;
    unsigned long int u;
    p = *primo;
    pmin = *primo;

    //ciclo tutta la lista fino quando non arrivo all'ultimo elemento
    while (p != NULL){
        //se il valore del nodo attuale è minore di quello del minimo sostituisco il minimo con il nodo attuale
        if (d[p->riga] < d[pmin->riga]){
            pmin = p;
            precmin = prec;
        }
        //avanzo lungo la lista
        prec = p;
        p = p->next;
    }
    //salvo l'indice di riga del minimo
    u = pmin->riga;

    //sistemo la lista
    if (precmin == NULL){
        *primo = (*primo)->next;
    }else{
        precmin->next = pmin->next;
    }
    free(pmin);
    return u;                   //restituisco l'indice di riga del nodo con distanza minima
}

//algoritmo di dijkstra
void Dijkstra(unsigned long int **matrice, unsigned long int *pesi, unsigned long int dim)
{
    unsigned long int u, v;
    struct NodoCP *primo = NULL;

    //inizializzo tutte le sistanze ad INF
    for (u = 1; u < dim; u++){
        pesi[u] = INF;
    }
    //metto la distanza della sorgente a 0
    pesi[0] = 0;

    //aggiungi tutti i nodi del grafo alla lista (coda di priorità)
    for (u = 0; u < dim; u++){
        accoda(&primo, u);
    }

    //ciclo fino quando la coda di priorità non è vuota
    while (primo != NULL){
        //estraggo il minimo della CP
        u = estraiMin(&primo, pesi);

        //ciclo tutti i nodi della riga selezionata
        for (v = 0; v < dim; v++){
            //se il peso del vertice attuale è minore di quello precedente + il peso attuale lo sostituisco
            if (pesi[v] > pesi[u] + matrice[u][v] && matrice[u][v] != 0){
                pesi[v] = pesi[u] + matrice[u][v];
            }
        }
    }
    //libero la memoria della lista
    free(primo);
}

//funzione che crea il maxHeap
void buildMaxHeap(NodoTopK *topGrafi, unsigned long int size){
    unsigned long int i;

    //ciclo che parte dalla root del MaxHeap e lo sistema mettendo il massimo in testa
    for (i = (size / 2) - 1; i >= 0; i--){
        maxHeapify(topGrafi, i, size);          //funzione che sistema il maxHeap
        if (i == 0)                             //controllo che gestisce quando la i diventa negativa e termina il ciclo
            break;
    }
}

//funzione che sistema il maxHeap mettendo il massimo in testa
void maxHeapify(NodoTopK heap[], unsigned long int i, unsigned long int size){
    unsigned long int l = 0, r = 0, largest = 0;
    do{
        //inizializzo gli indici del figlio sinistro, destro e i massimo.
        l = (i * 2) + 1, r = (i * 2) + 2, largest = i;
        //controlli per selezionare il massimo tra padre, figlio sx e dx
        if (l < size && heap[l].sommaCammini > heap[largest].sommaCammini)
            largest = l;
        else if (l < size && heap[l].sommaCammini == heap[largest].sommaCammini &&
                 heap[l].indiceGrafo > heap[largest].indiceGrafo)
            largest = l;
        if (r < size && heap[r].sommaCammini > heap[largest].sommaCammini)
            largest = r;
        else if (r < size && heap[r].sommaCammini == heap[largest].sommaCammini &&
                 heap[r].indiceGrafo > heap[largest].indiceGrafo)
            largest = r;

        //se il il max è in testa termino, altrimenti scambio il padre con il figlio selezionato dai controlli
        if (largest != i)
        {
            NodoTopK tmp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = tmp;
            i = largest;
        }
        else{
            break;
        }
    } while (1);                                //utilizzo di u ciclo infinito con i dovuti controlli per evitare le numerose chiamate di sistema per la ricorsione ed ottimizzare i tempi di esecuzione
}
