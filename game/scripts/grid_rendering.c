#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

/*

There might be a lot of comments here. They are made to keep track of what has been made
and to make it easier to explain to my teammates.

[07/03/22] Project began.
[07/03/22] Made empty grid, player spawning, player moving functions.
[07/03/22] Player can now push rocks, or crates, or whatever it will be ! Have to fix the fact
that two rocks can overlap.
[07/03/22] Reworked the render system.
[08/03/22] Made the camera follow system, deleted the old static one.
[08/03/22] Added the textures system.
[08/03/22] Player sprite now changes depending of his last movements direction.
[08/03/22] Moved player spawning and movement in a dedicated file.
[09/03/22] Changed some tiles so it looks better.
[09/03/22] Added comments everywhere.
[09/03/22] Wall tiles now connect with eachothers making the game prettier.
[09/03/22] Updated player model.
[10/03/22] Made comments in the grid rendering file.
[15/03/22] Fixed the rendering bug.

*/

extern int fov_x, fov_y;
extern int player_pos_x, player_pos_y;
extern int direction;

extern SDL_Window *window;

// Reaching textures.
extern SDL_Texture *image_wall;
extern SDL_Texture *image_grass;
extern SDL_Texture *image_crate;
extern SDL_Texture *image_outer;
extern SDL_Texture *image_player_down;
extern SDL_Texture *image_player_up;
extern SDL_Texture *image_player_left;
extern SDL_Texture *image_player_right;
extern SDL_Texture *image_wall_single;
extern SDL_Texture *image_wall_single_open_all;
extern SDL_Texture *image_wall_single_open_left;
extern SDL_Texture *image_wall_single_open_right;
extern SDL_Texture *image_wall_single_open_left_and_right;
extern SDL_Texture *image_wall_single_open_up;
extern SDL_Texture *image_wall_single_open_down;
extern SDL_Texture *image_wall_single_open_up_and_down;
extern SDL_Texture *image_wall_single_open_all_but_down;
extern SDL_Texture *image_wall_single_open_all_but_top;
extern SDL_Texture *image_wall_single_open_all_but_left;
extern SDL_Texture *image_wall_single_open_all_but_right;
extern SDL_Texture *image_wall_single_corner_bl;
extern SDL_Texture *image_wall_single_corner_br;
extern SDL_Texture *image_wall_single_corner_tl;
extern SDL_Texture *image_wall_single_corner_tr;

extern SDL_Surface *img_key;
extern SDL_Texture *image_key;

extern SDL_Surface *img_no_key;
extern SDL_Texture *image_no_key;

extern SDL_Surface *img_has_key;
extern SDL_Texture *image_has_key;

extern SDL_Surface *img_door;
extern SDL_Texture *image_door;

extern SDL_Surface *img_exit;
extern SDL_Texture *image_exit;

extern SDL_Surface *img_border_bl;
extern SDL_Texture *image_border_bl;

extern SDL_Surface *img_border_br;
extern SDL_Texture *image_border_br;

extern SDL_Surface *img_border_tl;
extern SDL_Texture *image_border_tl;

extern SDL_Surface *img_border_tr;
extern SDL_Texture *image_border_tr;

extern SDL_Surface *img_border_lr;
extern SDL_Texture *image_border_lr;

extern SDL_Surface *img_border_tb;
extern SDL_Texture *image_border_tb;

extern SDL_Surface *img_fog;
extern SDL_Texture *image_fog;

extern SDL_Surface *img_heart;
extern SDL_Texture *image_heart;

extern bool has_key;

// Makes an empty map of given size.
void init_grid(int x, int y, char grid[y][x]) {
        for (int index_y = 0; index_y < y; index_y++) {
            for (int index_x = 0; index_x < x; index_x++) {
                if(index_y == player_pos_y && index_x == player_pos_x) {
                    grid[index_y][index_x] = '@';
                    continue;
                }
                //Check on indexes to see if we're on a border
                if(index_x == 0 || index_x == x - 1 || index_y == 0 || index_y == y - 1) {
                    grid[index_y][index_x] = '#';
                    continue;
                }
                //Default empty cell
                grid[index_y][index_x] = ' ';
                }
    }   
}

// Renders the map around players position.
void render_map_around_player(SDL_Renderer* renderer, int x, int y, int screen_width, int screen_height, char grid[y][x]) {

    //Clearing the current render so we're sure to get the render we want.
    SDL_RenderClear(renderer);

    int step_w = screen_width / fov_x;  //Amount of pixels between two tiles, also the size of a tile on the x axis.
    int step_h = screen_height / fov_y; //Amount of pixels between two tiles, also the size of a tile on the y axis.


    // -fov_y/2 is the amount of tiles on player's left. 
    //  fov_y/2 is the amount of tiles on player's right.
    for(int j = -fov_y/2; j < fov_y/2; j++) {     // Iterating through Y axis.
        for(int i = -fov_x/2; i < fov_x/2; i++) { // Iterating through X axis.

            // Empty rectangle that will hold the texture.
            SDL_Rect rect = { step_w * (i + (fov_x/2)), step_h * (j + (fov_y/2)), step_w, step_h};


            // We need to calculate the position of the tile that is about to get rendered.
            // Right now, j and i are just indexes of the tiles that will be treated around, 
            // but they would remain the same every times.
            // Thus, we add the current player position, so every time the player moves, the tile is different.
            int tile_pos_y = j + player_pos_y;
            int tile_pos_x = i + player_pos_x;

            char tile = grid[tile_pos_y][tile_pos_x]; // Char value of the tile.

            // Checking if the current indexes are out of bounds (out of the grid/map).
            if(tile_pos_y < 0 || tile_pos_x < 0 || tile_pos_y >= y || tile_pos_x >= x) {
                SDL_RenderCopy(renderer, image_outer, NULL, &rect);
                continue;
            }

            // Empty tile.
            if(tile == ' ') {

                SDL_RenderCopy(renderer, image_grass, NULL, &rect);
                continue;
            }

            // Key tile.
            if(tile == 'K') {

                SDL_RenderCopy(renderer, image_key, NULL, &rect);
                continue;
            }

            // Exit tile.
            if(tile == 'E') {

                SDL_RenderCopy(renderer, image_exit, NULL, &rect);
                continue;
            }
            // Door tile.
            if(tile == 'D') {

                SDL_RenderCopy(renderer, image_door, NULL, &rect);
                continue;
            }

            // Crate tile.
            if(tile == 'C') {
                SDL_RenderCopy(renderer, image_crate, NULL, &rect);
                continue;
            }

            // Wall tile.
            if(tile == '#') {
                //Checking if corners
                if(tile_pos_y - 1 < 0 && tile_pos_x - 1 < 0) {
                    SDL_RenderCopy(renderer, image_border_tl, NULL, &rect);
                    continue;
                }

                if(tile_pos_y - 1 < 0 && tile_pos_x + 1 == x) {
                    SDL_RenderCopy(renderer, image_border_tr, NULL, &rect);
                    continue;
                }

                if(tile_pos_y + 1 == x && tile_pos_x - 1 < 0) {
                    SDL_RenderCopy(renderer, image_border_bl, NULL, &rect);
                    continue;
                }

                if(tile_pos_y + 1 == x && tile_pos_x + 1 == x) {
                    SDL_RenderCopy(renderer, image_border_br, NULL, &rect);
                    continue;
                }

                if(tile_pos_y - 1 < 0 || tile_pos_y + 1 >= y) {
                    SDL_RenderCopy(renderer, image_border_lr, NULL, &rect);
                    continue;
                }

                if(tile_pos_x - 1 < 0 || tile_pos_x + 1 == x) {
                    SDL_RenderCopy(renderer, image_border_tb, NULL, &rect);
                    continue;
                }

                //Top left 
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#' //Right
                && grid[tile_pos_y + 1][tile_pos_x + 0] == '#' //Bottom
                && grid[tile_pos_y + 0][tile_pos_x - 1] != '#' //Left
                && grid[tile_pos_y - 1][tile_pos_x + 0] != '#')//Top
                {
                    SDL_RenderCopy(renderer, image_wall_single_corner_tl, NULL, &rect);
                    continue;
                }

                //Top right 
                if(grid[tile_pos_y + 0][tile_pos_x + 1] != '#' 
                && grid[tile_pos_y + 1][tile_pos_x + 0] == '#'
                && grid[tile_pos_y + 0][tile_pos_x - 1] == '#'
                && grid[tile_pos_y - 1][tile_pos_x + 0] != '#') {
                    SDL_RenderCopy(renderer, image_wall_single_corner_tr, NULL, &rect);
                    continue;
                }

                //Bot left 
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#' 
                && grid[tile_pos_y + 1][tile_pos_x + 0] != '#'
                && grid[tile_pos_y + 0][tile_pos_x - 1] != '#'
                && grid[tile_pos_y - 1][tile_pos_x + 0] == '#') {
                    SDL_RenderCopy(renderer, image_wall_single_corner_bl, NULL, &rect);
                    continue;
                }

                //Bot right 
                if(grid[tile_pos_y + 0][tile_pos_x + 1] != '#' 
                && grid[tile_pos_y + 1][tile_pos_x + 0] != '#'
                && grid[tile_pos_y + 0][tile_pos_x - 1] == '#'
                && grid[tile_pos_y - 1][tile_pos_x + 0] == '#') {
                    SDL_RenderCopy(renderer, image_wall_single_corner_br, NULL, &rect);
                    continue;
                }

                //Checking if All but one

                //Left
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#'  //Right
                && grid[tile_pos_y + 1][tile_pos_x + 0] == '#'  //Bottom
                && grid[tile_pos_y + 0][tile_pos_x - 1] != '#'  //Left
                && grid[tile_pos_y - 1][tile_pos_x + 0] == '#') //Top
                {
                    SDL_RenderCopy(renderer, image_wall_single_open_all_but_left, NULL, &rect);
                    continue;
                }

                //Right
                if(grid[tile_pos_y + 0][tile_pos_x + 1] != '#'  //Right
                && grid[tile_pos_y + 1][tile_pos_x + 0] == '#'  //Bottom
                && grid[tile_pos_y + 0][tile_pos_x - 1] == '#'  //Left
                && grid[tile_pos_y - 1][tile_pos_x + 0] == '#') //Top
                {
                    SDL_RenderCopy(renderer, image_wall_single_open_all_but_right, NULL, &rect);
                    continue;
                }

                //Up
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#' //Right
                && grid[tile_pos_y + 1][tile_pos_x + 0] == '#' //Bottom
                && grid[tile_pos_y + 0][tile_pos_x - 1] == '#' //Left
                && grid[tile_pos_y - 1][tile_pos_x + 0] != '#')//Top
                {
                    SDL_RenderCopy(renderer, image_wall_single_open_all_but_top, NULL, &rect);
                    continue;
                }

                //Down
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#' //Right
                && grid[tile_pos_y + 1][tile_pos_x + 0] != '#' //Bottom
                && grid[tile_pos_y + 0][tile_pos_x - 1] == '#' //Left
                && grid[tile_pos_y - 1][tile_pos_x + 0] == '#')//Top
                {
                    SDL_RenderCopy(renderer, image_wall_single_open_all_but_down, NULL, &rect);
                    continue;
                }

                //Checking if right and left are open
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#' && grid[tile_pos_y + 0][tile_pos_x - 1] == '#') {
                    // Checking if up and down are open
                    if(grid[tile_pos_y + 1][tile_pos_x + 0] == '#' && grid[tile_pos_y - 1][tile_pos_x - 0] == '#') {
                        SDL_RenderCopy(renderer, image_wall_single_open_all, NULL, &rect);
                        continue;
                    }
                    //Else
                    SDL_RenderCopy(renderer, image_wall_single_open_left_and_right, NULL, &rect);
                    continue;
                }

                //Up and down
                if(grid[tile_pos_y + 1][tile_pos_x + 0] == '#' && grid[tile_pos_y - 1][tile_pos_x - 0] == '#') {
                    // Checking if right and left are open
                    if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#' && grid[tile_pos_y + 0][tile_pos_x - 1] == '#') {
                        SDL_RenderCopy(renderer, image_wall_single_open_all, NULL, &rect);
                        continue;
                    }
                    SDL_RenderCopy(renderer, image_wall_single_open_up_and_down, NULL, &rect);
                    continue;
                }

                //Only up
                if(grid[tile_pos_y - 1][tile_pos_x + 0] == '#') {
                    SDL_RenderCopy(renderer, image_wall_single_open_up, NULL, &rect);
                    continue;
                }

                //Only down
                if(grid[tile_pos_y + 1][tile_pos_x + 0] == '#') {
                    SDL_RenderCopy(renderer, image_wall_single_open_down, NULL, &rect);
                    continue;
                }

                //Only right
                if(grid[tile_pos_y + 0][tile_pos_x + 1] == '#') {
                    SDL_RenderCopy(renderer, image_wall_single_open_left, NULL, &rect);
                    continue;
                }

                //Only left
                if(grid[tile_pos_y + 0][tile_pos_x - 1] == '#') {
                    SDL_RenderCopy(renderer, image_wall_single_open_right, NULL, &rect);
                    continue;
                }



                SDL_RenderCopy(renderer, image_wall_single, NULL, &rect);
                continue;
            }
            
            // Player tile.
            if(tile == '@') {
                switch (direction) {
                    case 0:
                    SDL_RenderCopy(renderer, image_player_down, NULL, &rect);
                    break;

                    case 1:
                    SDL_RenderCopy(renderer, image_player_up, NULL, &rect);
                    break;

                    case 2:
                    SDL_RenderCopy(renderer, image_player_left, NULL, &rect);
                    break;

                    case 3:
                    SDL_RenderCopy(renderer, image_player_right, NULL, &rect);
                    break;
                }
                continue;
            }
            
        }
    }

    SDL_Rect rect_fog = { 0, 0, screen_width, screen_height};
    SDL_RenderCopy(renderer, image_fog, NULL, &rect_fog);

    for (int h = 0; h < 3; h++) {
        SDL_Rect rect_heart = {h * step_w, 0, step_w, step_h};
        SDL_RenderCopy(renderer, image_heart, NULL, &rect_heart);
    }

    SDL_Rect rect_key = { (fov_x - 2) * step_w, (fov_y - 1) * step_w, step_w, step_h};
    SDL_Rect rect_key_amnt = {(fov_x - 1) * step_w, (fov_y - 1) * step_w, step_w, step_h};
    if(has_key) {
        SDL_RenderCopy(renderer, image_has_key, NULL, &rect_key_amnt);
    }
    else {
        SDL_RenderCopy(renderer, image_no_key, NULL, &rect_key_amnt);
    }
    SDL_RenderCopy(renderer, image_key, NULL, &rect_key);

    // Updating the render and showing it.
    SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);
}

