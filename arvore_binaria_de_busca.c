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

typedef struct ListaJR {
    Jogador jogador;
    struct ListaJR* proximo;
} ListaJR;

No* criar_no(Jogador jogador) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->jogador = jogador;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    return novoNo;
}

No* inserir_jogadores(No* raiz, Jogador jogador) {
    if (raiz == NULL) {
        return criar_no(jogador);
    }

    if (jogador.pontos < raiz->jogador.pontos) {
        raiz->esquerda = inserir_jogadores(raiz->esquerda, jogador);
    } else if (jogador.pontos > raiz->jogador.pontos) {
        raiz->direita = inserir_jogadores(raiz->direita, jogador);
    }

    return raiz;
}

No* buscar_jogadores(No* raiz, int pontos) {
    if (raiz == NULL || raiz->jogador.pontos == pontos) {
        return raiz;
    }

    if (pontos < raiz->jogador.pontos) {
        return buscar_jogadores(raiz->esquerda, pontos);
    } else {
        return buscar_jogadores(raiz->direita, pontos);
    }
}

No* encontrar_minimo(No* no) {
    while (no->esquerda != NULL) {
        no = no->esquerda;
    }
    return no;
}

No* remover_jogadores(No* raiz, int pontos, ListaJR** listaRemovidos) {
    if (raiz == NULL) {
        return raiz;
    }

    if (pontos < raiz->jogador.pontos) {
        raiz->esquerda = remover_jogadores(raiz->esquerda, pontos, listaRemovidos);
    } else if (pontos > raiz->jogador.pontos) {
        raiz->direita = remover_jogadores(raiz->direita, pontos, listaRemovidos);
    } else {
        ListaJR* novoRemovido = (ListaJR*)malloc(sizeof(ListaJR));
        novoRemovido->jogador = raiz->jogador;
        novoRemovido->proximo = *listaRemovidos;
        *listaRemovidos = novoRemovido;

        if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        No* temp = encontrar_minimo(raiz->direita);
        raiz->jogador = temp->jogador;
        raiz->direita = remover_jogadores(raiz->direita, temp->jogador.pontos, listaRemovidos);
    }

    return raiz;
}

void imprimir_em_ordem(No* raiz) {
    if (raiz != NULL) {
        imprimir_em_ordem(raiz->esquerda);
        printf("Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
               raiz->jogador.nome, raiz->jogador.pontos, raiz->jogador.rebotes, raiz->jogador.assistencias);
        imprimir_em_ordem(raiz->direita);
    }
}

int calcular_tamanho(No* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + calcular_tamanho(raiz->esquerda) + calcular_tamanho(raiz->direita);
}

void imprimir_lista_removidos(ListaJR* listaRemovidos) {
    if (listaRemovidos == NULL) {
        printf("Nenhum jogador removido.\n");
        return;
    }

    printf("\\\\\\\\\\Lista de jogadores removidos://///////\n");
    while (listaRemovidos != NULL) {
        printf("Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
               listaRemovidos->jogador.nome, listaRemovidos->jogador.pontos,
               listaRemovidos->jogador.rebotes, listaRemovidos->jogador.assistencias);
        listaRemovidos = listaRemovidos->proximo;
    }
}

void liberar_lista_removidos(ListaJR* listaRemovidos) {
    while (listaRemovidos != NULL) {
        ListaJR* temp = listaRemovidos;
        listaRemovidos = listaRemovidos->proximo;
        free(temp);
    }
}

void exibirMenu() {
    printf("\n\\\\\\\\\\MENU://///////\n\n");
    printf("\t0 - Sair do programa\n");
    printf("\t1 - Inserir jogador no jogo\n");
    printf("\t2 - Buscar jogador pela pontuação\n");
    printf("\t3 - Remover jogador pela pontuação\n");
    printf("\t4 - Imprimir jogadores em ordem de pontos\n");
    printf("\t5 - Listar todos os jogadores removidos\n");
}

int main() {
    No* raiz = NULL;
    ListaJR* listaRemovidos = NULL;

    int opcao;
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Jogador novoJogador;
                printf("\n\n");
                printf("Informe o nome do jogador: ");
                scanf("%s", novoJogador.nome);
                printf("Informe a pontuação do jogador: ");
                scanf("%d", &novoJogador.pontos);
                printf("Informe o número de rebotes do jogador: ");
                scanf("%d", &novoJogador.rebotes);
                printf("Informe o número de assistências do jogador: ");
                scanf("%d", &novoJogador.assistencias);

                raiz = inserir_jogadores(raiz, novoJogador);
                printf("Jogador inserido\n");
                break;
            }
            case 2: {
                int pontosBusca;
                printf("\n\n");
                printf("Informe a pontuação para buscar o jogador: ");
                scanf("%d", &pontosBusca);

                No* jogadorBuscado = buscar_jogadores(raiz, pontosBusca);
                if (jogadorBuscado != NULL) {
                    printf("\\\\\\\\\\Lista de jogador(es) encontrado(s)://///////");
                    printf("Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
                           jogadorBuscado->jogador.nome, jogadorBuscado->jogador.pontos,
                           jogadorBuscado->jogador.rebotes, jogadorBuscado->jogador.assistencias);
                } else {
                    printf("Jogador com %d pontos não encontrado.\n", pontosBusca);
                }
                break;
            }
            case 3: {
                printf("\n\n");
                int pontosRemocao;
                printf("Informe a pontuação para remover o jogador: ");
                scanf("%d", &pontosRemocao);
                raiz = remover_jogadores(raiz, pontosRemocao, &listaRemovidos);
                printf("Jogador removido com sucesso!\n");
                break;
            }
            case 4:
                printf("\n\n");
                printf("\\\\\\\\\\Lista de jogadores em ordem de pontos://///////\n");
                imprimir_em_ordem(raiz);
                printf("O tamanho da arvore é: %d\n", calcular_tamanho(raiz));
                break;
            case 5:
            printf("\n\n");
                imprimir_lista_removidos(listaRemovidos);
                break;
            case 0:
                printf("\n\n");
                printf("Saindo do programa. Obrigado!\n");
                break;
            default:
                printf("\n\n");
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    liberar_lista_removidos(listaRemovidos);

    return 0;
}