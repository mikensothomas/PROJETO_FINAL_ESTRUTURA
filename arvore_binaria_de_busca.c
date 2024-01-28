#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Jogador {
    char nome[50];
    int pontos;
    int rebotes;
    int assistencias;
} Jogador;

typedef struct No {
    Jogador jogador;
    struct No* esquerda;
    struct No* direita;
} No;

No* criarNo(Jogador jogador) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->jogador = jogador;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

No* inserir(No* raiz, Jogador jogador) {
    if (raiz == NULL) {
        return criarNo(jogador);
    }

    if (jogador.pontos < raiz->jogador.pontos) {
        raiz->esquerda = inserir(raiz->esquerda, jogador);
    } else if (jogador.pontos > raiz->jogador.pontos) {
        raiz->direita = inserir(raiz->direita, jogador);
    }

    return raiz;
}

No* buscar(No* raiz, int pontos) {
    if (raiz == NULL || raiz->jogador.pontos == pontos) {
        return raiz;
    }

    if (pontos < raiz->jogador.pontos) {
        return buscar(raiz->esquerda, pontos);
    } else {
        return buscar(raiz->direita, pontos);
    }
}

No* encontrarMinimo(No* no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

No* remover(No* raiz, int pontos) {
    if (raiz == NULL) {
        return raiz;
    }

    if (pontos < raiz->jogador.pontos) {
        raiz->esquerda = remover(raiz->esquerda, pontos);
    } else if (pontos > raiz->jogador.pontos) {
        raiz->direita = remover(raiz->direita, pontos);
    } else {
        if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        No* temp = encontrarMinimo(raiz->direita);
        raiz->jogador = temp->jogador;
        raiz->direita = remover(raiz->direita, temp->jogador.pontos);
    }

    return raiz;
}

void listarEmOrdem(No* raiz) {
    if (raiz != NULL) {
        listarEmOrdem(raiz->esquerda);
        printf("Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
               raiz->jogador.nome, raiz->jogador.pontos, raiz->jogador.rebotes, raiz->jogador.assistencias);
        listarEmOrdem(raiz->direita);
    }
}
void exibirMenu() {
    printf("\nMenu:\n");
    printf("\t1 - Inserir jogador\n");
    printf("\t2 - Buscar jogador por pontos\n");
    printf("\t3 - Remover jogador por pontos\n");
    printf("\t4 - Listar jogadores em ordem de pontos\n");
    printf("\t0 - Sair\n");
}

int main() {
    No* raiz = NULL;

    int opcao;
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Jogador novoJogador;
                printf("Informe o nome do jogador: ");
                scanf("%s", novoJogador.nome);
                printf("Informe a pontuação do jogador: ");
                scanf("%d", &novoJogador.pontos);
                printf("Informe o número de rebotes do jogador: ");
                scanf("%d", &novoJogador.rebotes);
                printf("Informe o número de assistências do jogador: ");
                scanf("%d", &novoJogador.assistencias);

                raiz = inserir(raiz, novoJogador);
                printf("Jogador inserido com sucesso!\n");
                break;
            }
            case 2: {
                int pontosBusca;
                printf("Informe a pontuação para buscar o jogador: ");
                scanf("%d", &pontosBusca);

                No* jogadorBuscado = buscar(raiz, pontosBusca);
                if (jogadorBuscado != NULL) {
                    printf("Jogador encontrado - Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
                           jogadorBuscado->jogador.nome, jogadorBuscado->jogador.pontos,
                           jogadorBuscado->jogador.rebotes, jogadorBuscado->jogador.assistencias);
                } else {
                    printf("Jogador com %d pontos não encontrado.\n", pontosBusca);
                }
                break;
            }
            case 3: {
                int pontosRemocao;
                printf("Informe a pontuação para remover o jogador: ");
                scanf("%d", &pontosRemocao);

                raiz = remover(raiz, pontosRemocao);
                printf("Jogador removido com sucesso!\n");
                break;
            }
            case 4:
                printf("Lista de jogadores em ordem de pontos:\n");
                listarEmOrdem(raiz);
                break;
            case 0:
                printf("Saindo do programa. Obrigado!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}