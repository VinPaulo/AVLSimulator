#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    float startX, startY; // Posição inicial
    float endX, endY;     // Posição final
    float progress;       // Progresso da animação (0 a 1)
    bool isAnimating;     // Indica se a animação está ativa
} Animation;

typedef struct sNode {
    int data;
    struct sNode* left;
    struct sNode* right;
    int height;
    Animation animation;
    float x, y; // Adicione estas linhas para armazenar as coordenadas do nó
} Node;



void initAnimation(Node* node, float startX, float startY, float targetX, float targetY, float duration);
void updateAnimation(Node* node, float delta_time);
bool animationActive(Node* node);
void updateTreeAnimations(Node* root, float delta_time, ALLEGRO_EVENT_QUEUE* queue);

// Fun��es de visualiza��o e c�lculo
void calculateTreePositions(Node* root, float x, float y, float offset);
void drawTree(Node* root, int highlightValue);

// Fun��es principais da visualiza��o com Allegro
void initAllegro(); // Inicializa��o do Allegro
void gameLoop();    // Loop principal do programa
void initFont();
void destroyFont();

Node* newNode(int data);
int maxValue(int a, int b);
int height(Node* root);
int balance(Node* root);
Node* minValueNode(Node* root);
Node* rightRotate(Node* y);
Node* leftRotate(Node* x);
Node* insert(Node* root, int data, float startX, float startY);
Node* removeNode(Node* root, int data);
bool search(Node* root, int value, char* resultBuffer);
void inorder(Node* root, char* buffer);
void preorder(Node* root, char* buffer);
void postorder(Node* root, char* buffer);

#endif // AVL_TREE_H