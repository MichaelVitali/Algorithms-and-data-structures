#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 0xFFFFFFFF

struct NodoCP
{
    unsigned long int riga;
    unsigned long int priorita;
    struct NodoCP *next;
} NodoCP;
typedef struct
{
    unsigned long int sommaCammini;
    unsigned long int indiceGrafo;
} NodoTopK;

unsigned long int AggiungiGrafo(unsigned long int **matrice, unsigned long int dim, unsigned long int *pesi);
void Dijkstra(unsigned long int **matrice, unsigned long int *pesi, unsigned long int dim);
unsigned long int estraiMin(struct NodoCP **primo, unsigned long int d[]);
void accoda(struct NodoCP **primo, unsigned long int riga);
void buildMaxHeap(NodoTopK *topGrafi, unsigned long int size);
void maxHeapify(NodoTopK heap[], unsigned long int i, unsigned long int size);

int main()
{
    unsigned long int D, K, i, indiceTop = 0, indiceGrafo = 0;
    char comando[15];

    if (scanf("%lu %lu", &D, &K) == EOF)
    { //Leggo dall'input d e k;
        return -1;
    }

    unsigned long int **matrice = (unsigned long int **)malloc(D * sizeof(unsigned long int *));
    for (i = 0; i < D; i++)
        matrice[i] = (unsigned long int *)malloc(D * sizeof(unsigned long int));
    unsigned long int *pesi = (unsigned long int *)malloc(D * sizeof(unsigned long int));
    NodoTopK *topGrafi = (NodoTopK *)malloc(K * sizeof(NodoTopK));

    while (fgets(comando, 15, stdin) != NULL)
    {
        if (comando[0] == 'A')
        {
            unsigned long int somma = AggiungiGrafo(matrice, D, pesi);
            if (indiceTop < K)
            {
                topGrafi[indiceTop].indiceGrafo = indiceGrafo;
                topGrafi[indiceTop].sommaCammini = somma;
                indiceTop++;
            }
            else
            {
                if (indiceTop == K)
                {
                    buildMaxHeap(topGrafi, K);
                    indiceTop++;
                }
                if (topGrafi[0].sommaCammini > somma)
                {
                    topGrafi[0].sommaCammini = somma;
                    topGrafi[0].indiceGrafo = indiceGrafo;
                    maxHeapify(topGrafi, 0, K);
                }
            }
            indiceGrafo++;
        }
        else if (comando[0] == 'T')
        {
            if (indiceTop == 0)
                printf("\n");
            else if (indiceTop < K)
            {
                for (i = 0; i < indiceTop - 1; i++)
                    printf("%lu ", topGrafi[i].indiceGrafo);
                printf("%lu\n", topGrafi[i].indiceGrafo);
            }
            else
            {
                for (i = 0; i < K - 1; i++)
                    printf("%lu ", topGrafi[i].indiceGrafo);
                printf("%lu\n", topGrafi[i].indiceGrafo);
            }
        }
    }
    return 0;
}

unsigned long int AggiungiGrafo(unsigned long int **matrice, unsigned long int dim, unsigned long int *pesi)
{
    char *token = NULL;
    char *stringa = NULL;
    size_t len = 0;
    size_t line_size = 0;
    unsigned long int i, j, s = 0, peso, somma = 0;
    for (i = 0; i < dim; i++)
    {
        line_size = getline(&stringa, &len, stdin);
        if (line_size == 0)
            return INF;

        token = stringa;
        for (j = 0; j < dim; j++)
        {
            peso = 0;
            while (*token >= '0' && *token <= '9')
            {
                peso = peso * 10 + (*token++ - '0');
            }
            while (*token && *token++ != ',')
            {
            }

            matrice[i][j] = peso;
            if (i == 0)
                s += peso;
        }
    }
    free(stringa);
    if (s == 0)
        return 0;
    Dijkstra(matrice, pesi, dim);
    for (i = 0; i < dim; i++)
    {
        if (pesi[i] != INF)
            somma += pesi[i];
    }
    return somma;
}

void accoda(struct NodoCP **primo, unsigned long int riga)
{
    struct NodoCP *p;
    p = malloc(sizeof(NodoCP));
    p->riga = riga;
    p->next = *primo;
    *primo = p;
}

unsigned long int estraiMin(struct NodoCP **primo, unsigned long int d[])
{
    struct NodoCP *p, *pmin, *prec = NULL, *precmin = NULL;
    unsigned long int u;
    p = *primo;
    pmin = *primo;
    while (p != NULL)
    {
        if (d[p->riga] < d[pmin->riga])
        {
            pmin = p;
            precmin = prec;
        }
        prec = p;
        p = p->next;
    }
    u = pmin->riga;

    if (precmin == NULL)
    {
        *primo = (*primo)->next;
    }
    else
    {
        precmin->next = pmin->next;
    }
    free(pmin);
    return u;
}

void Dijkstra(unsigned long int **matrice, unsigned long int *pesi, unsigned long int dim)
{
    unsigned long int u, v;
    struct NodoCP *primo = NULL;

    for (u = 1; u < dim; u++)
    {
        pesi[u] = INF;
    }
    pesi[0] = 0;
    for (u = 0; u < dim; u++)
    {
        accoda(&primo, u);
    }

    while (primo != NULL)
    {
        u = estraiMin(&primo, pesi);
        for (v = 0; v < dim; v++)
        {
            if (pesi[v] > pesi[u] + matrice[u][v] && matrice[u][v] != 0)
            {
                pesi[v] = pesi[u] + matrice[u][v];
            }
        }
    }
    free(primo);
}

void buildMaxHeap(NodoTopK *topGrafi, unsigned long int size)
{
    unsigned long int i;
    for (i = (size / 2) - 1; i >= 0; i--)
    {
        maxHeapify(topGrafi, i, size);
        if (i == 0)
            break;
    }
}
void maxHeapify(NodoTopK heap[], unsigned long int i, unsigned long int size)
{
    unsigned long int l = 0, r = 0, largest = 0;
    do
    {
        l = (i * 2) + 1, r = (i * 2) + 2, largest = i;
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
        if (largest != i)
        {
            NodoTopK tmp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = tmp;
            i = largest;
        }
        else
        {
            break;
        }
    } while (1);
}
