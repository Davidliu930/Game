#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 20

int grid[HEIGHT][WIDTH];

// Tetrimino shapes
int shapes[7][4][4] = {
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I
    {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // J
    {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // L
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // O
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // S
    {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // T
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}   // Z
};

int tetrimino_x = WIDTH / 2 - 2;
int tetrimino_y = 0;
int tetrimino_type;
int tetrimino_rotation;

void draw_tetrimino(int x, int y, int type, int rotation) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shapes[type][rotation][i * 4 + j] != 0) {
                grid[y + i][x + j] = 1;
            }
        }
    }
}

void erase_tetrimino(int x, int y, int type, int rotation) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shapes[type][rotation][i * 4 + j] != 0) {
                grid[y + i][x + j] = 0;
            }
        }
    }
}

int check_collision(int x, int y, int type, int rotation) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (shapes[type][rotation][i * 4 + j] != 0) {
                if (x + j < 0 || x + j >= WIDTH || y + i >= HEIGHT || grid[y + i][x + j] != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void clear_rows() {
    int i, j, k;
    for (i = HEIGHT - 1; i >= 0; i--) {
        int full = 1;
        for (j = 0; j < WIDTH; j++) {
            if (grid[i][j] == 0) {
                full = 0;
                break;
            }
        }
        if (full) {
            for (k = i; k > 0; k--) {
                for (j = 0; j < WIDTH; j++) {
                    grid[k][j] = grid[k - 1][j];
                }
            }
            for (j = 0; j < WIDTH; j++) {
                grid[0][j] = 0;
            }
        }
    }
}

void draw_grid() {
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (grid[i][j] != 0) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void move_down() {
    erase_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
    if (!check_collision(tetrimino_x, tetrimino_y + 1, tetrimino_type, tetrimino_rotation)) {
        tetrimino_y++;
    } else {
        draw_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
        clear_rows();
        tetrimino_x = WIDTH / 2 - 2;
        tetrimino_y = 0;
        tetrimino_type = rand() % 7;
        tetrimino_rotation = 0;
        if (check_collision(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation)) {
            system("cls");
            printf("Game Over\n");
            exit(0);
        }
    }
    draw_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
}

void move_left() {
    erase_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
    if (!check_collision(tetrimino_x - 1, tetrimino_y, tetrimino_type, tetrimino_rotation)) {
        tetrimino_x--;
    }
    draw_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
}

void move_right() {
    erase_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
    if (!check_collision(tetrimino_x + 1, tetrimino_y, tetrimino_type, tetrimino_rotation)) {
        tetrimino_x++;
    }
    draw_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
}

void rotate() {
    erase_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
    int next_rotation = (tetrimino_rotation + 1) % 4;
    if (!check_collision(tetrimino_x, tetrimino_y, tetrimino_type, next_rotation)) {
        tetrimino_rotation = next_rotation;
    }
    draw_tetrimino(tetrimino_x, tetrimino_y, tetrimino_type, tetrimino_rotation);
}

int main() {
    int i, j;

    srand(time(NULL));

    // Initialize the grid
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            grid[i][j] = 0;
        }
    }

    tetrimino_type = rand() % 7;
    tetrimino_rotation = 0;

    while (1) {
        system("cls");
        draw_grid();

        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'a':
                    move_left();
                    break;
                case 'd':
                    move_right();
                    break;
                case 's':
                    move_down();
                    break;
                case 'w':
                    rotate();
                    break;
                case 'q':
                    exit(0);
            }
        }

        Sleep(500);
        move_down();
    }

    return 0;
}
