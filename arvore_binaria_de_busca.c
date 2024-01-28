#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Jogador{
    char nome[50];
    int pontos;
    int rebotes;
    int assistencias;
} Jogador;

typedef struct No {
    Jogador jogador;
    struct No *esquerda;
    struct No *direita;
} No;

No *criar_novo_no(Jogador jogador) {
    No *novo_no = (No *)malloc(sizeof(No));
    if (novo_no != NULL) {
        novo_no->jogador = jogador;
        novo_no->esquerda = NULL;
        novo_no->direita = NULL;
    }
    return novo_no;
}

No *inserir_jogador(No *raiz, Jogador jogador, char chave_de_busca[]) {
    if (raiz == NULL) {
        return criar_novo_no(jogador);
    }

    int valorAtual = 0;
    if (strcmp(chave_de_busca, "pontos") == 0) {
        valorAtual = raiz->jogador.pontos;
    } else if (strcmp(chave_de_busca, "rebotes") == 0) {
        valorAtual = raiz->jogador.rebotes;
    } else if (strcmp(chave_de_busca, "assistencias") == 0) {
        valorAtual = raiz->jogador.assistencias;
    }

    if (valorAtual > jogador.pontos) {
        raiz->esquerda = inserir_jogador(raiz->esquerda, jogador, chave_de_busca);
    } else if (valorAtual < jogador.pontos) {
        raiz->direita = inserir_jogador(raiz->direita, jogador, chave_de_busca);
    }

    return raiz;
}

No *buscar_jogador(No *raiz, int pontos, char chaveDeBusca[]) {
    if (raiz == NULL || (strcmp(chaveDeBusca, "pontos") == 0 && raiz->jogador.pontos == pontos) ||
        (strcmp(chaveDeBusca, "rebotes") == 0 && raiz->jogador.rebotes == pontos) ||
        (strcmp(chaveDeBusca, "assistencias") == 0 && raiz->jogador.assistencias == pontos)) {
        return raiz;
    }

    if (raiz->jogador.pontos > pontos) {
        return buscar_jogador(raiz->esquerda, pontos, chaveDeBusca);
    } else {
        return buscar_jogador(raiz->direita, pontos, chaveDeBusca);
    }
}

No *encontrar_no_minimo(No *raiz) {
    while (raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

No *remover_jogador(No *raiz, int pontos, char chaveDeBusca[]) {
    if (raiz == NULL) {
        return raiz;
    }

    int valorAtual = 0;
    if (strcmp(chaveDeBusca, "pontos") == 0) {
        valorAtual = raiz->jogador.pontos;
    } else if (strcmp(chaveDeBusca, "rebotes") == 0) {
        valorAtual = raiz->jogador.rebotes;
    } else if (strcmp(chaveDeBusca, "assistencias") == 0) {
        valorAtual = raiz->jogador.assistencias;
    }

    if (pontos < valorAtual) {
        raiz->esquerda = remover_jogador(raiz->esquerda, pontos, chaveDeBusca);
    }
    else if (pontos > valorAtual) {
        raiz->direita = remover_jogador(raiz->direita, pontos, chaveDeBusca);
    }
    else {
        if (raiz->esquerda == NULL) {
            No *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        No *temp = encontrar_no_minimo(raiz->direita);
        raiz->jogador = temp->jogador;
        raiz->direita = remover_jogador(raiz->direita, temp->jogador.pontos, chaveDeBusca);
    }
    return raiz;
}

void listar_em_ordem(No *raiz, char chaveDeBusca[]) {
    if (raiz != NULL) {
        listar_em_ordem(raiz->esquerda, chaveDeBusca);

        printf("%s - %s: %d\n", raiz->jogador.nome, chaveDeBusca, 
               (strcmp(chaveDeBusca, "pontos") == 0) ? raiz->jogador.pontos :
               (strcmp(chaveDeBusca, "rebotes") == 0) ? raiz->jogador.rebotes :
               raiz->jogador.assistencias);

        listar_em_ordem(raiz->direita, chaveDeBusca);
    }
}

void liberar_arvore(No *raiz) {
    if (raiz != NULL) {
        liberar_arvore(raiz->esquerda);
        liberar_arvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    No *arvore = NULL;

    Jogador jogador1 = {"Jogador1", 20, 5, 10};
    Jogador jogador2 = {"Jogador2", 15, 8, 12};
    Jogador jogador3 = {"Jogador3", 25, 3, 8};

    arvore = inserir_jogador(arvore, jogador1, "pontos");
    arvore = inserir_jogador(arvore, jogador2, "pontos");
    arvore = inserir_jogador(arvore, jogador3, "pontos");

    No *resultadoBusca = buscar_jogador(arvore, 15, "pontos");
    if (resultadoBusca != NULL) {
        printf("Jogador encontrado: %s\n", resultadoBusca->jogador.nome);
    } else {
        printf("Jogador não encontrado.\n");
    }

    arvore = remover_jogador(arvore, 20, "pontos");

    printf("Lista de jogadores em ordem de pontos:\n");
    listar_em_ordem(arvore, "pontos");

    liberar_arvore(arvore);

    return 0;
}
