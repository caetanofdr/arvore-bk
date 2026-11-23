# Árvore BK em C

Implementação de uma **árvore BK** (Burkhard-Keller) em C para **busca aproximada de palavras**, o tipo de busca usado em corretores ortográficos: dada uma palavra, encontrar todas as palavras do dicionário que estão a no máximo $d$ edições dela.

Trabalho prático da disciplina **Estruturas de Dados** do curso de Ciência da Computação da **UFSM** (2026/2), ministrada pelo prof. Sérgio Mergen.

**Grupo:** Caetano Facco, Tonny Rettore e Tiago Perleberg.

📑 **[Slides da apresentação (PDF)](slides-apresentacao/slides-apresentacao.pdf)**: a explicação completa da árvore BK, com os exemplos passo a passo de inserção, remoção e busca.

---

## Sumário

- [O que é uma árvore BK](#o-que-é-uma-árvore-bk)
- [Como compilar e executar](#como-compilar-e-executar)
- [Interface do TAD](#interface-do-tad)
- [Exemplo de uso](#exemplo-de-uso)
- [Detalhes de implementação](#detalhes-de-implementação)
- [Complexidade](#complexidade)
- [Limitações](#limitações)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Referências](#referências)

---

## O que é uma árvore BK

A árvore BK é uma **árvore métrica**: cada nó guarda uma palavra, e cada filho fica pendurado no pai por uma aresta cujo peso é a **distância** entre as duas palavras. Aqui a distância usada é a de **Levenshtein**, que conta o número mínimo de inserções, remoções e substituições de letras para transformar uma palavra na outra (por exemplo, `gato` → `pato` → `pata` tem distância 2).

Inserindo `gato, gatos, bola, pato, gado, mola, mole, mato`, nessa ordem, a árvore fica assim (o número entre parênteses é a distância ao pai):

```
gato
├── (1) gatos
│   └── (2) pato
│       ├── (1) mato
│       └── (2) gado
└── (4) bola
    ├── (1) mola
    └── (2) mole
```

### Por que a busca é rápida

Como a distância de Levenshtein é uma métrica, ela respeita a **desigualdade triangular**. Se a palavra buscada $s$ está a distância $d$ do nó atual e o raio de busca é $r$, qualquer resposta só pode estar em um filho cujo peso $p$ satisfaz

$$d - r \le p \le d + r$$

Todos os outros filhos (e as subárvores inteiras deles) são descartados sem calcular nenhuma distância. Essa **poda** é o que faz a árvore BK visitar só parte dos nós em vez de comparar a palavra com o dicionário inteiro.

---

## Como compilar e executar

Requisitos: compilador C99 (`cc`, `gcc` ou `clang`) e `make`. Testado em macOS e Linux.

```sh
make          # compila o executável demo
make run      # compila e executa a demonstração
make debug    # compila com AddressSanitizer e UndefinedBehaviorSanitizer e executa
make memoria  # verifica vazamentos (leaks no macOS, valgrind no Linux)
make clean    # apaga o executável
```

A demonstração (`main.c`) mostra, nessa ordem, a inserção, a busca com raios diferentes, a remoção de uma folha, de um nó interno e da raiz, e um pequeno corretor ortográfico interativo. Entre as etapas, aperte **Enter** para continuar.

---

## Interface do TAD

O tipo é opaco: o usuário só enxerga `Arv_bk` e as funções de `arv_bk/arv_bk.h`.

| Função | O que faz |
|---|---|
| `Arv_bk arv_bk_cria(void)` | Cria uma árvore vazia. |
| `void arv_bk_destroi(Arv_bk a)` | Libera a árvore e todas as palavras. |
| `int arv_bk_insere(Arv_bk a, const char *s)` | Insere `s`. Devolve `1` se inseriu e `0` se a palavra já existia. |
| `int arv_bk_remove(Arv_bk a, const char *s)` | Remove `s`. Devolve `1` se removeu e `0` se não encontrou. |
| `char **arv_bk_busca(Arv_bk a, const char *s, int dist_max, int *quant)` | Devolve um vetor com todas as palavras a distância `<= dist_max` de `s` e escreve o tamanho em `*quant`. Sem resultados, devolve `NULL` e `*quant = 0`. |
| `void arv_bk_destroi_busca(char **resultados, int quant)` | Libera o vetor devolvido pela busca. |
| `int arv_bk_tamanho(Arv_bk a)` | Número de palavras na árvore. |
| `int arv_bk_visitados(Arv_bk a)` | Quantos nós a última busca visitou (útil para ver a poda funcionando). |
| `void arv_bk_imprime(Arv_bk a)` | Imprime a árvore no formato mostrado acima. |

A árvore guarda **cópias** das palavras, então a string passada pode ser liberada ou reutilizada depois da inserção.

---

## Exemplo de uso

```c
#include <stdio.h>
#include "arv_bk/arv_bk.h"

int main(void)
{
    Arv_bk a = arv_bk_cria();

    arv_bk_insere(a, "gato");
    arv_bk_insere(a, "pato");
    arv_bk_insere(a, "bola");
    arv_bk_insere(a, "mato");

    int quant;
    char **res = arv_bk_busca(a, "gata", 1, &quant);

    for (int i = 0; i < quant; i++) printf("%s\n", res[i]);

    printf("visitados: %d de %d\n", arv_bk_visitados(a), arv_bk_tamanho(a));

    arv_bk_destroi_busca(res, quant);
    arv_bk_destroi(a);
    return 0;
}
```

Trecho da saída da demonstração, com a árvore de 8 palavras acima:

```
Busca: "gato" | Distancia: 1
Resultados: 5 | Nos visitados: 5/8
- gato
- gatos
- pato
- mato
- gado

Busca: "casa" | Distancia: 1
Resultados: 0 | Nos visitados: 3/8
```

---

## Detalhes de implementação

- **Representação filho-irmão.** Cada nó guarda `palavra`, `peso` (distância ao pai), `filho` (primeiro filho) e `irmao` (próximo nó do mesmo nível). Assim cada nó tem tamanho fixo, mesmo que a quantidade de filhos varie.
- **Filhos ordenados por peso.** A lista de irmãos fica em ordem crescente de peso. Com isso a inserção, a remoção e a busca param de percorrer a lista assim que passam do peso procurado (na busca, assim que o peso passa de $d + r$).
- **Distância de Levenshtein** calculada por programação dinâmica (algoritmo de Wagner e Fischer), com uma matriz $(|a|+1) \times (|b|+1)$.
- **Inserção.** Desce pela árvore seguindo o filho com peso igual à distância até a palavra nova. Quando não existe filho com aquele peso, o novo nó é ligado ali. Palavra repetida (distância 0) não é inserida.
- **Remoção.** A árvore BK não tem um jeito simples de "tapar o buraco" deixado por um nó, porque os pesos dos filhos dependem dele. Por isso o nó é retirado e **todos os nós da sua subárvore são reinseridos** na árvore. Os nós são reaproveitados (não há nova alocação de memória), só os pesos e as ligações mudam.
- **Busca.** Recursiva, aplicando a poda pela desigualdade triangular descrita acima. O vetor de resultados é alocado com o tamanho da árvore e devolvido ao usuário.
- **Falta de memória.** Se um `malloc` falha, o programa imprime um erro em `stderr` e termina.

---

## Complexidade

Sendo $n$ o número de palavras, $h$ a altura da árvore e $L$ o tamanho das palavras, cada cálculo de distância custa $O(L^2)$.

| Operação | Custo |
|---|---|
| Inserção | $O(h \cdot L^2)$ |
| Busca | $O(n \cdot L^2)$ no pior caso; na prática visita só uma fração dos nós, que depende do raio e da forma da árvore |
| Remoção | $O((k+1) \cdot h \cdot L^2)$, sendo $k$ o tamanho da subárvore do nó removido; $O(n^2 \cdot L^2)$ no pior caso |

O desempenho depende muito da **forma da árvore** (que depende da ordem de inserção) e da **distribuição das distâncias** entre as palavras. Com raios grandes, a poda descarta pouco e a busca se aproxima de comparar com todas as palavras.

---

## Limitações

- **Acentos.** A distância é calculada **byte a byte**. Em UTF-8, letras acentuadas ocupam 2 bytes, então `é` contra `e` conta como mais de uma edição. As palavras da demonstração são sem acento por esse motivo.
- **Remoção cara.** Reinserir a subárvore inteira pode custar muito quando o nó removido está perto da raiz.
- **Resultados sem ordem.** A busca devolve as palavras na ordem em que foram encontradas, não da mais próxima para a mais distante.
- **Árvore não balanceada.** A forma depende da ordem de inserção e não há rebalanceamento.

---

## Estrutura do repositório

```
.
├── arv_bk/
│   ├── arv_bk.h   # interface pública do TAD
│   └── arv_bk.c   # implementação
├── slides-apresentacao/
│   └── slides-apresentacao.pdf   # slides da apresentação
├── main.c         # demonstração
└── Makefile
```

---

## Referências

BURKHARD, W. A.; KELLER, R. M. Some approaches to best-match file searching. **Communications of the ACM**, v. 16, n. 4, p. 230-236, 1973.

CHÁVEZ, E.; NAVARRO, G.; BAEZA-YATES, R.; MARROQUÍN, J. L. Searching in metric spaces. **ACM Computing Surveys**, v. 33, n. 3, p. 273-321, 2001.

RAKOWSKI, A.; BERWALDT, G.; SCHMAEDECK, D.; MERGEN, S. Comparação entre diferentes implementações de BK-trees para o problema de busca por intervalo. In: ESCOLA REGIONAL DE BANCO DE DADOS (ERBD), 14., 2018, Rio Grande. **Anais** [...]. Porto Alegre: SBC, 2018. Disponível em: https://sol.sbc.org.br/index.php/erbd/article/view/2829.

WAGNER, R. A.; FISCHER, M. J. The string-to-string correction problem. **Journal of the ACM**, v. 21, n. 1, p. 168-173, 1974.
