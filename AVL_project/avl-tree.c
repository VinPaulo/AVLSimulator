#define _CRT_SECURE_NO_WARNINGS
#include "avl-tree.h"

Node* newNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Erro ao alocar memoria para o no.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int maxValue(int a, int b) {
    return a > b ? a : b; // Retorna o maior valor entre a e b
}

int height(Node* root) {
    return root ? root->height : 0; // Se tiver elemento, retorna a altura da raíz, caso contrário a altura é 0
}

int balance(Node* root) {
    return root ? height(root->left) - height(root->right) : 0; // Se a raíz tiver elementos, retorna a diferença entre a altura da subárvore esquerda e direita, caso contrário retorna 0
}

Node* minValueNode(Node* root) {
    Node* current = root; // Nó atual recebe a raíz
    while (current && current->left) {
        current = current->left; // Atual recebe o nó da esquerda e percorre a subárvore esquerda
    }
    return current;
}

Node* rightRotate(Node* y) {
    Node* x = y->left; // Nó x recebe o nó da esquerda de y
    Node* T2 = x->right; // Nó T2 recebe o nó da direita de x (T2 é a subárvore direita)

    x->right = y; // Rotação simples à esquerda
    y->left = T2; // Rotação simples à direita  

    y->height = maxValue(height(y->left), height(y->right)) + 1; // Atualiza a altura da árvore
    x->height = maxValue(height(x->left), height(x->right)) + 1; // Atualiza a altura da árvore

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right; // A mesma coisa da função acima, porém faz para a esquerda
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxValue(height(x->left), height(x->right)) + 1;
    y->height = maxValue(height(y->left), height(y->right)) + 1;

    return y;
}

Node* insert(Node* root, int data, float startX, float startY) {
    if (root == NULL) {
        Node* node = newNode(data); // Cria um novo nó
        node->x = startX; // Posição horizontal do nó
        node->y = startY; // A mesma coisa porém vertical
        initAnimation(&node->animation, startX, startY, startX, startY);
        return node;
    }

    animate_insertion(root, data);

    if (data < root->data) {
        root->left = insert(root->left, data, startX - 100, startY + 50); // rotação simples à esquerda
    }
    else if (data > root->data) {
        root->right = insert(root->right, data, startX + 100, startY + 50); // rotação simples à direita
    }
    else {
        return root;
    }

    root->height = 1 + maxValue(height(root->left), height(root->right)); // atualização da altura da árvore depois da inserção dos elementos
    int balanceFactor = balance(root); // fator de balanceamento recebe a função que faz o cálculo do balanceamento 

    if (balanceFactor > 1 && data < root->left->data) {
        return rightRotate(root); // rotação simples à direita (caso o fator de balanceamento seja maior que 1)
    }

    if (balanceFactor < -1 && data > root->right->data) { 
        return leftRotate(root); // rotação simples à esquerda (caso o fator de balanceamento seja menor que -1)
    }

    if (balanceFactor > 1 && data > root->left->data) { 
        root->left = leftRotate(root->left); // rotação dupla à esquerda (caso o fator de balanceamento seja maior que 1 e o valor seja maior que o valor do nó à esquerda)
        return rightRotate(root); // rotação simples à direita 
    }

    if (balanceFactor < -1 && data < root->right->data) {
        root->right = rightRotate(root->right); // rotação dupla à direita (a mesma coisa da função de cima, só que para a direita)
        return leftRotate(root); // rotação simples à esquerda
    }

    return root;
}

Node* removeNode(Node* root, int data) {
    if (!root) return root;

    if (data < root->data)
        root->left = removeNode(root->left, data); // Faz a remoção da subárvore esquerda
    else if (data > root->data)
        root->right = removeNode(root->right, data); // O mesmo para a direita
    else {
        if (!root->left || !root->right) { // Se o nó tiver apenas um filho
            Node* temp = root->left ? root->left : root->right; // O temp recebe o nó da esquerda ou direita

            if (!temp) {
                free(root); // Libera o nó
                return NULL;
            }
            else {
                *root = *temp; // Copia o conteúdo de temp para root
                free(temp);
            }
        }
        else {
            Node* temp = minValueNode(root->right); // O temp recebe o menor valor da subárvore direita
            root->data = temp->data;
            root->right = removeNode(root->right, temp->data); // Remove o nó
        }
    }

    if (!root) return root;

    root->height = 1 + maxValue(height(root->left), height(root->right)); // Atualiza a altura da árvore
    int balanceFactor = balance(root);

    if (balanceFactor > 1 && balance(root->left) >= 0) return rightRotate(root); // Rotação simples à direita

    if (balanceFactor > 1 && balance(root->left) < 0) { // A mesma funcionalidade da função de inserir elemento 
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balanceFactor < -1 && balance(root->right) <= 0) return leftRotate(root);

    if (balanceFactor < -1 && balance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

bool search(Node* root, int value, char* resultBuffer) {
    if (!root) {
        sprintf(resultBuffer, "Valor %d nao encontrado na arvore.", value);
        return false;
    }

    if (root->data == value) {
        sprintf(resultBuffer, "Valor %d encontrado na arvore.", value); // Converte o valor para string
        return true;
    }

    if (value < root->data) {
        return search(root->left, value, resultBuffer); // Busca na subarvore esquerda
    } else {
        return search(root->right, value, resultBuffer); // Busca na subarvore direita
    }
}

Node* searchNode(Node* root, int value) {
    if (root == NULL) {
        return NULL; // Valor nao encontrado
    }
    if (root->data == value) {
        return root; // Valor encontrado
    }
    if (value < root->data) {
        return searchNode(root->left, value); // Busca na subarvore esquerda
    } else {
        return searchNode(root->right, value); // Busca na subarvore direita
    }
}

void inorder(Node* root, char* buffer) {
    if (!root) return;

    inorder(root->left, buffer); // Percorre a subárvore esquerda

    char temp[20];
    sprintf(temp, "%d ", root->data); // Converte o valor do nó para string
    strcat(buffer, temp); // Adiciona o valor ao buffer

    inorder(root->right, buffer); // Percorre a subárvore direita
}

void preorder(Node* root, char* buffer) {
    if (!root) return;

    char temp[20];
    sprintf(temp, "%d ", root->data); // Converte o valor do nó para string
    strcat(buffer, temp); // Adiciona o valor ao buffer

    preorder(root->left, buffer); // Percorre a subárvore esquerda
    preorder(root->right, buffer); // Percorre a subárvore direita
}

void postorder(Node* root, char* buffer) {
    if (!root) return;

    postorder(root->left, buffer); // Percorre a subárvore esquerda
    postorder(root->right, buffer); // Percorre a subárvore direita

    char temp[20];
    sprintf(temp, "%d ", root->data); // Converte o valor do nó para string
    strcat(buffer, temp); // Adiciona o valor ao buffer
}