#define _CRT_SECURE_NO_WARNINGS 
#include "avl-tree.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>


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

void drawTextOnScreen(const char* text, int y, ALLEGRO_FONT* font) {
    al_draw_text(font, al_map_rgb(255, 255, 255), 10, y, ALLEGRO_ALIGN_LEFT, text);
}

int main() {
    // Inicializa o Allegro e seus addons
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

    Animation animation = {
    .startX = 400,
    .startY = 300,
    .endX = 0,
    .endY = 0,
    .progress = 0,
    .isAnimating = false
    };
    
    // Update animation call
    updateAnimation(&animation, 1.0f / FPS);

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

    initFont(); // Chama a função para inicializar a fonte

    Node* root = NULL;
    int running = 1;
    int highlightValue = -1;
    int value = 0; // Inicializa value para evitar comportamento indefinido
    


    al_start_timer(timer);

    char traversalText[1024] = ""; // Buffer para armazenar o texto da travessia

    while (running) {
        displayMenu();

        int option;
        scanf("%d", &option);

        switch (option) {
        case 1:
            printf("Digite o valor para inserir: ");
            scanf("%d", &value);
            highlightValue = value; // Define o valor a ser destacado
            root = insert(root, value, SCREEN_WIDTH / 2, 50);
            calculateTreePositions(root, SCREEN_WIDTH / 2, 50, SCREEN_WIDTH / 4); // Atualiza posições
            break;

        case 2:
            printf("Digite o valor para remover: ");
            scanf("%d", &value);
            root = removeNode(root, value);
            printf("Valor %d removido (se existia na árvore).\n", value);
            calculateTreePositions(root, SCREEN_WIDTH / 2, 50, SCREEN_WIDTH / 4); // Atualiza posições
            break;

        case 3:{
            printf("Digite o valor para buscar: ");
            scanf("%d", &value);

            char searchResult[100]; // Buffer para armazenar o resultado da busca
            if (search(root, value, searchResult)) {
                printf("%s\n", searchResult); // Exibe no terminal
            }
            else {
                printf("%s\n", searchResult); // Exibe no terminal
            }

            // Exibe o resultado na tela
            ALLEGRO_FONT* font = al_create_builtin_font();
            if (font) {
                drawTextOnScreen(searchResult, 50, font); // Exibe na tela
            }
            break;
        }
        case 4:
            printf("Exibindo árvore em ordem:\n");
            traversalText[0] = '\0'; // Limpa o buffer
            inorder(root, traversalText); // Passa o buffer para a função inorder
            printf("%s\n", traversalText);
            break;

        case 5:
            printf("Exibindo árvore em pré-ordem:\n");
            traversalText[0] = '\0'; // Limpa o buffer
            preorder(root, traversalText); // Passa o buffer para a função preorder
            printf("%s\n", traversalText);
            break;

        case 6:
            printf("Exibindo árvore em pós-ordem:\n");
            traversalText[0] = '\0'; // Limpa o buffer
            postorder(root, traversalText); // Passa o buffer para a função postorder
            printf("%s\n", traversalText);
            break;

        case 7:
            running = 0;
            break;

        default:
            printf("Opção inválida!\n");
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
                updateAnimation(&animation, 1.0 / FPS); // Atualiza a animação com deltaTime
            }

            if (redraw && al_is_event_queue_empty(queue)) {
                redraw = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));

                ALLEGRO_FONT* font = al_create_builtin_font(); // Usa a fonte padrão do Allegro

                // Desenha o valor digitado no canto esquerdo
                if (font) {
                    char buffer[50];
                    sprintf(buffer, "Valor digitado: %d", value); // Ajustado para mostrar apenas o valor digitado
                    al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, ALLEGRO_ALIGN_LEFT, buffer);

                    // Desenha o texto da travessia na tela
                    drawTextOnScreen(traversalText, 30, font);
                }

                // Desenha a árvore
                drawTree(root, highlightValue); // Passa o segundo argumento (highlightValue)

                al_flip_display();
            }
        }
    }

    // Libera os recursos
    destroyFont(); // Chama a função para liberar a fonte
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}
