#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "./scripts/grid_rendering.c"
#include "./scripts/player_related.c"
#include <SDL2/SDL.h>
#include "./scripts/level_loader.c"
#include "./scripts/player_input.c"
#include "./scripts/maze_maker.c"

// Window & Rendering stuff
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *screen_surface;

// Images
// Char '#'
SDL_Surface *img_wall;
SDL_Texture *image_wall;

// Char ' '
SDL_Surface *img_grass;
SDL_Texture *image_grass;

// Char 'C'
SDL_Surface *img_crate;
SDL_Texture *image_crate;

////Char None (out of bounds)
SDL_Surface *img_outer;
SDL_Texture *image_outer;

// Char '@' direction = 2
SDL_Surface *img_player_left;
SDL_Texture *image_player_left;

// Char '@' direction = 3
SDL_Surface *img_player_right;
SDL_Texture *image_player_right;

// Char '@' direction = 1
SDL_Surface *img_player_up;
SDL_Texture *image_player_up;

// Char '@' direction = 0
SDL_Surface *img_player_down;
SDL_Texture *image_player_down;

// Menu related
SDL_Surface *img_menu;
SDL_Texture *image_menu;

SDL_Surface *img_play;
SDL_Texture *image_play;

SDL_Surface *img_quit;
SDL_Texture *image_quit;

SDL_Surface *img_play_s;
SDL_Texture *image_play_s;

SDL_Surface *img_quit_s;
SDL_Texture *image_quit_s;

SDL_Surface *img_settings;
SDL_Texture *image_settings;

SDL_Surface *img_settings_s;
SDL_Texture *image_settings_s;

SDL_Surface *img_menu_cursor;
SDL_Texture *image_menu_cursor;

SDL_Surface *img_menu_background;
SDL_Texture *image_menu_background;

// Textures
SDL_Surface *img_wall_single;
SDL_Texture *image_wall_single;

SDL_Surface *img_wall_single_open_all;
SDL_Texture *image_wall_single_open_all;

SDL_Surface *img_wall_single_open_left;
SDL_Texture *image_wall_single_open_left;

SDL_Surface *img_wall_single_open_right;
SDL_Texture *image_wall_single_open_right;

SDL_Surface *img_wall_single_open_left_and_right;
SDL_Texture *image_wall_single_open_left_and_right;

SDL_Surface *img_wall_single_open_up;
SDL_Texture *image_wall_single_open_up;

SDL_Surface *img_wall_single_open_down;
SDL_Texture *image_wall_single_open_down;

SDL_Surface *img_wall_single_open_up_and_down;
SDL_Texture *image_wall_single_open_up_and_down;

SDL_Surface *img_wall_single_open_all_but_top;
SDL_Texture *image_wall_single_open_all_but_top;

SDL_Surface *img_wall_single_open_all_but_right;
SDL_Texture *image_wall_single_open_all_but_right;

SDL_Surface *img_wall_single_open_all_but_down;
SDL_Texture *image_wall_single_open_all_but_down;

SDL_Surface *img_wall_single_open_all_but_left;
SDL_Texture *image_wall_single_open_all_but_left;

SDL_Surface *img_wall_single_corner_bl;
SDL_Texture *image_wall_single_corner_bl;

SDL_Surface *img_wall_single_corner_br;
SDL_Texture *image_wall_single_corner_br;

SDL_Surface *img_wall_single_corner_tl;
SDL_Texture *image_wall_single_corner_tl;

SDL_Surface *img_wall_single_corner_tr;
SDL_Texture *image_wall_single_corner_tr;

SDL_Surface *img_door;
SDL_Texture *image_door;

SDL_Surface *img_key;
SDL_Texture *image_key;

SDL_Surface *img_no_key;
SDL_Texture *image_no_key;

SDL_Surface *img_has_key;
SDL_Texture *image_has_key;

SDL_Surface *img_exit;
SDL_Texture *image_exit;

SDL_Surface *img_border_bl;
SDL_Texture *image_border_bl;
SDL_Surface *img_border_br;
SDL_Texture *image_border_br;

SDL_Surface *img_border_tl;
SDL_Texture *image_border_tl;

SDL_Surface *img_border_tr;
SDL_Texture *image_border_tr;

SDL_Surface *img_border_lr;
SDL_Texture *image_border_lr;

SDL_Surface *img_border_tb;
SDL_Texture *image_border_tb;

SDL_Surface *img_fog;
SDL_Texture *image_fog;

SDL_Surface *img_heart;
SDL_Texture *image_heart;

// Allows to set how many cells of the grid will be seen in game.
int fov_x = 10;
int fov_y = 10;

// Settings related.
int width = 960;
int height = 960;


int size_x = 5;
int size_y = 5;

// Allows to track the player position and to make it move.
int player_pos_x;
int player_pos_y;

bool has_key;

// Used to put a different image in function of where the entity last moved.
// 0 : down, 1 : up, 2 : left, 3 : right
int direction = 0;

bool won = false;

void play() {
    won = false;
    player_pos_x = 1;
    player_pos_y = 1;

    char grid[(size_y * 2 + 3) * (size_x * 2 + 3)];

    GenMaze(size_x, size_y, grid);
    
    size_x = size_x * 2 + 3;
    size_y = size_y * 2 + 3;

    SpawnPlayer(size_x, size_y, &grid);
    render_map_around_player(renderer, size_x, size_y, width, height, &grid);

    SDL_Delay(1000);

    
    while (Read_Input(renderer, size_x, size_y, width, height, &grid) != 5)
    {
        if(won) {
            break;
        }
    }

    size_x = (size_x - 3) / 2;
    size_y = (size_y - 3) / 2;

    free(grid);
}

// Pre-game menu
int menu()
{
    int selected_button = 0;
    SDL_Event event;

    // Mother loop
    while (1)
    {
        // Reads input
        while (SDL_PollEvent(&event))
        {
            SDL_RenderClear(renderer);

            SDL_Rect rect_bg = {0, 0, width, height};
            SDL_RenderCopy(renderer, image_menu_background, NULL, &rect_bg);

            // Drawing the "Menu" image.
            SDL_Rect rect = {width * 0.20, height / 10, width * 0.60, height * 0.20};
            SDL_RenderCopy(renderer, image_menu, NULL, &rect);

            SDL_Rect rect_cursor = {width * 0.20, height * (0.41 + (selected_button * 0.15)), width * 0.10, height * 0.08};
            SDL_RenderCopy(renderer, image_menu_cursor, NULL, &rect_cursor);

            SDL_Rect rect_play = {width * 0.30, height * 0.40, width * 0.40, height * 0.10};

            // Draws the "Play" button, depending on if it is the currently selected button.
            if (selected_button == 0)
            {
                SDL_RenderCopy(renderer, image_play_s, NULL, &rect_play);
            }
            else
            {
                SDL_RenderCopy(renderer, image_play, NULL, &rect_play);
            }

            SDL_RenderDrawRect(renderer, &rect_play);

            // Draws the "Settings" button, depending on if it is the currently selected button.
            SDL_Rect rect_settings = {width * 0.30, height * 0.55, width * 0.40, height * 0.10};
            if (selected_button == 1)
            {
                SDL_RenderCopy(renderer, image_settings_s, NULL, &rect_settings);
            }
            else
            {
                SDL_RenderCopy(renderer, image_settings, NULL, &rect_settings);
            }

            // Draws the "Quit" button, depending on if it is the currently selected button.
            SDL_Rect rect_quit = {width * 0.30, height * 0.70, width * 0.40, height * 0.10};
            if (selected_button == 2)
            {
                SDL_RenderCopy(renderer, image_quit_s, NULL, &rect_quit);
            }
            else
            {
                SDL_RenderCopy(renderer, image_quit, NULL, &rect_quit);
            }

            // Renders everything listed above.
            SDL_RenderPresent(renderer);

            // Allows to change the currently selected button.
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                // Allows to confirm a choice by "pressing" a button.
                case SDLK_RETURN:
                    if (selected_button == 0)
                    {

                        play();
                        continue;//return 0;
                    }
                    if (selected_button == 2)
                    {
                        return 1;
                    }
                    break;

                // Allows to select a button using the "UP", "DOWN", "Z" and "S" keyboard keys.
                case SDLK_z:
                case SDLK_UP:
                    selected_button--;
                    if (selected_button == -1)
                    {
                        selected_button = 2;
                    }
                    if (selected_button == 3)
                    {
                        selected_button = 0;
                    }
                    break;

                case SDLK_s:
                case SDLK_DOWN:
                    selected_button++;
                    if (selected_button == -1)
                    {
                        selected_button = 2;
                    }
                    if (selected_button == 3)
                    {
                        selected_button = 0;
                    }
                    break;
                }
                break;

            // Allows to quit the game.
            case SDL_QUIT:
                goto Quit;
                break;

            default:
                break;
            }
        }
    }
// Allows to quit properly
Quit:
    return 1;
}

// ALlows to load images when the game starts.
void init_images()
{

    img_wall = SDL_LoadBMP("./img/wall_simple.bmp");
    image_wall = SDL_CreateTextureFromSurface(renderer, img_wall);

    img_grass = SDL_LoadBMP("./img/grass_simple.bmp");
    image_grass = SDL_CreateTextureFromSurface(renderer, img_grass);

    img_crate = SDL_LoadBMP("./img/crate_simple_2.bmp");
    image_crate = SDL_CreateTextureFromSurface(renderer, img_crate);

    img_outer = SDL_LoadBMP("./img/outer_simple.bmp");
    image_outer = SDL_CreateTextureFromSurface(renderer, img_outer);

    img_player_left = SDL_LoadBMP("./img/player_left.bmp");
    image_player_left = SDL_CreateTextureFromSurface(renderer, img_player_left);

    img_player_right = SDL_LoadBMP("./img/player_right.bmp");
    image_player_right = SDL_CreateTextureFromSurface(renderer, img_player_right);

    img_player_up = SDL_LoadBMP("./img/player_back.bmp");
    image_player_up = SDL_CreateTextureFromSurface(renderer, img_player_up);

    img_player_down = SDL_LoadBMP("./img/player_front.bmp");
    image_player_down = SDL_CreateTextureFromSurface(renderer, img_player_down);

    img_menu = SDL_LoadBMP("./img/menu_simple.bmp");
    image_menu = SDL_CreateTextureFromSurface(renderer, img_menu);

    img_play = SDL_LoadBMP("./img/play_button.bmp");
    image_play = SDL_CreateTextureFromSurface(renderer, img_play);

    img_play_s = SDL_LoadBMP("./img/play_button_selected.bmp");
    image_play_s = SDL_CreateTextureFromSurface(renderer, img_play_s);

    img_quit = SDL_LoadBMP("./img/quit_button.bmp");
    image_quit = SDL_CreateTextureFromSurface(renderer, img_quit);

    img_quit_s = SDL_LoadBMP("./img/quit_button_selected.bmp");
    image_quit_s = SDL_CreateTextureFromSurface(renderer, img_quit_s);

    img_settings = SDL_LoadBMP("./img/settings_button.bmp");
    image_settings = SDL_CreateTextureFromSurface(renderer, img_settings);

    img_settings_s = SDL_LoadBMP("./img/settings_button_selected.bmp");
    image_settings_s = SDL_CreateTextureFromSurface(renderer, img_settings_s);

    img_menu_cursor = SDL_LoadBMP("./img/arrow.bmp");
    image_menu_cursor = SDL_CreateTextureFromSurface(renderer, img_menu_cursor);

    img_menu_background = SDL_LoadBMP("./img/menu_background.bmp");
    image_menu_background = SDL_CreateTextureFromSurface(renderer, img_menu_background);

    img_wall_single = SDL_LoadBMP("./img/wall_simple_single.bmp");
    image_wall_single = SDL_CreateTextureFromSurface(renderer, img_wall_single);

    img_wall_single_open_all = SDL_LoadBMP("./img/wall_simple_single_open_all.bmp");
    image_wall_single_open_all = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_all);

    img_wall_single_open_down = SDL_LoadBMP("./img/wall_simple_single_open_down.bmp");
    image_wall_single_open_down = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_down);

    img_wall_single_open_up = SDL_LoadBMP("./img/wall_simple_single_open_up.bmp");
    image_wall_single_open_up = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_up);

    img_wall_single_open_up_and_down = SDL_LoadBMP("./img/wall_simple_single_open_up_and_down.bmp");
    image_wall_single_open_up_and_down = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_up_and_down);

    img_wall_single_open_left = SDL_LoadBMP("./img/wall_simple_single_open_left.bmp");
    image_wall_single_open_left = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_left);

    img_wall_single_open_right = SDL_LoadBMP("./img/wall_simple_single_open_right.bmp");
    image_wall_single_open_right = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_right);

    img_wall_single_open_left_and_right = SDL_LoadBMP("./img/wall_simple_single_open_left_and_right.bmp");
    image_wall_single_open_left_and_right = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_left_and_right);

    img_wall_single_corner_bl = SDL_LoadBMP("./img/wall_simple_single_b_left.bmp");
    image_wall_single_corner_bl = SDL_CreateTextureFromSurface(renderer, img_wall_single_corner_bl);

    img_wall_single_corner_br = SDL_LoadBMP("./img/wall_simple_single_b_right.bmp");
    image_wall_single_corner_br = SDL_CreateTextureFromSurface(renderer, img_wall_single_corner_br);

    img_wall_single_corner_tl = SDL_LoadBMP("./img/wall_simple_single_t_left.bmp");
    image_wall_single_corner_tl = SDL_CreateTextureFromSurface(renderer, img_wall_single_corner_tl);

    img_wall_single_corner_tr = SDL_LoadBMP("./img/wall_simple_single_t_right.bmp");
    image_wall_single_corner_tr = SDL_CreateTextureFromSurface(renderer, img_wall_single_corner_tr);

    img_wall_single_open_all_but_down = SDL_LoadBMP("./img/wall_simple_single_open_all_but_bot.bmp");
    image_wall_single_open_all_but_down = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_all_but_down);

    img_wall_single_open_all_but_top = SDL_LoadBMP("./img/wall_simple_single_open_all_but_top.bmp");
    image_wall_single_open_all_but_top = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_all_but_top);

    img_wall_single_open_all_but_left = SDL_LoadBMP("./img/wall_simple_single_open_all_but_left.bmp");
    image_wall_single_open_all_but_left = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_all_but_left);

    img_wall_single_open_all_but_right = SDL_LoadBMP("./img/wall_simple_single_open_all_but_right.bmp");
    image_wall_single_open_all_but_right = SDL_CreateTextureFromSurface(renderer, img_wall_single_open_all_but_right);

    img_key = SDL_LoadBMP("./img/key.bmp");
    image_key = SDL_CreateTextureFromSurface(renderer, img_key);

    img_no_key = SDL_LoadBMP("./img/zero.bmp");
    image_no_key = SDL_CreateTextureFromSurface(renderer, img_no_key);

    img_has_key = SDL_LoadBMP("./img/one.bmp");
    image_has_key = SDL_CreateTextureFromSurface(renderer, img_has_key);

    img_door = SDL_LoadBMP("./img/door.bmp");
    image_door = SDL_CreateTextureFromSurface(renderer, img_door);

    img_exit = SDL_LoadBMP("./img/exit.bmp");
    image_exit = SDL_CreateTextureFromSurface(renderer, img_exit);

    img_border_tb = SDL_LoadBMP("./img/border_tb.bmp");
    image_border_tb = SDL_CreateTextureFromSurface(renderer, img_border_tb);

    img_border_lr = SDL_LoadBMP("./img/border_lr.bmp");
    image_border_lr = SDL_CreateTextureFromSurface(renderer, img_border_lr);

    img_border_bl = SDL_LoadBMP("./img/border_bl.bmp");
    image_border_bl = SDL_CreateTextureFromSurface(renderer, img_border_bl);

    img_border_br = SDL_LoadBMP("./img/border_br.bmp");
    image_border_br = SDL_CreateTextureFromSurface(renderer, img_border_br);

    img_border_tl= SDL_LoadBMP("./img/border_tl.bmp");
    image_border_tl = SDL_CreateTextureFromSurface(renderer, img_border_tl);

    img_border_tr= SDL_LoadBMP("./img/border_tr.bmp");
    image_border_tr = SDL_CreateTextureFromSurface(renderer, img_border_tr);

    img_fog= SDL_LoadBMP("./img/fog.bmp");
    image_fog = SDL_CreateTextureFromSurface(renderer, img_fog);

    img_heart= SDL_LoadBMP("./img/heart.bmp");
    image_heart = SDL_CreateTextureFromSurface(renderer, img_heart);
}

void init_handlers()
{
    window = SDL_CreateWindow("LONE GHOST", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    screen_surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
}

// Main
int main()
{
    /*char grid[20][20];
    open_file("./levels/test_land", 20, 20, grid);
    bool in_menu = true;
    bool quit = false;
    */
    int statut = EXIT_FAILURE;

    if (0 != SDL_Init(SDL_INIT_EVERYTHING))
        return EXIT_FAILURE;

    init_handlers();
    init_images();

    // If it returns 1, we quit the game.
    if (menu())
    {
        printf("%c", '\n');
        exit(EXIT_SUCCESS);
    }


    if (NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    if (NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    if (0 != SDL_RenderClear(renderer))
    {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;

    // Saut à la ligne une fois le programme fini.
    printf("%c", '\n');
    exit(EXIT_SUCCESS);
}