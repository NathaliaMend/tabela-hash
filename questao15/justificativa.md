# Questão 15 --- Estruturas de dados para uma biblioteca

## Objetivo

O sistema da biblioteca precisa atender a duas necessidades:

1.  Localizar um exemplar pelo ISBN.
2.  Listar todos os livros em ordem alfabética de título.

As estruturas mais apropriadas são uma **tabela hash** para a busca pelo
ISBN e uma **árvore AVL** para manter os livros ordenados pelo título.

## I --- Busca de um exemplar pelo ISBN: tabela hash

A tabela hash é adequada porque o ISBN funciona como identificador de
uma edição específica de um livro. A função hash transforma esse
identificador em um índice do vetor:

\[ h(k) = k `\bmod `{=tex}m \]

Em que:

-   `k` é o ISBN;
-   `m` é o tamanho da tabela;
-   `h(k)` é o índice calculado.

A busca utiliza esse índice para acessar diretamente a lista
correspondente. Quando a função hash distribui bem as chaves, a busca
tem custo médio de **O(1)**.

Podem ocorrer colisões, isto é, ISBNs diferentes podem gerar o mesmo
índice. No arquivo `tabela_hash.c`, as colisões são tratadas por
**encadeamento separado**: cada posição da tabela aponta para uma lista
ligada. Antes de inserir um novo registro, a função percorre a lista
para verificar se o ISBN já existe. Se existir, atualiza o registro;
caso contrário, insere um novo nó.

## II --- Listagem em ordem alfabética: árvore AVL

Uma árvore AVL é uma árvore binária de busca auto-balanceada. Os livros
são organizados de acordo com seus títulos.

Para listar os livros em ordem, utiliza-se o percurso **em ordem**
(*in-order*), que visita:

1.  A subárvore esquerda;
2.  O nó atual;
3.  A subárvore direita.

Como a árvore binária de busca mantém os menores elementos à esquerda e
os maiores à direita, esse percurso produz a listagem ordenada.
Percorrer todos os nós custa **O(n)**. A busca e a inserção custam
**O(log n)** em uma AVL, pois ela mantém sua altura balanceada.

No código `arvore_balanceada.c`, a comparação utiliza o título e, em
caso de títulos iguais, o ISBN como critério de desempate. Assim, dois
livros com o mesmo título, mas ISBNs diferentes, podem ser armazenados.

## III --- Por que o título isolado pode ser uma chave problemática?

O título não identifica necessariamente um livro de forma única. Obras
diferentes podem ter o mesmo título, e uma mesma obra pode possuir
várias edições, traduções ou versões, cada uma com ISBN próprio.

Se o título fosse usado como chave única, um cadastro poderia
sobrescrever outro ou impedir o armazenamento de livros diferentes com o
mesmo título.

Por isso, o ISBN é mais adequado para identificar uma edição. Para
ordenar os livros pelo título, pode-se usar o título como primeiro
critério e o ISBN como desempate.

## Comparação das estruturas

  Necessidade                       Estrutura       Complexidade esperada
  --------------------------------- ------------- -----------------------
  Buscar pelo ISBN                  Tabela hash                O(1) médio
  Inserir/atualizar pelo ISBN       Tabela hash                O(1) médio
  Listar todos em ordem de título   Árvore AVL                       O(n)
  Buscar por título na AVL          Árvore AVL                   O(log n)

*Observação:* as complexidades médias da tabela hash dependem de uma boa
distribuição das chaves e de um tratamento adequado das colisões. No
pior caso, uma operação pode chegar a O(n).

## Conclusão

A tabela hash é a melhor escolha para localizar rapidamente um exemplar
pelo ISBN, enquanto a árvore AVL é mais adequada para manter os livros
organizados e permitir sua listagem em ordem alfabética. As duas
estruturas se complementam porque atendem a necessidades diferentes do
sistema.
