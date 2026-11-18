#include <stdio.h>
#include <stdlib.h>

typedef struct ITEM
{
    int chave;
    int valor;
    struct ITEM *proximo;
} ITEM;

typedef struct
{
    int tamanho;
    ITEM **listas;
} TABELA;

// Funcao hash
int hash(int chave, int m)
{
    return chave % m;
}

// Insere ou atualiza um elemento
void inserir(TABELA *t, int chave, int valor)
{

    int indice = hash(chave, t->tamanho);

    ITEM *atual = t->listas[indice];

    // Verifica se a chave ja existe
    while (atual != NULL)
    {

        if (atual->chave == chave)
        {
            atual->valor = valor;
            return;
        }

        atual = atual->proximo;
    }

    // Cria um novo elemento
    ITEM *novo = malloc(sizeof(ITEM));

    if (novo == NULL)
    {
        printf("Erro de alocacao!\n");
        return;
    }

    novo->chave = chave;
    novo->valor = valor;

    // Insere no inicio da lista
    novo->proximo = t->listas[indice];
    t->listas[indice] = novo;
}

/// TESTE

int main()
{

    TABELA t;
    int m = 7;

    // Inicializa a tabela
    t.tamanho = m;
    t.listas = calloc(m, sizeof(ITEM *));

    if (t.listas == NULL)
    {
        printf("Erro de alocacao!\n");
        return 1;
    }

    // Teste 1: Insercao
    inserir(&t, 10, 100);
    inserir(&t, 17, 200);
    inserir(&t, 24, 300);

    // Teste 2: Atualizacao de uma chave existente
    inserir(&t, 17, 500);

    // Exibe os resultados
    printf("Tabela Hash:\n\n");

    for (int i = 0; i < t.tamanho; i++)
    {

        ITEM *atual = t.listas[i];

        printf("[%d] -> ", i);

        while (atual != NULL)
        {

            printf("(%d, %d) -> ",
                   atual->chave, atual->valor);

            atual = atual->proximo;
        }

        printf("NULL\n");
    }

    return 0;
}