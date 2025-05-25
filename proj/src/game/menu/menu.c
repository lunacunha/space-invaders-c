#include "menu.h"

static MenuState current_state = MENU_MAIN;
static MenuOption current_option = MENU_OPTION_START;
static bool option_selected = false;
extern uint8_t scancode;
extern vbe_mode_info_t mode_info;

void menu_init() {
    current_state = MENU_MAIN;
    current_option = MENU_OPTION_START;
    option_selected = false;
}

void menu_handle_input() {
    if (current_state == MENU_MAIN) {
        switch (scancode) {
            case KB_UP_ARROW:
                if (current_option > MENU_OPTION_START) {
                    current_option--;
                }
                break;
            case KB_DOWN_ARROW:
                if (current_option < MENU_OPTION_QUIT) {
                    current_option++;
                }
                break;
            case KB_ENTER:
                option_selected = true;
                switch (current_option) {
                    case MENU_OPTION_START:
                        current_state = MENU_START;
                        break;
                    case MENU_OPTION_SCORES:
                        current_state = MENU_SCORES;
                        break;
                    case MENU_OPTION_QUIT:
                        current_state = MENU_QUIT;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    } else if (scancode == KB_BREAK_ESC) {
        // Volta ao menu principal
        current_state = MENU_MAIN;
        option_selected = false;
    }
}

void menu_render() {
    switch (current_state) {
        case MENU_MAIN:
            // Desenha o título
            print_xpm(title, 0, 0);
            
            // Desenha as opções do menu com destaque para a selecionada
            switch (current_option) {
                case MENU_OPTION_START:
                    print_xpm(start1, 100, 300);  // Opção selecionada
                    print_xpm(scores2, 100, 350);  // Opção não selecionada
                    print_xpm(quit2, 100, 400);    // Opção não selecionada
                    break;
                case MENU_OPTION_SCORES:
                    print_xpm(start2, 100, 300);   // Opção não selecionada
                    print_xpm(scores1, 100, 350);  // Opção selecionada
                    print_xpm(quit2, 100, 400);    // Opção não selecionada
                    break;
                case MENU_OPTION_QUIT:
                    print_xpm(start2, 100, 300);   // Opção não selecionada
                    print_xpm(scores2, 100, 350);  // Opção não selecionada
                    print_xpm(quit1, 100, 400);    // Opção selecionada
                    break;
                default:
                    break;
            }
            break;
            
        case MENU_START:
            break;
            
        case MENU_SCORES:
            // Tela de pontuações
            print_xpm(score_board, 0, 0);
            break;
            
        case MENU_QUIT:
            current_state = MENU_EXIT;
            break;
            
        default:
            break;
    }
}

MenuState menu_get_state() {
    return current_state;
}

