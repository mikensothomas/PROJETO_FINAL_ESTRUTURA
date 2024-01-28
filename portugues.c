#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um jogador
typedef struct {
    char nome[50];
    int pontos;
    int rebotes;
    int assistencias;
} Jogador;

// Estrutura do nó da árvore
typedef struct Node {
    Jogador jogador;
    struct Node *esquerda;
    struct Node *direita;
} Node;

// Função para criar um novo nó
Node *criarNo(Jogador jogador) {
    Node *novoNo = (Node *)malloc(sizeof(Node));
    if (novoNo != NULL) {
        novoNo->jogador = jogador;
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
    }
    return novoNo;
}

// Função para inserir um jogador na árvore
Node *inserir(Node *raiz, Jogador jogador, char chaveDeBusca[]) {
    if (raiz == NULL) {
        return criarNo(jogador);
    }

    // Escolha da estatística como chave de busca
    int valorAtual = 0;
    if (strcmp(chaveDeBusca, "pontos") == 0) {
        valorAtual = raiz->jogador.pontos;
    } else if (strcmp(chaveDeBusca, "rebotes") == 0) {
        valorAtual = raiz->jogador.rebotes;
    } else if (strcmp(chaveDeBusca, "assistencias") == 0) {
        valorAtual = raiz->jogador.assistencias;
    }

    // Inserir recursivamente à esquerda ou à direita
    if (valorAtual > jogador.pontos) {
        raiz->esquerda = inserir(raiz->esquerda, jogador, chaveDeBusca);
    } else if (valorAtual < jogador.pontos) {
        raiz->direita = inserir(raiz->direita, jogador, chaveDeBusca);
    }

    return raiz;
}

// Função para buscar um jogador com uma quantidade específica de pontos
Node *buscar(Node *raiz, int pontos, char chaveDeBusca[]) {
    if (raiz == NULL || (strcmp(chaveDeBusca, "pontos") == 0 && raiz->jogador.pontos == pontos) ||
        (strcmp(chaveDeBusca, "rebotes") == 0 && raiz->jogador.rebotes == pontos) ||
        (strcmp(chaveDeBusca, "assistencias") == 0 && raiz->jogador.assistencias == pontos)) {
        return raiz;
    }

    if (raiz->jogador.pontos > pontos) {
        return buscar(raiz->esquerda, pontos, chaveDeBusca);
    } else {
        return buscar(raiz->direita, pontos, chaveDeBusca);
    }
}

// Função auxiliar para encontrar o nó mínimo de uma árvore
Node *encontrarMinimo(Node *raiz) {
    while (raiz->esquerda != NULL) {
        raiz = raiz->esquerda;
    }
    return raiz;
}

// Função para remover um jogador da árvore
Node *remover(Node *raiz, int pontos, char chaveDeBusca[]) {
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

    // Buscar na subárvore à esquerda
    if (pontos < valorAtual) {
        raiz->esquerda = remover(raiz->esquerda, pontos, chaveDeBusca);
    }
    // Buscar na subárvore à direita
    else if (pontos > valorAtual) {
        raiz->direita = remover(raiz->direita, pontos, chaveDeBusca);
    }
    // Encontrou o nó a ser removido
    else {
        // Nó com apenas um filho ou nenhum filho
        if (raiz->esquerda == NULL) {
            Node *temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            Node *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // Nó com dois filhos: encontrar o sucessor (menor nó na subárvore à direita)
        Node *temp = encontrarMinimo(raiz->direita);
        raiz->jogador = temp->jogador;
        // Remover o sucessor
        raiz->direita = remover(raiz->direita, temp->jogador.pontos, chaveDeBusca);
    }
    return raiz;
}

// Função para listar jogadores em ordem
void listarEmOrdem(Node *raiz, char chaveDeBusca[]) {
    if (raiz != NULL) {
        listarEmOrdem(raiz->esquerda, chaveDeBusca);

        printf("%s - %s: %d\n", raiz->jogador.nome, chaveDeBusca, 
               (strcmp(chaveDeBusca, "pontos") == 0) ? raiz->jogador.pontos :
               (strcmp(chaveDeBusca, "rebotes") == 0) ? raiz->jogador.rebotes :
               raiz->jogador.assistencias);

        listarEmOrdem(raiz->direita, chaveDeBusca);
    }
}

// Função para liberar a memória da árvore
void liberarArvore(Node *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main() {
    Node *arvore = NULL;

    Jogador jogador1 = {"Jogador1", 20, 5, 10};
    Jogador jogador2 = {"Jogador2", 15, 8, 12};
    Jogador jogador3 = {"Jogador3", 25, 3, 8};

    // Inserção de jogadores na árvore usando a estatística 'pontos'
    arvore = inserir(arvore, jogador1, "pontos");
    arvore = inserir(arvore, jogador2, "pontos");
    arvore = inserir(arvore, jogador3, "pontos");

    // Busca por jogadores com uma quantidade específica de pontos
    Node *resultadoBusca = buscar(arvore, 15, "pontos");
    if (resultadoBusca != NULL) {
        printf("Jogador encontrado: %s\n", resultadoBusca->jogador.nome);
    } else {
        printf("Jogador não encontrado.\n");
    }

    // Remoção de um jogador da árvore
    arvore = remover(arvore, 20, "pontos");

    // Listagem dos jogadores em ordem de pontos
    printf("Lista de jogadores em ordem de pontos:\n");
    listarEmOrdem(arvore, "pontos");

    // Liberar memória da árvore
    liberarArvore(arvore);

    return 0;
}
