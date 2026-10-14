#include "arv_bk.h"

#include <stdlib.h>

typedef struct no
{
    char *palavra;
    struct no *filho;
    struct no *irmao;
    int peso;
} No;

struct arv_bk
{
    No *raiz;
    int tamanho;
};

static void arv_bk_destroi_nos(No *n) {
    if (n == NULL) return;

    No *f = n->filho;
    No *i = n->irmao;

    free(n->palavra);
    free(n);

    arv_bk_destroi_nos(f);
    arv_bk_destroi_nos(i);
}

Arv_bk arv_bk_cria(void)
{
    Arv_bk a = malloc(sizeof(struct arv_bk));
    if (a == NULL) return NULL;

    a->raiz = NULL;
    a->tamanho = 0;

    return a;
}

void arv_bk_destroi(Arv_bk a)
{
    if (a == NULL) return;

    arv_bk_destroi_nos(a->raiz);
    free(a);
}