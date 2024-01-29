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
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->jogador = jogador;
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    return novo_no;
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

No* remover_jogadores(No* raiz, int pontos, ListaJR** lista_removida) {
    if (raiz == NULL) {
        return raiz;
    }

    if (pontos < raiz->jogador.pontos) {
        raiz->esquerda = remover_jogadores(raiz->esquerda, pontos, lista_removida);
    } else if (pontos > raiz->jogador.pontos) {
        raiz->direita = remover_jogadores(raiz->direita, pontos, lista_removida);
    } else {
        ListaJR* novoRemovido = (ListaJR*)malloc(sizeof(ListaJR));
        novoRemovido->jogador = raiz->jogador;
        novoRemovido->proximo = *lista_removida;
        *lista_removida = novoRemovido;

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
        raiz->direita = remover_jogadores(raiz->direita, temp->jogador.pontos, lista_removida);
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

void imprimir_jogadores_removidos(ListaJR* lista_removida) {
    if (lista_removida == NULL) {
        printf("Nenhum jogador removido.\n");
        return;
    }

    printf("\\\\\\\\\\Lista de jogadores removidos://///////\n");
    while (lista_removida != NULL) {
        printf("Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
               lista_removida->jogador.nome, lista_removida->jogador.pontos,
               lista_removida->jogador.rebotes, lista_removida->jogador.assistencias);
        lista_removida = lista_removida->proximo;
    }
}

void liberar_lista_removidos(ListaJR* lista_removida) {
    while (lista_removida != NULL) {
        ListaJR* temp = lista_removida;
        lista_removida = lista_removida->proximo;
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
    ListaJR* lista_removidos = NULL;

    int opcao;
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Jogador novo_jogador;
                printf("\n\n");
                printf("Informe o nome do jogador: ");
                scanf("%s", novo_jogador.nome);
                printf("Informe a pontuação do jogador: ");
                scanf("%d", &novo_jogador.pontos);
                printf("Informe o número de rebotes do jogador: ");
                scanf("%d", &novo_jogador.rebotes);
                printf("Informe o número de assistências do jogador: ");
                scanf("%d", &novo_jogador.assistencias);

                raiz = inserir_jogadores(raiz, novo_jogador);
                printf("O jogador %s inserido com sucesso!\n", novo_jogador.nome);
                break;
            }
            case 2: {
                int pontos_buscado;
                printf("\n\n");
                printf("Informe a pontuação para buscar o jogador: ");
                scanf("%d", &pontos_buscado);

                No* jogador_buscado = buscar_jogadores(raiz, pontos_buscado);
                if (jogador_buscado != NULL) {
                    printf("\\\\\\\\\\Jogador encontrado://///////\n");
                    printf("Nome: %s, Pontos: %d, Rebotes: %d, Assistencias: %d\n",
                           jogador_buscado->jogador.nome, jogador_buscado->jogador.pontos,
                           jogador_buscado->jogador.rebotes, jogador_buscado->jogador.assistencias);
                } else {
                    printf("Jogador com %d pontos não encontrado no jogo.\n", pontos_buscado);
                }
                break;
            }
            case 3: {
                printf("\n\n");
                int pontos_remocao;
                printf("Informe a pontuação para remover o jogador: ");
                scanf("%d", &pontos_remocao);
                raiz = remover_jogadores(raiz, pontos_remocao, &lista_removidos);
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
                imprimir_jogadores_removidos(lista_removidos);
                break;
            case 0:
                printf("Saindo do programa. Obrigado!\n\n");
                break;
            default:
                printf("\n\n");
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    liberar_lista_removidos(lista_removidos);

    return 0;
}