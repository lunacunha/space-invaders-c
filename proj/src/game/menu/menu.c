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
    // Clear the back buffer first (optional - choose appropriate background color)
    clear_back_buf(0x000000); // Black background
    
    // Get screen dimensions from mode_info
    uint16_t screen_width = mode_info.XResolution;
    uint16_t screen_height = mode_info.YResolution;
    
    // Define image dimensions (you'll need to adjust these based on your actual XPM sizes)
    uint16_t title_width = 400;   // Adjust to your title XPM width
    uint16_t title_height = 200;  // Adjust to your title XPM height
    uint16_t option_width = 145;  // Based on your XPM comment "145 51"
    uint16_t option_height = 51;  // Based on your XPM comment "145 51"
    
    switch (current_state) {
        case MENU_MAIN: {
            // Center the title horizontally, position it in upper portion of screen
            uint16_t title_x = (screen_width - title_width) / 2;
            uint16_t title_y = screen_height / 4;  // Position at 1/4 from top
            
            print_xpm(title, title_x, title_y);
            
            // Center the menu options horizontally, position them below title
            uint16_t options_x = (screen_width - option_width) / 2;
            uint16_t options_start_y = title_y + title_height + 50;  // 50px gap after title
            uint16_t option_spacing = option_height + 20;  // 20px spacing between options
            
            switch (current_option) {
                case MENU_OPTION_START:
                    print_xpm(start1, options_x, options_start_y);  
                    print_xpm(scores2, options_x, options_start_y + option_spacing);  
                    print_xpm(quit2, options_x, options_start_y + (option_spacing * 2));    
                    break;
                case MENU_OPTION_SCORES:
                    print_xpm(start2, options_x, options_start_y);   
                    print_xpm(scores1, options_x, options_start_y + option_spacing);  
                    print_xpm(quit2, options_x, options_start_y + (option_spacing * 2));   
                    break;
                case MENU_OPTION_QUIT:
                    print_xpm(start2, options_x, options_start_y);   
                    print_xpm(scores2, options_x, options_start_y + option_spacing);  
                    print_xpm(quit1, options_x, options_start_y + (option_spacing * 2));    
                    break;
                default:
                    break;
            }
            break;
        }
            
        case MENU_START:
            break;
            
        case MENU_SCORES: {
            // Center the score board
            /*uint16_t score_width = 195;   // Based on your XPM comment "195 51"
            uint16_t score_height = 51;   // Based on your XPM comment "195 51"
            uint16_t score_x = (screen_width - score_width) / 2;
            uint16_t score_y = (screen_height - score_height) / 2;
            
            print_xpm(score_board, score_x, score_y);*/
            break;
        }
            
        case MENU_QUIT:
            current_state = MENU_EXIT;
            break;
            
        default:
            break;
    }
    
    // CRITICAL: Swap buffers to display the rendered menu
    swap_buffers();
}

MenuState menu_get_state() {
    return current_state;
}

void menu_set_state(MenuState state) {
    current_state = state;
    option_selected = false;
}
