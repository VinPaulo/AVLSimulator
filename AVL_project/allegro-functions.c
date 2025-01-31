#define _CRT_SECURE_NO_WARNINGS
#include "avl-tree.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800; // Tamanho da janela (Horizontal)
const int SCREEN_HEIGHT = 600; // Tamanho da janela (Vertical)
const float FPS = 60.0; 
// Variáveis globais
ALLEGRO_DISPLAY* display = NULL; // Janela vazia (Inicar o display preto) 
ALLEGRO_EVENT_QUEUE* event_queue = NULL; // Fila de eventos (Ordem de acontecimentos)  
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_FONT* font = NULL; // Fonte vazio (Até receber o elemento)

Node* avlTree = NULL;  // Inicia o desenho da árvore vazio
int newNodedata = -1; // Inicia o novo nó com -1 (Fator de balanceamento [-1,1])
Animation newNodeAnimation; // Iniciar a animação do nó 
float blinkTimer = 0.0f; // Tempo de (piscar) do nó alocado
float delta_time = 0.016f; // Atualização da animação milisegundos

// Função para inicializar a fonte
void initFont() {
    font = al_create_builtin_font(); // Cria fonte interna (Fonte externa não funciona)
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

    al_set_new_display_flags(ALLEGRO_RESIZABLE); // Janela adaptável (Conforme crescimento da árvore)
    al_resize_display(display, SCREEN_WIDTH, SCREEN_HEIGHT); // Janalea com tamanho inicial

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

    initFont();

    al_register_event_source(event_queue, al_get_display_event_source(display)); // Registra o (contéudo do display)
    al_register_event_source(event_queue, al_get_timer_event_source(timer)); // Registra o (tempo)
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_keyboard_event_source()); // Registra o (a entrada do teclado)
}

void initAnimation(Animation* anim, float startX, float startY, float targetX, float targetY) {
    anim->startX = startX; // Posição inicial do nó
    anim->startY = startY; // Mesma coisa 
    anim->endX = targetX; // Posição final do nó
    anim->endY = targetY; // Mesma coisa
    anim->progress = 0.0f;
    anim->isAnimating = true; // Inicia a animação
}

void updateTreeAnimations(Node* root, float delta_time, ALLEGRO_EVENT_QUEUE* queue) {
    if (root != NULL) {
        updateTreeAnimations(root->left, delta_time, queue); // Atualiza a animação da subárvore esquerda 
        updateTreeAnimations(root->right, delta_time, queue); // Mesma coisa só que para a direita 

        updateAnimation(&root->animation, delta_time); // Atualiza a animação do nó 

        al_clear_to_color(al_map_rgb(0, 0, 0)); // Limpa o display
        drawTree(root, -1); // Desenha a árvore sem destacar nenhum nó
        al_flip_display(); 

        al_rest(delta_time);
    }
}

void calculateTreePositions(Node* root, float x, float y, float offset) {
    if (root == NULL) return;

    root->x = x; // Posição horizontal do nó
    root->y = y; // Posição vertical do nó

    if (root->left != NULL) {
        calculateTreePositions(root->left, x - offset, y + 50, offset / 2); // Posição que a subárvore esquerda vai ficar
    }
    if (root->right != NULL) {
        calculateTreePositions(root->right, x + offset, y + 50, offset / 2); // Mesma coisa só que para a direita 
    }
}

// Função para calcular a posição final do novo nó na árvore
void calculateFinalPosition(Node* root, int data, float* finalX, float* finalY, float parentX, float parentY) {
    if (!root) return;
    if (data < root->data) {
        *finalX = parentX - 100; // Mexe para a esquerda (100 posições)
        *finalY = parentY + 50;  // Mexe para baixo (50 posições)
        calculateFinalPosition(root->left, data, finalX, finalY, *finalX, *finalY); // Recebe os "parametros" e calcula a posição final
    }
    else {
        *finalX = parentX + 100; // Mexe para a direita (100 posições)
        *finalY = parentY + 50;  // Mexe para baixo (50 posições)
        calculateFinalPosition(root->right, data, finalX, finalY, *finalX, *finalY); // Igual a função de cima
    }
}

void animate_insertion(Node *root, int data) {
    Node *current = root;
    while (current) {
        draw_node(current->x, current->y, current->data, true); // Faz o desenhho do nó
        al_flip_display();
        al_rest(0.3); // Quanto esperar para desenhar o outro
        draw_node(current->x, current->y, current->data, false); // Desenha o nó
        al_flip_display();
        al_rest(0.3);

        if (data < current->data) current = current->left; // Se o elemento for menor que a raíz, vai para à esquerda
        else current = current->right; // O mesmo, porém se for maior é direita 
    }
}

void draw_node(float x, float y, int data, bool highlight) {
    if (highlight) {
        al_draw_filled_circle(x, y, 20, al_map_rgb(0, 0, 255));
    } else {
        al_draw_filled_circle(x, y, 20, al_map_rgb(255, 255, 255));
    }

    char buffer[10];
    sprintf(buffer, "%d", data);
    al_draw_text(font, al_map_rgb(0, 0, 0), x, y, ALLEGRO_ALIGN_CENTER, buffer);
}

// Função para desenhar a árvore
void drawTree(Node* root, int highlightdata) {
    if (root == NULL) return;

    // Desenha o nó atual
    if (root->data == highlightdata) al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(0, 0, 255)); // Valor inserido fica destacado
    else if (root->animation.isAnimating) {
        al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(255, 255, 255)); // Destaca o nó em animação
    } else {
        al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(255, 0, 0)); // Nó normal
    }

    char buffer[10]; //Armazenar o valor do nó
    sprintf(buffer, "%d", root->data); // Desenha o valor do nó (convertendo para string)
    al_draw_text(font, al_map_rgb(0, 0, 0), root->x, root->y, ALLEGRO_ALIGN_CENTER, buffer); // Desenha o valor do nó

    int balanceFactor = balance(root); // Recebe a função que calcula o valor do balanceamento
    sprintf(buffer, "[%d]", balanceFactor); // Imprime o valor do balanceamento (convertendo para string)
    al_draw_text(font, al_map_rgb(255, 255, 255), root->x + 30, root->y, ALLEGRO_ALIGN_LEFT, buffer); // Desenha o valor do balanceamento

    if (root->left) { // Conexões com os filhos 
        float dx = root->left->x - root->x; // Calcula a diferença entre as posições (raíz -> [elemento à esquerda] - raíz -> [raíz (elemento)])
        float dy = root->left->y - root->y; // O mesmo para a função acima, porém para o eixo das ordenadas (vertical) 
        float distance = sqrt(dx * dx + dy * dy); // Distância entre dois ponto 
        float offsetX = (dx / distance) * 30; // Ajuste para a borda do círculo (30 posições [pixeis] entre as bordas)
        float offsetY = (dy / distance) * 30; // Ajuste para a borda do círculo 

        al_draw_line(root->x + offsetX, root->y + offsetY, root->left->x - offsetX, root->left->y - offsetY, al_map_rgb(255, 255, 255), 2); // Desenho das linhas entre pai e filho (2, espessura da linha)
        drawTree(root->left, highlightdata); // Faz o desenho da árvore recursivamente para à esquerda com o valor destacado (último elemento inserido)
    }
    if (root->right) { // Mesma função da anterior, agora para à direita
        float dx = root->right->x - root->x;
        float dy = root->right->y - root->y;
        float distance = sqrt(dx * dx + dy * dy);
        float offsetX = (dx / distance) * 30;
        float offsetY = (dy / distance) * 30;

        al_draw_line(root->x + offsetX, root->y + offsetY, root->right->x - offsetX, root->right->y - offsetY, al_map_rgb(0, 0, 255), 2);
        drawTree(root->right, highlightdata);
    }
}

void drawNewNode() { // Desenha os nós
    if (newNodedata != -1) { 
        float x = newNodeAnimation.startX + (newNodeAnimation.endX - newNodeAnimation.startX) * newNodeAnimation.progress; // Cálculo da posição final do nó
        float y = newNodeAnimation.startY + (newNodeAnimation.endY - newNodeAnimation.startY) * newNodeAnimation.progress; // O mesmo, porém no eixo das ordenadas 

        al_draw_filled_circle(x, y, 20, al_map_rgb(255, 255, 255)); // Circulo do novo nó
        if (font) {
            char buffer[10];
            sprintf(buffer, "%d", newNodedata); // Elemento do nó que vai dentro do circulo
            al_draw_text(font, al_map_rgb(0, 0, 0), x, y - 10, ALLEGRO_ALIGN_CENTER, buffer); // Desenha o valor do nó
        }
        else {
            fprintf(stderr, "Fonte não carregada corretamente.\n");
        }
    }
}

void highlightNode(Node* root) {
    if (root != NULL) {
        al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(255, 255, 0)); // Amarelo para nós destacados
        char value_str[10];
        sprintf(value_str, "%d", root->data);
        al_draw_text(font, al_map_rgb(0, 0, 0), root->x, root->y - 10, ALLEGRO_ALIGN_CENTER, value_str); // Use a variável global 'font'
    }
}

// ESSA FUNÇÃO PODE SER MODIFICADA PARA FAZER O DESTAQUE DOS NÓS??  (Sim, pesquisar como depois)
void updateAnimation(Animation* anim, float delta_time) { // Atualiza a animação
    if (anim->isAnimating) {
        anim->progress += delta_time * 4; // Atualiza o progresso da animação com base no delta_time
        if (anim->progress >= 1.0f) { 
            anim->progress = 1.0f; // Garante que o progresso não exceda 1.0 
            anim->isAnimating = false; // Marca a animação como concluída
        }
    }
}
 
void handleInput(ALLEGRO_EVENT event) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode >= ALLEGRO_KEY_0 && event.keyboard.keycode <= ALLEGRO_KEY_9) {
            newNodedata = event.keyboard.keycode - ALLEGRO_KEY_0; 
            float finalX, finalY;
            calculateFinalPosition(avlTree, newNodedata, &finalX, &finalY, SCREEN_WIDTH / 2, 50); // Posição da ráiz

            // Inicia a animação
            newNodeAnimation.startX = 50; // Posição horizontal inicial (canto superior esquerdo)
            newNodeAnimation.startY = 50; // Posição vertical inicial
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
            al_acknowledge_resize(event.display.source); // Redimensiona a janela
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            blinkTimer += delta_time; // Atualiza o timer do efeito de piscar
            updateAnimation(&newNodeAnimation, delta_time); // Atualiza a animação

            // Quando a animação terminar, insere o novo nó na árvore
            if (!newNodeAnimation.isAnimating && newNodedata != -1) {
                avlTree = insert(avlTree, newNodedata, newNodeAnimation.endX, newNodeAnimation.endY); // Insere o novo nó na árvore
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
