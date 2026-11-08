#include "arv_bk.h"

#include <stdio.h>

void pausa(void)
{
    printf("\n[Aperte Enter para continuar]");
    fflush(stdout);

    int c = getchar();
    while (c != '\n' && c != EOF) c = getchar();
}

void imprime_quant(int quant)
{
    if (quant == 0) printf("Nenhuma palavra encontrada");
    else if (quant == 1) printf("Foi encontrada 1 palavra");
    else printf("Foram encontradas %d palavras", quant);
}

void teste_insercao(Arv_bk a)
{
    char *palavras[] = {"gato", "gatos", "bola", "pato", "gado", "mola", "mole", "mato"};
    printf("\n======= Teste de insercao: =======\n");
    printf("\nInserindo as palavras:\n");

    for (int i = 0; i < 7; i++) {
        printf("%s\n", palavras[i]);
        arv_bk_insere(a, palavras[i]);
    }

    printf("\nComo ficou na arvore:\n");
    arv_bk_imprime(a);

    printf("\nInserindo \"%s\":\n", palavras[7]);
    arv_bk_insere(a, palavras[7]);
    arv_bk_imprime(a);

    printf("\nInserindo \"gato\" de novo: %d\n", arv_bk_insere(a, "gato"));
}

void busca_e_imprime(Arv_bk a, char *s, int dist)
{
    int quant = 0;
    char **busca = arv_bk_busca(a, s, dist, &quant);

    printf("\nBuscando \"%s\" com distancia %d\n", s, dist);
    imprime_quant(quant);
    printf(" (visitou %d de %d nos):\n", arv_bk_visitados(a), arv_bk_tamanho(a));
    for (int j = 0; j < quant; j++) {
        printf("%s\n", busca[j]);
    }

    arv_bk_destroi_busca(busca, quant);
}

void teste_busca(Arv_bk a)
{
    printf("\n======= Teste de busca: =======\n");

    for (int i = 0; i < 2; i++) {
        busca_e_imprime(a, "gato", i);
    }

    printf("\n--- Palavras fora da arvore ---\n");
    busca_e_imprime(a, "gata", 1);
    busca_e_imprime(a, "bolo", 2);
    busca_e_imprime(a, "casa", 1);
}

void remove_e_imprime(Arv_bk a, char *s)
{
    printf("\nRemovendo \"%s\": %d\n", s, arv_bk_remove(a, s));
    arv_bk_imprime(a);
}

void teste_remocao(Arv_bk a)
{
    printf("\n======= Teste de remocao: =======\n");

    printf("\n--- Remocao de folha ---\n");
    remove_e_imprime(a, "mato");

    printf("\n--- Remocao de no interno ---\n");
    remove_e_imprime(a, "gatos");

    printf("\n--- Remocao da raiz ---\n");
    remove_e_imprime(a, "gato");

    printf("\n--- Remocao de palavra que nao esta na arvore ---\n");
    remove_e_imprime(a, "casa");

    printf("\n--- Busca depois das remocoes ---\n");
    busca_e_imprime(a, "gata", 1);
    busca_e_imprime(a, "pata", 1);
}

int main(void)
{
    // Criação da árvore BK:
    Arv_bk a = arv_bk_cria();

    // Insercao:
    teste_insercao(a);
    pausa();

    // Buscando:
    teste_busca(a);
    pausa();

    // Removendo:
    teste_remocao(a);

    arv_bk_destroi(a);

    return 0;
}
