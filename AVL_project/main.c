#define _CRT_SECURE_NO_WARNINGS 
#include "avl-tree.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

void displayMenu() {
    printf("\nMenu:\n");
    printf("1. Inserir um valor\n");
    printf("2. Remover um valor\n");
    printf("3. Buscar um valor\n");
    printf("4. Exibir arvore em ordem\n");
    printf("5. Exibir arvore em pre-ordem\n");
    printf("6. Exibir arvore em pos-ordem\n");
    printf("7. Sair\n");
    printf("Escolha uma opcao: ");
}

void drawTextOnScreen(const char* text, int y, ALLEGRO_FONT* font, int previewValue, int tipo) {
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, y, ALLEGRO_ALIGN_LEFT, text);
    
    if (previewValue != -1) {
        float previewX = 50;
        float previewY = 50;
        
        al_draw_filled_circle(previewX, previewY, 20, al_map_rgb(255, 255, 255)); 
        
        char buffer[10];
        sprintf(buffer, "%d", previewValue);
        al_draw_text(font, al_map_rgb(0, 0, 0), previewX, previewY - 10, ALLEGRO_ALIGN_CENTER, buffer); // Desenha o valor do nó
    
        if(tipo == 0){
            al_draw_text(font, al_map_rgb(255, 255, 255), previewX + 40, previewY - 20, // Desenhar o texto (+40 espaçamento no horizontal e -20 no vertical) 
                    ALLEGRO_ALIGN_LEFT, "Valor a ser inserido");
        }else if (tipo == 1){
            al_draw_text(font, al_map_rgb(255, 255, 255), previewX + 40, previewY - 20, // Mesma coisa 
                    ALLEGRO_ALIGN_LEFT, "Valor a ser removido");       
    }else if (tipo == 2){
            al_draw_text(font, al_map_rgb(255, 255, 255), previewX + 40, previewY - 20, // The same thing 
                    ALLEGRO_ALIGN_LEFT, "Valor a ser buscado");       
    }
    }
}

void blinkNode(Node* node, ALLEGRO_FONT* font) {
    if (node) {
        for (int i = 0; i < 2; i++) { // Pisca o negócio 2 vezes
            al_draw_filled_circle(node->x, node->y, 20, al_map_rgb(255, 255, 0)); 
            char buffer[10];
            sprintf(buffer, "%d", node->data);
            al_draw_text(font, al_map_rgb(0, 0, 0), node->x, node->y - 10, ALLEGRO_ALIGN_CENTER, buffer);
            al_flip_display();
            al_rest(0.3);

            al_draw_filled_circle(node->x, node->y, 20, al_map_rgb(255, 255, 255)); 
            al_draw_text(font, al_map_rgb(0, 0, 0), node->x, node->y - 10, ALLEGRO_ALIGN_CENTER, buffer);
            al_flip_display();
            al_rest(0.3);
        }
    }
}

void inorderWithBlink(Node* root, char* buffer, ALLEGRO_FONT* font) {
    if (!root) return;
    inorderWithBlink(root->left, buffer, font); // Mesma coisa da função de baixo
    char temp[20];
    sprintf(temp, "%d ", root->data);
    strcat(buffer, temp);
    blinkNode(root, font);
    inorderWithBlink(root->right, buffer, font);
}

void preorderWithBlink(Node* root, char* buffer, ALLEGRO_FONT* font) {
    if (!root) return;
    char temp[20];
    sprintf(temp, "%d ", root->data); // Mesma coisa da função de baixo
    strcat(buffer, temp);
    blinkNode(root, font);
    preorderWithBlink(root->left, buffer, font);
    preorderWithBlink(root->right, buffer, font);
}

void postorderWithBlink(Node* root, char* buffer, ALLEGRO_FONT* font) {
    if (!root) return;
    postorderWithBlink(root->left, buffer, font); // Percorre a subárvore esquerda
    postorderWithBlink(root->right, buffer, font); // Mesma coisa para a direita
    char temp[20];
    sprintf(temp, "%d ", root->data); 
    strcat(buffer, temp); // Adiciona o valor ao buffer (buffer = local de armazenamento temporário)
    blinkNode(root, font); // Pisca o nó
}

bool searchWithBlink(Node* root, int value, char* resultBuffer, ALLEGRO_FONT* font) {
    if (!root) {
        sprintf(resultBuffer, "Valor %d nao encontrado na arvore.", value);
        return false;
    }

    if (root->data == value) {
        for (int i = 0; i < 2; i++) { // Piscar o negócio duas vezes
            al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(255, 255, 0));
            char buffer[10];
            sprintf(buffer, "%d", root->data);
            al_draw_text(font, al_map_rgb(0, 0, 0), root->x, root->y - 10, ALLEGRO_ALIGN_CENTER, buffer); // O valor do nó será desenhado (-10 o nó fica mais pra cima)
            al_flip_display();
            al_rest(0.3);

            al_draw_filled_circle(root->x, root->y, 20, al_map_rgb(255, 255, 255)); 
            al_draw_text(font, al_map_rgb(0, 0, 0), root->x, root->y - 10, ALLEGRO_ALIGN_CENTER, buffer); // Mesma coisa da função de cima (após parar de piscar o elemento dentro do nó volta para a posição central)
            al_flip_display();
            al_rest(0.3);
        }
        sprintf(resultBuffer, "Valor %d encontrado na arvore.", value);
        return true;
    }

    return (value < root->data) ? searchWithBlink(root->left, value, resultBuffer, font) : searchWithBlink(root->right, value, resultBuffer, font); // Se o valor for menor que o ponteiro da raíz está apontando imprime o lado esquerdo, caso contrário imprime o direito
}

int main() {
    if (!al_init()) {
        fprintf(stderr, "Erro ao inicializar o Allegro.\n");
        return -1;
    }

    if (!al_install_system(ALLEGRO_VERSION_INT, NULL)) {
        fprintf(stderr, "Erro ao inicializar o Allegro.\n");
        return -1;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Erro ao inicializar o addon de primitivas.\n");
        return -1;
    }

    if (!al_init_font_addon() || !al_init_ttf_addon()) {
        fprintf(stderr, "Erro ao inicializar o addon de fontes.\n");
        return -1;
    }

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const float FPS = 60.0;

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Erro ao criar a janela.\n");
        return -1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "Erro ao criar o timer.\n");
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Erro ao criar a fila de eventos.\n");
        al_destroy_timer(timer);
        al_destroy_display(display);
        return -1;
    }

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_install_keyboard();
    al_register_event_source(queue, al_get_keyboard_event_source());

    initFont(); // Chama a funcao para inicializar a fonte

    Node* root = NULL;
    int running = 1;
    int highlightValue = -1; // Inicializa o valor a ser destacado
    int previewValue = -1; // Inicializa o valor a ser exibido como preview (bolinha na esquerda)
    int value = 0; // Inicializa value para evitar comportamento indefinido
    int type = -1;
    ALLEGRO_FONT* font = al_create_builtin_font();


    al_start_timer(timer);

    char traversalText[1024] = ""; // Buffer para armazenar o texto da travessia (buffer, local onde armazena temporariamente)

    while (running) {
        displayMenu();

        int option;
        scanf("%d", &option);

        switch (option) {
        case 1:

            printf("Digite o valor para inserir: ");
            scanf("%d", &value);
            previewValue = value; // Atualiza o preview
                
            // Mostra o preview por 1 segundo
            float preview_time = 1.0;
            float elapsed = 0.0;
            while (elapsed < preview_time) {
                // Renderiza a tela com o preview
                al_clear_to_color(al_map_rgb(0, 0, 0));
                drawTextOnScreen("", 30, font, previewValue, 0); // Desenha o preview
                type = 0;
                drawTree(root, highlightValue); // Continua mostrando a árvore
                al_flip_display();
                    
                al_rest(0.016); // ~60 FPS
                elapsed += 0.016;
            }
                
            // Insere o valor e atualiza a árvore
            highlightValue = value;
            root = insert(root, value, SCREEN_WIDTH / 2, 100); // Ajuste a posição Y para 100
            calculateTreePositions(root, SCREEN_WIDTH / 2, 100, SCREEN_WIDTH / 8); // Ajuste a posição Y para 100 e diminua o offset
            previewValue = -1; // Limpa o preview
            break;

        case 2:
            printf("Digite o valor para remover: ");
            scanf("%d", &value);
        
            previewValue = value;
            
            // Mostra o preview por 1 segundo
            preview_time = 1.0;
            elapsed = 0.0;

            while (elapsed < preview_time) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                drawTextOnScreen("", 80, font, previewValue, 1);
                type = 1;
                drawTree(root, value); // Destaca o nó na árvore
                al_flip_display();
                
                al_rest(0.016);
                elapsed += 0.016;
            }
            
            // Remove o nó e atualiza a árvore
            root = removeNode(root, value);
            printf("Valor %d removido.\n", value);
            calculateTreePositions(root, SCREEN_WIDTH / 2, 100, SCREEN_WIDTH / 8); // Ajuste a posição Y para 100 e diminua o offset
            previewValue = -1; // Limpa o preview
            break;

        case 3: {
            printf("Digite o valor para buscar: ");
            scanf("%d", &value);

            char searchResult[100];
            if (searchWithBlink(root, value, searchResult, font)) {
                printf("%s\n", searchResult);
                highlightValue = value;
            } else {
                printf("%s\n", searchResult);
                highlightValue = -1;
            }

            if (font) {
                // Mostra resultado por 2 segundos
                preview_time = 2.0;
                elapsed = 0.0;
                while (elapsed < preview_time) {
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    drawTextOnScreen(searchResult, 80, font, highlightValue, 2);
                    type = 2;
                    drawTree(root, highlightValue);
                    al_flip_display();
                    
                    al_rest(0.016);
                    elapsed += 0.016;
                }
            }
            highlightValue = -1;

            break;
        }
        case 4:
            printf("Exibindo arvore em ordem:\n");
            traversalText[0] = '\0';
            inorderWithBlink(root, traversalText, font);
            printf("%s\n", traversalText);
            break;

        case 5:
            printf("Exibindo arvore em pre-ordem:\n");
            traversalText[0] = '\0';
            preorderWithBlink(root, traversalText, font);
            printf("%s\n", traversalText);
            break;

        case 6:
            printf("Exibindo arvore em pos-ordem:\n");
            traversalText[0] = '\0';
            postorderWithBlink(root, traversalText, font);
            printf("%s\n", traversalText);
            break;

        case 7:
            running = 0;
            break;

        default:
            printf("Opcao invalida!\n");
        }

        bool redraw = true;
        while (redraw) {
            ALLEGRO_EVENT event;
            al_wait_for_event(queue, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = 0;
                break;
            }
            else if (event.type == ALLEGRO_EVENT_TIMER) {
                redraw = true;
            }

           if (redraw && al_is_event_queue_empty(queue)) {
                redraw = false;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                ALLEGRO_FONT* font = al_create_builtin_font();
                
                // Desenha o preview e outros textos
                drawTextOnScreen(traversalText, 30, font, previewValue,type);
                type = -1;
                
                // Desenha a árvore
                drawTree(root, highlightValue);
                
                al_flip_display();
            }
        }
    }

    // Libera os recursos
    destroyFont(); // Chama a funcao para liberar a fonte
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}