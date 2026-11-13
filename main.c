
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
    printf("\n[Enter para continuar]");
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void busca_e_imprime(Arv_bk a, const char *s, int dist)
{
    int quant = 0;
    char **busca = arv_bk_busca(a, s, dist, &quant);

    printf("\nBusca: \"%s\" | Distancia: %d\n", s, dist);
    printf("Resultados: %d | Nos visitados: %d/%d\n",
           quant, arv_bk_visitados(a), arv_bk_tamanho(a));

    for (int i = 0; i < quant; i++)
        printf("- %s\n", busca[i]);

    arv_bk_destroi_busca(busca, quant);
}

static void teste_insercao(Arv_bk a)
{
    const char *palavras[] = {
        "gato", "gatos", "bola", "pato",
        "gado", "mola", "mole", "mato"
    };

    printf("\n===== INSERCAO =====\n");

    for (int i = 0; i < 8; i++)
    {
        printf("Inserindo: %s\n", palavras[i]);
        arv_bk_insere(a, palavras[i]);
    }

    printf("\nArvore resultante:\n");
    arv_bk_imprime(a);

    printf("\nTentativa de inserir duplicata:\n");
    printf("Resultado: %d\n", arv_bk_insere(a, "gato"));

    printf("Total de palavras: %d\n", arv_bk_tamanho(a));
}

static void teste_busca(Arv_bk a)
{
    printf("\n===== BUSCA =====\n");

    busca_e_imprime(a, "gato", 0);
    busca_e_imprime(a, "gato", 1);
    busca_e_imprime(a, "gato", 2);
    busca_e_imprime(a, "gata", 1);
    busca_e_imprime(a, "bolo", 2);
    busca_e_imprime(a, "casa", 1);
}

static void teste_remocao(Arv_bk a)
{
    const char *palavras[] = {
        "mato", "gatos", "gato"
    };

    const char *tipos[] = {
        "folha", "no interno", "raiz"
    };

    printf("\n===== REMOCAO =====\n");

    for (int i = 0; i < 3; i++)
    {
        printf("\nRemovendo %s (%s): %d\n",
               palavras[i], tipos[i],
               arv_bk_remove(a, palavras[i]));

        arv_bk_imprime(a);
    }

    printf("\nTotal de palavras: %d\n",
           arv_bk_tamanho(a));
}

int main(void)
{
    Arv_bk a = arv_bk_cria();

    printf("===== DEMONSTRACAO ARVORE BK =====\n");

    teste_insercao(a);
    pausa();

    teste_busca(a);
    pausa();

    teste_remocao(a);
    pausa();

    teste_corretor();

    printf("\n===== FIM DA DEMONSTRACAO =====\n");

    arv_bk_destroi(a);
    return 0;
}
