#define _CRT_SECURE_NO_WARNINGS
#include "avl-tree.h"
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro5.h>
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro_primitives.h>
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro_font.h>
#include <D:\vinic\OneDrive\Documentos\UFV\Algorítimos e Estruturas de Dados\projeto2025\AVLSimulator\allegro\include\allegro5\allegro_ttf.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float FPS = 60.0;
// Variáveis globais
ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_FONT* font = NULL;

Node* avlTree = NULL;          // Raiz da árvore AVL
int newNodedata = -1;          // Valor do novo nó (-1 significa que não há novo nó)
Animation newNodeAnimation;    // Animação do novo nó
float blinkTimer = 0.0f;       // Timer para o efeito de piscar
float delta_time = 0.016f;     // Tempo entre frames (~60 FPS)

// Função para inicializar a fonte
void initFont() {
    font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Erro ao carregar a fonte.\n");
        exit(EXIT_FAILURE);
    }
}

// Função para liberar a fonte
void destroyFont() {
    if (font) {
        al_destroy_font(font);
    }
}

// Função para inicializar o Allegro
void initAllegro() {
    if (!al_init()) {
        fprintf(stderr, "Erro ao inicializar o Allegro.\n");
        exit(EXIT_FAILURE);
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Erro ao inicializar o addon de primitivas.\n");
        exit(EXIT_FAILURE);
    }

    if (!al_init_font_addon() || !al_init_ttf_addon()) {
        fprintf(stderr, "Erro ao inicializar o addon de fontes.\n");
        exit(EXIT_FAILURE);
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Erro ao criar a janela.\n");
        exit(EXIT_FAILURE);
    }

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    al_resize_display(display, SCREEN_WIDTH, SCREEN_HEIGHT);

    timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
        fprintf(stderr, "Erro ao criar o timer.\n");
        exit(EXIT_FAILURE);
    }

    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Erro ao criar a fila de eventos.\n");
        exit(EXIT_FAILURE);
    }

    initFont(); // Inicializa a fonte

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
}

// Função para calcular a posição final do novo nó na árvore
void calculateFinalPosition(Node* root, int data, float* finalX, float* finalY, float parentX, float parentY) {
    if (root == NULL) return;
    if (data < root->data) {
        *finalX = parentX - 100; // Ajuste a posição X para o filho esquerdo
        *finalY = parentY + 50;  // Ajuste a posição Y para o filho esquerdo
        calculateFinalPosition(root->left, data, finalX, finalY, *finalX, *finalY);
    }
    else {
        *finalX = parentX + 100; // Ajuste a posição X para o filho direito
        *finalY = parentY + 50;  // Ajuste a posição Y para o filho direito
        calculateFinalPosition(root->right, data, finalX, finalY, *finalX, *finalY);
    }
}

// Função para desenhar a árvore
void drawTree(Node* root, int highlightValue) {
    if (root == NULL) return;

    // Desenha o nó atual
    if (root->data == highlightValue) {
        al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(0, 0, 255)); // Destaca o nó
    }
    else {
        al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(255, 255, 255));
    }

    // Desenha o valor do nó
    char buffer[10];
    sprintf(buffer, "%d", root->data);
    al_draw_text(font, al_map_rgb(0, 0, 0), root->x, root->y, ALLEGRO_ALIGN_CENTER, buffer);

    // Desenha o valor do balanceamento
    int balanceFactor = balance(root);
    sprintf(buffer, "[%d]", balanceFactor);
    al_draw_text(font, al_map_rgb(255, 255, 255), root->x + 30, root->y, ALLEGRO_ALIGN_LEFT, buffer);

    // Desenha as conexões com os filhos
    if (root->left != NULL) {
        float dx = root->left->x - root->x;
        float dy = root->left->y - root->y;
        float distance = sqrt(dx * dx + dy * dy);
        float offsetX = (dx / distance) * 30; // Ajuste para a borda do círculo
        float offsetY = (dy / distance) * 30;

        al_draw_line(root->x + offsetX, root->y + offsetY, root->left->x - offsetX, root->left->y - offsetY, al_map_rgb(255, 255, 255), 2);
        drawTree(root->left, highlightValue);
    }
    if (root->right != NULL) {
        float dx = root->right->x - root->x;
        float dy = root->right->y - root->y;
        float distance = sqrt(dx * dx + dy * dy);
        float offsetX = (dx / distance) * 30; // Ajuste para a borda do círculo
        float offsetY = (dy / distance) * 30;

        al_draw_line(root->x + offsetX, root->y + offsetY, root->right->x - offsetX, root->right->y - offsetY, al_map_rgb(0, 0, 255), 2);
        drawTree(root->right, highlightValue);
    }
}

void drawNewNode() {
    if (newNodedata != -1) {
        float x = newNodeAnimation.startX + (newNodeAnimation.endX - newNodeAnimation.startX) * newNodeAnimation.progress;
        float y = newNodeAnimation.startY + (newNodeAnimation.endY - newNodeAnimation.startY) * newNodeAnimation.progress;

        al_draw_filled_circle(x, y, 20, al_map_rgb(255, 255, 255)); // Desenha o novo nó
        if (font) {
            char buffer[10];
            sprintf(buffer, "%d", newNodedata);
            al_draw_text(font, al_map_rgb(0, 0, 0), x, y - 10, ALLEGRO_ALIGN_CENTER, buffer);
        }
        else {
            fprintf(stderr, "Fonte não carregada corretamente.\n");
        }
    }
}

void updateAnimation(Animation* anim, float delta_time) {
    if (anim->isAnimating) {
        anim->progress += delta_time * 4;
        if (anim->progress >= 1.0f) {
            anim->progress = 1.0f;
            anim->isAnimating = false;
        }
    }
}

// Função para lidar com a entrada do usuário
void handleInput(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode >= ALLEGRO_KEY_0 && event.keyboard.keycode <= ALLEGRO_KEY_9) {
            newNodedata = event.keyboard.keycode - ALLEGRO_KEY_0; // Armazena o valor do novo nó

            // Calcula a posição final do novo nó na árvore
            float finalX, finalY;
            calculateFinalPosition(avlTree, newNodedata, &finalX, &finalY, SCREEN_WIDTH / 2, 50); // Posição inicial da raiz

            // Inicia a animação
            newNodeAnimation.startX = 50; // Posição X inicial (canto superior esquerdo)
            newNodeAnimation.startY = 50; // Posição Y inicial
            newNodeAnimation.endX = finalX;
            newNodeAnimation.endY = finalY;
            newNodeAnimation.progress = 0.0f;
            newNodeAnimation.isAnimating = true;
        }
    }
}

// Loop principal do jogo
void gameLoop() {
    bool running = true;

    al_start_timer(timer);

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
            al_acknowledge_resize(event.display.source);
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            blinkTimer += delta_time; // Atualiza o timer do efeito de piscar
            updateAnimation(&newNodeAnimation, delta_time); // Atualiza a animação

            // Quando a animação terminar, insere o novo nó na árvore
            if (!newNodeAnimation.isAnimating && newNodedata != -1) {
                avlTree = insert(avlTree, newNodedata, newNodeAnimation.endX, newNodeAnimation.endY);
                newNodedata = -1; // Reseta o valor do novo nó
            }

            al_clear_to_color(al_map_rgb(0, 0, 0));
            drawTree(avlTree, -1); // Desenha a árvore sem destacar nenhum nó
            drawNewNode(); // Desenha o novo nó
            al_flip_display();
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            handleInput(event);
        }
    }
}

void cleanup() {
    destroyFont();
    if (timer) al_destroy_timer(timer);
    if (event_queue) al_destroy_event_queue(event_queue);
    if (display) al_destroy_display(display);
}
