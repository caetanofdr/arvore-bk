#include "arv_bk.h"

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

