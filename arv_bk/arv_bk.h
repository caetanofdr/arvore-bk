#ifndef ARV_BK_H
#define ARV_BK_H

typedef struct arv_bk *Arv_bk;

Arv_bk arv_bk_cria(void);

void arv_bk_destroi(Arv_bk a);

int arv_bk_insere(Arv_bk a, const char *s);

int arv_bk_remove(Arv_bk a, const char *s);

char **arv_bk_busca(Arv_bk a, const char *s, int dist_max, int *quant);

void arv_bk_destroi_busca(char **resultados, int quant);

void arv_bk_imprime(Arv_bk a);

int arv_bk_tamanho(Arv_bk a);

#endif