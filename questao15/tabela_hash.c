#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 11
#define TAM_TITULO 100

typedef struct {
    long long isbn;
    char titulo[TAM_TITULO];
} LIVRO;

typedef struct ITEM {
    LIVRO livro;
    struct ITEM *proximo;
} ITEM;

typedef struct {
    ITEM *listas[M];
} TABELA_HASH;

/* h(k) = k mod M */
int hash(long long isbn) {
    return (int)(isbn % M);
}

/* Inicializa todas as listas da tabela */
void inicializar(TABELA_HASH *t) {
    for (int i = 0; i < M; i++) {
        t->listas[i] = NULL;
    }
}

/*
 * Insere um livro ou atualiza o titulo caso o ISBN ja exista.
 * Retorna 1 em caso de sucesso e 0 se ocorrer erro de alocacao.
 */
int inserir_ou_atualizar(TABELA_HASH *t, long long isbn,
                         const char *titulo) {
    int indice = hash(isbn);
    ITEM *atual = t->listas[indice];

    /* Procura o ISBN na lista para evitar duplicacao */
    while (atual != NULL) {
        if (atual->livro.isbn == isbn) {
            snprintf(atual->livro.titulo, TAM_TITULO, "%s", titulo);
            return 1;
        }
        atual = atual->proximo;
    }

    /* ISBN ainda nao existe: cria um novo elemento */
    ITEM *novo = malloc(sizeof(ITEM));
    if (novo == NULL) {
        return 0;
    }

    novo->livro.isbn = isbn;
    snprintf(novo->livro.titulo, TAM_TITULO, "%s", titulo);

    /* Encadeamento separado: insere no inicio da lista */
    novo->proximo = t->listas[indice];
    t->listas[indice] = novo;

    return 1;
}

/* Busca um livro pelo ISBN */
LIVRO *buscar_por_isbn(TABELA_HASH *t, long long isbn) {
    int indice = hash(isbn);
    ITEM *atual = t->listas[indice];

    while (atual != NULL) {
        if (atual->livro.isbn == isbn) {
            return &atual->livro;
        }
        atual = atual->proximo;
    }

    return NULL;
}

/* Mostra o conteudo da tabela, incluindo as colisoes */
void imprimir_tabela(TABELA_HASH *t) {
    for (int i = 0; i < M; i++) {
        ITEM *atual = t->listas[i];

        printf("[%d] -> ", i);
        while (atual != NULL) {
            printf("(%lld, %s) -> ",
                   atual->livro.isbn, atual->livro.titulo);
            atual = atual->proximo;
        }
        printf("NULL\n");
    }
}

/* Libera os nos alocados dinamicamente */
void liberar_tabela(TABELA_HASH *t) {
    for (int i = 0; i < M; i++) {
        ITEM *atual = t->listas[i];

        while (atual != NULL) {
            ITEM *aux = atual;
            atual = atual->proximo;
            free(aux);
        }

        t->listas[i] = NULL;
    }
}

int main(void) {
    TABELA_HASH tabela;
    inicializar(&tabela);

    /* Estes ISBNs de exemplo geram colisoes com M = 11 */
    if (!inserir_ou_atualizar(&tabela, 1001, "Dom Casmurro") ||
        !inserir_ou_atualizar(&tabela, 1012, "O Pequeno Principe") ||
        !inserir_ou_atualizar(&tabela, 1023, "1984")) {
        printf("Erro ao inserir livro.\n");
        liberar_tabela(&tabela);
        return 1;
    }

    /* Atualiza o titulo do ISBN 1012, sem criar outra entrada */
    inserir_ou_atualizar(&tabela, 1012, "O Pequeno Principe - Edicao especial");

    printf("Tabela hash:\n");
    imprimir_tabela(&tabela);

    printf("\nBusca pelo ISBN 1012:\n");
    LIVRO *encontrado = buscar_por_isbn(&tabela, 1012);

    if (encontrado != NULL) {
        printf("ISBN: %lld\nTitulo: %s\n",
               encontrado->isbn, encontrado->titulo);
    } else {
        printf("Livro nao encontrado.\n");
    }

    liberar_tabela(&tabela);
    return 0;
}
