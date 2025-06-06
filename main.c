#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <glib.h>
#define INPUT_SIZE 100000000
typedef struct No
{
    int dado;
    struct No *prox;
} No;

typedef struct
{
    No *topo;
} PilhaEnc;

PilhaEnc *criarPilhaEnc()
{
    PilhaEnc *p = malloc(sizeof(PilhaEnc));
    p->topo = NULL;
    return p;
}

static inline void pushEnc(PilhaEnc *restrict p, int valor)
{
    No *novo = malloc(sizeof(No));
    novo->dado = valor;
    novo->prox = p->topo;
    p->topo = novo;
}

static inline int popEnc(PilhaEnc *restrict p)
{
    No *temp = p->topo;
    int valor = temp->dado;
    p->topo = temp->prox;
    free(temp);
    return valor;
}

typedef struct
{
    int *dados;
    int topo;
    int capacidade;
} PilhaArr;

PilhaArr *criarPilhaArr(int cap)
{
    PilhaArr *p = malloc(sizeof(PilhaArr));
    p->dados = malloc(cap * sizeof(int));
    p->topo = -1;
    p->capacidade = cap;
    return p;
}

static inline void redimensionar(PilhaArr *restrict p, int novo_cap)
{
    p->dados = realloc(p->dados, novo_cap * sizeof(int));
    p->capacidade = novo_cap;
}

static inline void pushArr(PilhaArr *restrict p, int valor)
{
    if (++p->topo == p->capacidade)
        redimensionar(p, p->capacidade * 2);
    p->dados[p->topo] = valor;
}

static inline int popArr(PilhaArr *restrict p)
{
    return p->dados[p->topo--];
}


__attribute__((hot)) void testEnc(PilhaEnc *restrict p)
{
    for (int i = 0; i < INPUT_SIZE; i++)
        pushEnc(p, i);
    for (int i = 0; i < INPUT_SIZE; i++)
        popEnc(p);
}

__attribute__((hot)) void testArr(PilhaArr *restrict p)
{
    for (int i = 0; i < INPUT_SIZE; i++)
        pushArr(p, i);
    for (int i = 0; i < INPUT_SIZE; i++)
        popArr(p);
}

__attribute__((hot)) void testGlib(GQueue *restrict g)
{
    for (int i = 0; i < INPUT_SIZE; i++)
        g_queue_push_head(g, GINT_TO_POINTER(i));
    for (int i = 0; i < INPUT_SIZE; i++)
        g_queue_pop_head(g);
}

void main()
{
    clock_t t1, t2, t3, t4, t5, t6;
    PilhaEnc *pEnc = criarPilhaEnc();
    PilhaArr *pArr = criarPilhaArr(16);
    GQueue *g = g_queue_new();

    t1 = clock();
    testEnc(pEnc);
    t2 = clock();

    t3 = clock();
    testArr(pArr);
    t4 = clock();

    t5 = clock();
    testGlib(g);
    t6 = clock();

    printf("Items pushed: 100 million\nItems popped: 100 million\n");
    printf("linked list: %.3f s\n", (double)(t2 - t1) / CLOCKS_PER_SEC);
    printf("      array: %.3f s\n", (double)(t4 - t3) / CLOCKS_PER_SEC);
    printf("      glib:  %.3f s\n", (double)(t6 - t5) / CLOCKS_PER_SEC);

    free(pArr->dados);
    free(pArr);
    while (pEnc->topo)
        popEnc(pEnc);
    free(pEnc);
    g_queue_free(g);
}
