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
    return a > b ? a : b;
}

int height(Node* root) {
    return root ? root->height : 0;
}

int balance(Node* root) {
    return root ? height(root->left) - height(root->right) : 0;
}

Node* minValueNode(Node* root) {
    Node* current = root;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = maxValue(height(y->left), height(y->right)) + 1;
    x->height = maxValue(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = maxValue(height(x->left), height(x->right)) + 1;
    y->height = maxValue(height(y->left), height(y->right)) + 1;

    return y;
}

Node* insert(Node* root, int data, float startX, float startY) {
    if (root == NULL) {
        Node* node = newNode(data);
        node->x = startX;
        node->y = startY;
        initAnimation(&node->animation, startX, startY, startX, startY);
        return node;
    }

    if (data < root->data) {
        root->left = insert(root->left, data, startX - 100, startY + 50);
    }
    else if (data > root->data) {
        root->right = insert(root->right, data, startX + 100, startY + 50);
    }
    else {
        return root;
    }

    root->height = 1 + maxValue(height(root->left), height(root->right));
    int balanceFactor = balance(root);

    if (balanceFactor > 1 && data < root->left->data) {
        return rightRotate(root);
    }

    if (balanceFactor < -1 && data > root->right->data) {
        return leftRotate(root);
    }

    if (balanceFactor > 1 && data > root->left->data) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balanceFactor < -1 && data < root->right->data) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* removeNode(Node* root, int data) {
    if (!root) return root;

    if (data < root->data)
        root->left = removeNode(root->left, data);
    else if (data > root->data)
        root->right = removeNode(root->right, data);
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;

            if (!temp) {
                free(root);
                return NULL;
            }
            else {
                *root = *temp;
                free(temp);
            }
        }
        else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = removeNode(root->right, temp->data);
        }
    }

    if (!root) return root;

    root->height = 1 + maxValue(height(root->left), height(root->right));
    int balanceFactor = balance(root);

    if (balanceFactor > 1 && balance(root->left) >= 0) return rightRotate(root);

    if (balanceFactor > 1 && balance(root->left) < 0) {
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
        sprintf(resultBuffer, "Valor %d encontrado na arvore.", value);
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