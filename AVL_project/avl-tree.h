#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    float startX, startY;
    float endX, endY;
    float progress;
    bool isAnimating;
} Animation; 

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

void calculateTreePositions(Node* root, float x, float y, float offset);
void animate_insertion(Node *root, int data);
void drawTree(Node* root, int highlightValue);
void draw_node(float x, float y, int data, bool highlight);

void initAllegro(); 
void gameLoop(); 
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