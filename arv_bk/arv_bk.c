#include "arv_bk.h"

#include <stdlib.h>
#include <string.h>

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

static void verifica_memoria(void *p)
{
    if (p != NULL) return;

    exit(EXIT_FAILURE);
}

static No *arv_bk_cria_no(const char *s, int p)
{
    if (s == NULL) return NULL;
    if (p < 0) return NULL;

    No *n = (No*) malloc(sizeof(No));
    verifica_memoria(n);

    int tam_s = strlen(s) + 1;
    n->palavra = (char*) malloc(tam_s * sizeof(char));
    verifica_memoria(n->palavra);

    strcpy(n->palavra, s);

    n->filho = NULL;
    n->irmao = NULL;
    n->peso = p;

    return n;
}

static void arv_bk_destroi_no(No *n)
{
    if (n == NULL) return;

    free(n->palavra);
    free(n);
}

static void arv_bk_destroi_nos(No *n)
{
    if (n == NULL) return;

    No *f = n->filho;
    No *i = n->irmao;

    arv_bk_destroi_no(n);

    arv_bk_destroi_nos(f);
    arv_bk_destroi_nos(i);
}

static int dist_min(int a, int b, int c)
{
    if (a <= b && a <= c) return a;
    else if (b <= a && b <= c) return b;
    else return c;
}

static void dist_destroi_matriz(int **d, int lin)
{
    for (int i = 0; i < lin; i++) {
        free(d[i]);
    }

    free(d);
}

static int **dist_cria_matriz(int lin, int cul)
{
    int **d = (int**) malloc(lin * sizeof(int*));
    verifica_memoria(d);

    for (int i = 0; i < lin; i++) {
        d[i] = (int*) malloc(cul * sizeof(int));
        if (d[i] == NULL) { 
            dist_destroi_matriz(d, i);
            verifica_memoria(NULL);
        }
    }
    
    return d;
}

static int dist_levenshtein(const char *a, const char *b)
{
    int lin = strlen(a) + 1;
    int cul = strlen(b) + 1;

    int **d = dist_cria_matriz(lin, cul);
    verifica_memoria(d);

    for (int i = 0; i < lin; i++) d[i][0] = i;
    for (int j = 0; j < cul; j++) d[0][j] = j;

    for (int i = 1; i < lin; i++) {
        for (int j = 1; j < cul; j++) {
            int custo;

            if (a[i - 1] == b[j - 1]) custo = 0;
            else custo = 1;

            int remocao = d[i - 1][j] + 1;
            int insercao = d[i][j - 1] + 1;
            int igual_ou_substituicao = d[i - 1][j - 1] + custo;

            d[i][j] = dist_min(remocao, insercao, igual_ou_substituicao);
        }
    }

    int dist = d[lin - 1][cul - 1];
    dist_destroi_matriz(d, lin);
    return dist;
}

Arv_bk arv_bk_cria(void)
{
    Arv_bk a = (Arv_bk) malloc(sizeof(struct arv_bk));
    verifica_memoria(a);

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