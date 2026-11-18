#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_TITULO 100

typedef struct {
    long long isbn;
    char titulo[TAM_TITULO];
} LIVRO;

typedef struct NO {
    LIVRO livro;
    int altura;
    struct NO *esquerda;
    struct NO *direita;
} NO;

int maior(int a, int b) {
    return (a > b) ? a : b;
}

int altura(NO *no) {
    return (no == NULL) ? 0 : no->altura;
}

/*
 * Compara primeiro o titulo.
 * Se os titulos forem iguais, usa o ISBN como desempate.
 * Assim, livros com o mesmo titulo podem coexistir na arvore.
 */
int comparar_livros(const LIVRO *a, const LIVRO *b) {
    int resultado = strcmp(a->titulo, b->titulo);

    if (resultado != 0) {
        return resultado;
    }

    if (a->isbn < b->isbn) return -1;
    if (a->isbn > b->isbn) return 1;
    return 0;
}

NO *criar_no(LIVRO livro) {
    NO *novo = malloc(sizeof(NO));

    if (novo == NULL) {
        return NULL;
    }

    novo->livro = livro;
    novo->altura = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

NO *rotacao_direita(NO *y) {
    NO *x = y->esquerda;
    NO *subarvore = x->direita;

    x->direita = y;
    y->esquerda = subarvore;

    y->altura = 1 + maior(altura(y->esquerda), altura(y->direita));
    x->altura = 1 + maior(altura(x->esquerda), altura(x->direita));

    return x;
}

NO *rotacao_esquerda(NO *x) {
    NO *y = x->direita;
    NO *subarvore = y->esquerda;

    y->esquerda = x;
    x->direita = subarvore;

    x->altura = 1 + maior(altura(x->esquerda), altura(x->direita));
    y->altura = 1 + maior(altura(y->esquerda), altura(y->direita));

    return y;
}

int fator_balanceamento(NO *no) {
    return (no == NULL) ? 0 : altura(no->esquerda) - altura(no->direita);
}

/* Insere um livro e rebalanceia a arvore AVL */
NO *inserir(NO *raiz, LIVRO livro, int *sucesso) {
    if (raiz == NULL) {
        NO *novo = criar_no(livro);
        if (novo == NULL) {
            *sucesso = 0;
        }
        return novo;
    }

    int comparacao = comparar_livros(&livro, &raiz->livro);

    if (comparacao < 0) {
        NO *novo = inserir(raiz->esquerda, livro, sucesso);
        if (*sucesso) {
            raiz->esquerda = novo;
        } else {
            return raiz;
        }
    } else if (comparacao > 0) {
        NO *novo = inserir(raiz->direita, livro, sucesso);
        if (*sucesso) {
            raiz->direita = novo;
        } else {
            return raiz;
        }
    } else {
        /* Mesmo titulo e mesmo ISBN: atualiza o registro */
        raiz->livro = livro;
        return raiz;
    }

    raiz->altura = 1 + maior(altura(raiz->esquerda),
                             altura(raiz->direita));

    int balanceamento = fator_balanceamento(raiz);

    /* Caso esquerda-esquerda */
    if (balanceamento > 1 &&
        comparar_livros(&livro, &raiz->esquerda->livro) < 0) {
        return rotacao_direita(raiz);
    }

    /* Caso direita-direita */
    if (balanceamento < -1 &&
        comparar_livros(&livro, &raiz->direita->livro) > 0) {
        return rotacao_esquerda(raiz);
    }

    /* Caso esquerda-direita */
    if (balanceamento > 1 &&
        comparar_livros(&livro, &raiz->esquerda->livro) > 0) {
        raiz->esquerda = rotacao_esquerda(raiz->esquerda);
        return rotacao_direita(raiz);
    }

    /* Caso direita-esquerda */
    if (balanceamento < -1 &&
        comparar_livros(&livro, &raiz->direita->livro) < 0) {
        raiz->direita = rotacao_direita(raiz->direita);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

/* Percurso em ordem: lista os livros pelo titulo */
void listar_em_ordem(NO *raiz) {
    if (raiz != NULL) {
        listar_em_ordem(raiz->esquerda);
        printf("%-30s ISBN: %lld\n",
               raiz->livro.titulo, raiz->livro.isbn);
        listar_em_ordem(raiz->direita);
    }
}

void liberar_arvore(NO *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esquerda);
        liberar_arvore(raiz->direita);
        free(raiz);
    }
}

int main(void) {
    NO *raiz = NULL;

    LIVRO livros[] = {
        {1001, "Dom Casmurro"},
        {1002, "O Pequeno Principe"},
        {1003, "1984"},
        {1004, "A Revolucao dos Bichos"},
        {1005, "Dom Casmurro"} /* Mesmo titulo, ISBN diferente */
    };

    int quantidade = sizeof(livros) / sizeof(livros[0]);

    for (int i = 0; i < quantidade; i++) {
        int sucesso = 1;
        NO *nova_raiz = inserir(raiz, livros[i], &sucesso);

        if (!sucesso) {
            printf("Erro de alocacao!\n");
            liberar_arvore(raiz);
            return 1;
        }

        raiz = nova_raiz;
    }

    printf("Livros em ordem alfabetica de titulo:\n\n");
    listar_em_ordem(raiz);

    liberar_arvore(raiz);
    return 0;
}
