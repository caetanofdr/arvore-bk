
#include <stdio.h>
#include "arv_bk/arv_bk.h"
#include <string.h>

// INICIO CORRETOR
void teste_corretor(void)
{
    char *dicionario[] = {
        "casa", "carro", "gato", "pato",
        "bola", "mola", "computador",
        "programacao", "universidade",
        "estrutura", "arvore", "algoritmo",
        "linguagem", "codigo", "memoria",
        "teclado", "monitor", "arquivo",
        "faculdade", "estudante"};

    Arv_bk a = arv_bk_cria();
    char palavra[50];
    char **resultados;
    int quant = 0;
    int correta = 0;
    int i;

    for (i = 0; i < 20; i++)
        arv_bk_insere(a, dicionario[i]);

    printf("\n===== CORRETOR ORTOGRAFICO =====\n");
    printf("Digite uma palavra: ");
    scanf("%49s", palavra);

    resultados = arv_bk_busca(a, palavra, 2, &quant);

    for (i = 0; i < quant; i++)
    {
        if (strcmp(resultados[i], palavra) == 0)
            correta = 1;
    }

    if (correta)
    {
        printf("\nA palavra '%s' esta correta!\n", palavra);
    }
    else if (quant > 0)
    {
        printf("\nVoce quis dizer:\n");

        for (i = 0; i < quant; i++)
            printf("- %s\n", resultados[i]);
    }
    else
    {
        printf("\nNenhuma sugestao encontrada.\n");
    }

    printf("\nNos visitados: %d de %d\n",
           arv_bk_visitados(a), arv_bk_tamanho(a));

    arv_bk_destroi_busca(resultados, quant);
    arv_bk_destroi(a);
}

// FIM DO CORRETOR

static void pausa(void)
{
    printf("\n[Aperte Enter para continuar]");
    fflush(stdout);

    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}

static void imprime_quant(int quant)
{
    if (quant == 0)
        printf("Nenhuma palavra encontrada");
    else if (quant == 1)
        printf("Foi encontrada 1 palavra");
    else
        printf("Foram encontradas %d palavras", quant);
}

static void busca_e_imprime(Arv_bk a, const char *s, int dist)
{
    int quant = 0;
    char **busca = arv_bk_busca(a, s, dist, &quant);

    printf("\nBuscando \"%s\" com distancia %d\n", s, dist);

    imprime_quant(quant);

    printf(" (visitou %d de %d nos):\n",
           arv_bk_visitados(a), arv_bk_tamanho(a));

    for (int j = 0; j < quant; j++)
        printf("- %s\n", busca[j]);

    arv_bk_destroi_busca(busca, quant);
}

static void teste_insercao(Arv_bk a)
{
    const char *palavras[] = {
        "gato", "gatos", "bola", "pato",
        "gado", "mola", "mole", "mato"};

    int n = sizeof(palavras) / sizeof(palavras[0]);

    printf("\n======= TESTE DE INSERCAO =======\n");

    printf("\nInserindo as palavras:\n");

    for (int i = 0; i < n - 1; i++)
    {
        printf("%s\n", palavras[i]);
        arv_bk_insere(a, palavras[i]);
    }

    printf("\nArvore apos as primeiras insercoes:\n");
    arv_bk_imprime(a);

    printf("\nInserindo \"%s\":\n", palavras[n - 1]);

    arv_bk_insere(a, palavras[n - 1]);

    arv_bk_imprime(a);

    printf("\nInserindo \"gato\" novamente: %d\n",
           arv_bk_insere(a, "gato"));

    printf("Total de palavras: %d\n", arv_bk_tamanho(a));
}

static void teste_busca(Arv_bk a)
{
    printf("\n======= TESTE DE BUSCA =======\n");

    printf("\n--- Diferentes distancias ---\n");

    busca_e_imprime(a, "gato", 0);
    busca_e_imprime(a, "gato", 1);
    busca_e_imprime(a, "gato", 2);

    printf("\n--- Palavras com erros ---\n");

    busca_e_imprime(a, "gata", 1);
    busca_e_imprime(a, "bolo", 2);

    printf("\n--- Busca sem resultados ---\n");

    busca_e_imprime(a, "casa", 1);
}

static void remove_e_imprime(Arv_bk a, const char *s)
{
    printf("\nRemovendo \"%s\": %d\n",
           s, arv_bk_remove(a, s));

    printf("\nArvore atual:\n");
    arv_bk_imprime(a);

    printf("\nTotal de palavras: %d\n", arv_bk_tamanho(a));
}

static void teste_remocao(Arv_bk a)
{
    printf("\n======= TESTE DE REMOCAO =======\n");

    printf("\n--- Remocao de folha ---\n");
    remove_e_imprime(a, "mato");

    printf("\n--- Remocao de no interno ---\n");
    remove_e_imprime(a, "gatos");

    printf("\n--- Remocao da raiz ---\n");
    remove_e_imprime(a, "gato");

    printf("\n--- Remocao de palavra inexistente ---\n");
    remove_e_imprime(a, "casa");

    printf("\n--- Buscas apos as remocoes ---\n");

    busca_e_imprime(a, "gata", 1);
    busca_e_imprime(a, "pata", 1);
}

int main(void)
{
    Arv_bk a = arv_bk_cria();

    printf("====================================\n");
    printf("       DEMONSTRACAO ARVORE BK\n");
    printf("====================================\n");

    /* Insercao */

    teste_insercao(a);
    pausa();

    /* Busca */

    teste_busca(a);
    pausa();

    /* Remocao */

    teste_remocao(a);
    pausa();

    teste_corretor();

    /* Finalizacao */

    printf("\n====================================\n");
    printf("       FIM DA DEMONSTRACAO\n");
    printf("====================================\n");

    arv_bk_destroi(a);

    return 0;
}
