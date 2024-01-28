#include <stdio.h>
#include <stdlib.h>

// Definição da estrutura do nó da árvore
typedef struct Player {
    char name[50];
    int points;
    int rebounds;
    int assists;
    struct Player* left;
    struct Player* right;
} Player;

// Função para criar um novo jogador
Player* createPlayer(char name[], int points, int rebounds, int assists) {
    Player* newPlayer = (Player*)malloc(sizeof(Player));
    strcpy(newPlayer->name, name);
    newPlayer->points = points;
    newPlayer->rebounds = rebounds;
    newPlayer->assists = assists;
    newPlayer->left = newPlayer->right = NULL;
    return newPlayer;
}

// Função para inserir um jogador na árvore
Player* insertPlayer(Player* root, Player* player, char statistic[]) {
    if (root == NULL) {
        return player;
    }

    // Comparação com base na estatística escolhida
    if (strcmp(statistic, "points") == 0) {
        if (player->points < root->points) {
            root->left = insertPlayer(root->left, player, statistic);
        } else {
            root->right = insertPlayer(root->right, player, statistic);
        }
    } else if (strcmp(statistic, "rebounds") == 0) {
        // Adapte para outras estatísticas, se necessário
        // (repeats the structure for other statistics)
    }

    return root;
}

// Função para buscar um jogador com uma quantidade específica de pontos
Player* searchPlayer(Player* root, int targetPoints) {
    if (root == NULL || root->points == targetPoints) {
        return root;
    }

    if (targetPoints < root->points) {
        return searchPlayer(root->left, targetPoints);
    } else {
        return searchPlayer(root->right, targetPoints);
    }
}

// Função para encontrar o jogador com a menor pontuação (utilizada para remoção)
Player* findMin(Player* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

// Função para remover um jogador da árvore
Player* removePlayer(Player* root, int targetPoints) {
    if (root == NULL) {
        return root;
    }

    if (targetPoints < root->points) {
        root->left = removePlayer(root->left, targetPoints);
    } else if (targetPoints > root->points) {
        root->right = removePlayer(root->right, targetPoints);
    } else {
        // Jogador encontrado, realizar a remoção
        if (root->left == NULL) {
            Player* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Player* temp = root->left;
            free(root);
            return temp;
        }

        Player* temp = findMin(root->right);
        strcpy(root->name, temp->name);
        root->points = temp->points;
        root->rebounds = temp->rebounds;
        root->assists = temp->assists;
        root->right = removePlayer(root->right, temp->points);
    }

    return root;
}

// Função para listar jogadores em ordem baseada na chave de busca
void inorderTraversal(Player* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%s - Points: %d, Rebounds: %d, Assists: %d\n", root->name, root->points, root->rebounds, root->assists);
        inorderTraversal(root->right);
    }
}

// Função para liberar a memória da árvore
void freeTree(Player* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

int main() {
    Player* root = NULL;

    // Inserção de jogadores
    root = insertPlayer(root, createPlayer("Player1", 20, 5, 10), "points");
    root = insertPlayer(root, createPlayer("Player2", 15, 8, 12), "points");
    root = insertPlayer(root, createPlayer("Player3", 25, 3, 8), "points");

    // Busca por jogadores com uma quantidade específica de pontos
    int targetPoints = 20;
    Player* foundPlayer = searchPlayer(root, targetPoints);
    if (foundPlayer != NULL) {
        printf("Player found: %s - Points: %d\n", foundPlayer->name, foundPlayer->points);
    } else {
        printf("Player not found with %d points\n", targetPoints);
    }

    // Remoção de jogadores
    int pointsToRemove = 15;
    root = removePlayer(root, pointsToRemove);
    printf("Player with %d points removed.\n", pointsToRemove);

    // Listagem dos jogadores em ordem de pontuação
    printf("\nPlayers in order of points:\n");
    inorderTraversal(root);

    // Liberação de memória
    freeTree(root);

    return 0;
}