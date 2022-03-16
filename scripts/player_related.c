#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

extern int fov_x, fov_y;
extern int player_pos_x, player_pos_y;
extern int direction;
extern bool has_key;
extern bool won;
extern int size_x, size_y, width, height;
extern SDL_Renderer *renderer;

void SpawnPlayer(int x, int y, char grid[y][x]) {
    grid[player_pos_y][player_pos_x] = '@';
}

void Move(int dest_x, int dest_y, int x, int y, char grid[y][x]) {
    if(player_pos_x + dest_x < x && player_pos_y + dest_y < y) {
        if(grid[player_pos_y + dest_y][player_pos_x + dest_x] == 'E') {
            won = true;
            grid[player_pos_y + dest_y][player_pos_x + dest_x] = '@';
            grid[player_pos_y][player_pos_x] = ' ';
            render_map_around_player(renderer, size_x, size_y, width, height, grid);
            SDL_Delay(1000);
            int i = 155;
            while (i > 0)
            {
                SDL_Rect rect_fade = {0, 0, width, height};
                SDL_SetRenderDrawColor(renderer, i, i, i, 255);
                SDL_RenderFillRect(renderer, &rect_fade);
                SDL_RenderPresent(renderer);

                i--;
                SDL_Delay(10);
            }
            grid[player_pos_y + dest_y][player_pos_x + dest_x] = 'E';
            render_map_around_player(renderer, size_x, size_y, width, height, grid);
            return;
        }
        if(grid[player_pos_y + dest_y][player_pos_x + dest_x] == '#') {
            return;
        }
        if(grid[player_pos_y + dest_y][player_pos_x + dest_x] == 'C') {
            if(grid[player_pos_y + dest_y * 2][player_pos_x + dest_x * 2] == '#') {
                return;
            }
            grid[player_pos_y + dest_y * 2][player_pos_x + dest_x * 2] = 'C';
            grid[player_pos_y + dest_y][player_pos_x + dest_x] = '@';
            grid[player_pos_y][player_pos_x] = ' ';
            player_pos_x = player_pos_x + dest_x;
            player_pos_y = player_pos_y + dest_y;

            return;
        }
        if(grid[player_pos_y + dest_y][player_pos_x + dest_x] == 'K') {
            has_key = true;
        }

        if(grid[player_pos_y + dest_y][player_pos_x + dest_x] == 'D') {
            if(!has_key) { return;}
            has_key = false;
        }

        grid[player_pos_y + dest_y][player_pos_x + dest_x] = '@';
        grid[player_pos_y][player_pos_x] = ' ';

        player_pos_x = player_pos_x + dest_x;
        player_pos_y = player_pos_y + dest_y;

    }
}