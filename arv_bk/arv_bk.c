#include "arv_bk.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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

    fprintf(stderr, "Erro: memoria insuficiente.\n");
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

static int **dist_cria_matriz(int lin, int col)
{
    int **d = (int**) malloc(lin * sizeof(int*));
    verifica_memoria(d);

    for (int i = 0; i < lin; i++) {
        d[i] = (int*) malloc(col * sizeof(int));
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
    int col = strlen(b) + 1;

    int **d = dist_cria_matriz(lin, col);
    verifica_memoria(d);

    for (int i = 0; i < lin; i++) d[i][0] = i;
    for (int j = 0; j < col; j++) d[0][j] = j;

    for (int i = 1; i < lin; i++) {
        for (int j = 1; j < col; j++) {
            int custo;

            if (a[i - 1] == b[j - 1]) custo = 0;
            else custo = 1;

            int remocao = d[i - 1][j] + 1;
            int insercao = d[i][j - 1] + 1;
            int igual_ou_substituicao = d[i - 1][j - 1] + custo;

            d[i][j] = dist_min(remocao, insercao, igual_ou_substituicao);
        }
    }

    int dist = d[lin - 1][col - 1];
    dist_destroi_matriz(d, lin);
    return dist;
}

static No *arv_bk_acha_filho(No *pai, int peso)
{
    No *filho = pai->filho;

    while (filho != NULL && filho->peso < peso) {
        filho = filho->irmao;
    }

    if (filho != NULL && filho->peso == peso) return filho;

    return NULL;
}

static void arv_bk_liga_filho(No *pai, No *novo)
{
    No **link = &pai->filho;

    while (*link != NULL && (*link)->peso < novo->peso) {
        link = &(*link)->irmao;
    }

    novo->irmao = *link;
    *link = novo;
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

int arv_bk_insere(Arv_bk a, const char *s)
{
    if (a == NULL) return 0;
    if (s == NULL) return 0;

    if (a->raiz == NULL) {
        a->raiz = arv_bk_cria_no(s, 0);
        if (a->raiz == NULL) return 0;

        a->tamanho = 1;
        return 1;
    }

    No *atual = a->raiz;
    int d = dist_levenshtein(atual->palavra, s);

    while (d != 0) {
        No *filho = arv_bk_acha_filho(atual, d);

        if (filho == NULL) {
            No *novo = arv_bk_cria_no(s, d);
            if (novo == NULL) return 0;

            arv_bk_liga_filho(atual, novo);
            a->tamanho++;
            return 1;
        }

        atual = filho;
        d = dist_levenshtein(atual->palavra, s);
    }

    return 0;
}

static void arv_bk_insere_no(Arv_bk a, No *novo)
{
    novo->filho = NULL;
    novo->irmao = NULL;
    novo->peso = 0;

    if (a->raiz == NULL) {
        a->raiz = novo;
        return;
    }

    No *atual = a->raiz;
    int d = dist_levenshtein(atual->palavra, novo->palavra);

    while (1) {
        if (d == 0) {
            arv_bk_destroi_no(novo);
            a->tamanho--;
            return;
        }

        No *filho = arv_bk_acha_filho(atual, d);

        if (filho == NULL) {
            novo->peso = d;
            arv_bk_liga_filho(atual, novo);
            return;
        }

        atual = filho;
        d = dist_levenshtein(atual->palavra, novo->palavra);
    }
}

static void arv_bk_reinsere_nos(Arv_bk a, No *n)
{
    while (n != NULL) {
        No *filhos = n->filho;
        No *prox = n->irmao;

        arv_bk_insere_no(a, n);
        arv_bk_reinsere_nos(a, filhos);

        n = prox;
    }
}

static int arv_bk_remove_aux(Arv_bk a, No **link, const char *s)
{
    if (*link == NULL) return 0;

    No *atual = *link;
    int d = dist_levenshtein(atual->palavra, s);

    if (d == 0) {
        No *filhos = atual->filho;

        *link = atual->irmao;
        atual->filho = NULL;
        atual->irmao = NULL;

        arv_bk_destroi_no(atual);
        a->tamanho--;

        arv_bk_reinsere_nos(a, filhos);
        return 1;
    }

    No **prox = &atual->filho;

    while (*prox != NULL && (*prox)->peso <= d) {
        if ((*prox)->peso == d) return arv_bk_remove_aux(a, prox, s);

        prox = &(*prox)->irmao;
    }

    return 0;
}

int arv_bk_remove(Arv_bk a, const char *s)
{
    if (a == NULL) return 0;
    if (s == NULL) return 0;

    if (a->raiz == NULL) return 0;

    return arv_bk_remove_aux(a, &a->raiz, s);
}

static void arv_bk_busca_aux(No *atual, const char *s, int dist_max, char **resultados, int *quant)
{
    if (atual == NULL) return;

    int d = dist_levenshtein(s, atual->palavra);

    if (d <= dist_max) {
        int tam = strlen(atual->palavra) + 1;

        resultados[*quant] = (char*) malloc(tam * sizeof(char));
        verifica_memoria(resultados[*quant]);

        strcpy(resultados[*quant], atual->palavra);
        (*quant)++;
    }

    No *filho = atual->filho;

    while (filho != NULL && filho->peso <= d + dist_max) {
        if (filho->peso >= d - dist_max) {
            arv_bk_busca_aux(filho, s, dist_max, resultados, quant);
        }

        filho = filho->irmao;
    }
}

char **arv_bk_busca(Arv_bk a, const char *s, int dist_max, int *quant)
{
    if (quant == NULL) return NULL;

    *quant = 0;

    if (a == NULL) return NULL;
    if (s == NULL) return NULL;
    if (dist_max < 0) return NULL;

    if (a->raiz == NULL) return NULL;

    char **resultados = (char**) malloc(a->tamanho * sizeof(char*));
    verifica_memoria(resultados);

    arv_bk_busca_aux(a->raiz, s, dist_max, resultados, quant);

    if (*quant == 0) {
        free(resultados);
        return NULL;
    }

    return resultados;
}

void arv_bk_destroi_busca(char **resultados, int quant)
{
    if (resultados == NULL) return;

    for (int i = 0; i < quant; i++) {
        free(resultados[i]);
    }

    free(resultados);
}

static void arv_bk_imprime_aux(No *no, int nivel, bool continua[])
{
    No *p = no;

    while (p != NULL) {
        for (int i = 0; i < nivel; i++) {
            if (continua[i]) printf("│   ");
            else printf("    ");
        }

        if (p->irmao != NULL) {
            printf("├── ");
        } else {
            printf("└── ");
        }

        printf("(%d) %s\n", p->peso, p->palavra);

        continua[nivel] = p->irmao != NULL;
        arv_bk_imprime_aux(p->filho, nivel + 1, continua);
        p = p->irmao;
    }
}

void arv_bk_imprime(Arv_bk a)
{
    if (a == NULL) return;

    if (a->raiz == NULL) {
        printf("A arvore esta vazia.\n");
        return;
    }

    printf("%s\n", a->raiz->palavra);

    bool *continua = (bool*) malloc(a->tamanho * sizeof(bool));
    verifica_memoria(continua);

    arv_bk_imprime_aux(a->raiz->filho, 0, continua);

    free(continua);
}

int arv_bk_tamanho(Arv_bk a)
{
    if (a == NULL) return 0;

    return a->tamanho;
}