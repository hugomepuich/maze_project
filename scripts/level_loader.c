#include <stdlib.h>
#include <stdio.h>

/*

[09/03/22] Can load a level from a file.
[15/03/22] Fixed the loading bug, last line was ignored.
*/


void open_file(char* path, int map_dimension_x, int map_dimension_y, char grid[map_dimension_x][map_dimension_y]) {
    FILE *map_file = fopen(path, "r");

    int current_x = 0;
    int current_y = 0;
    char c = '.';
    while(c != EOF) {
        c = fgetc(map_file);
        if (c == EOF) { break; }
        if (current_x == map_dimension_x) {
            current_x = 0;
            current_y++;
            continue;
        }
        if(current_y == map_dimension_y) {
            break;
        }
        grid[current_y][current_x] = c;
        current_x++;
    }
    fclose(map_file);
}