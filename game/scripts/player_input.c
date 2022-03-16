#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

extern int fov_x;
extern int fov_y;
extern int size_x, size_y, width, height, direction;

int Read_Input(SDL_Renderer* renderer, int x, int y, int screen_width, int screen_height, char grid[y][x]) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
        {

            /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_m:
                    if (fov_x < 22)
                    {
                        fov_x += 2;
                        fov_y += 2;
                        render_map_around_player(renderer, size_x, size_y, width, height, grid);
                    }
                    break;
                case SDLK_p:
                    if (fov_x > 10)
                    {
                        fov_x -= 2;
                        fov_y -= 2;
                        render_map_around_player(renderer, size_x, size_y, width, height, grid);
                    }
                    break;
                case SDLK_LEFT:
                case SDLK_q:
                    Move(-1, 0, size_x, size_x, grid);
                    direction = 2;
                    render_map_around_player(renderer, size_x, size_y, width, height, grid);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    Move(1, 0, size_x, size_x, grid);
                    direction = 3;
                    render_map_around_player(renderer, size_x, size_y, width, height, grid);
                    break;
                case SDLK_UP:
                case SDLK_z:
                    Move(0, -1, size_x, size_x, grid);
                    direction = 1;
                    render_map_around_player(renderer, size_x, size_y, width, height, grid);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    Move(0, 1, size_x, size_x, grid);
                    direction = 0;
                    render_map_around_player(renderer, size_x, size_y, width, height, grid);
                    break;
                }
                break;

            case SDL_QUIT:
                return 5;
            default:
                break;
            }
        }
    return 0;
}
