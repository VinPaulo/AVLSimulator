#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro5.h>
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro_primitives.h>
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro_font.h>
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro_ttf.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

// Remove the duplicate Animation structure and keep just one
typedef struct {
    float startX, startY;
    float endX, endY;
    float progress;
    bool isAnimating;
} Animation;  // This will be the only animation structure

typedef struct sNode {
    int data;
    struct sNode* left;
    struct sNode* right;
    int height;
    Animation animation;
    float x, y;
} Node;

void initAnimation(Animation* anim, float startX, float startY, float targetX, float targetY);
void updateAnimation(Animation* anim, float delta_time);
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