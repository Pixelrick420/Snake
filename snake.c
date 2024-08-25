#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 36
#define HEIGHT 27
#define SNAKEBODY '#'
#define APPLE 'O'
#define EMPTY '.'

struct cell {
    int x;
    int y;
};

struct snake {
    int movedir; //0 = up, 1 = right, 2 = down, 3 = left
    int len;
    struct cell *cells;
} player; 

char board[HEIGHT][WIDTH];
int applecords[3][2] = {{22, 9}, {21, 34}, {21, 10}};
int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
int gameover = 0;
int points = 0;

int random(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void clear(){
    for (int i = 0; i < (HEIGHT + 10); i++) {
        gotoxy(0, i);
        for (int j = 0; j < (WIDTH + 20); j++) {
            printf(" ");
        }
    }
    gotoxy(0, 0); 

}

void init() {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = EMPTY;
        }
    }

    for (int i = 0; i < 3; i++) {
        board[applecords[i][0]][applecords[i][1]] = APPLE;
    }

    player.len = 3;
    player.cells = (struct cell *)malloc(sizeof(struct cell) * player.len);
    player.cells[0].x = 8;
    player.cells[0].y = 8;
    player.cells[1].x = 8;
    player.cells[1].y = 7;
    player.cells[2].x = 8;
    player.cells[2].y = 6;
    player.movedir = 1; 
}

void display() {
    char buffer[HEIGHT + 1][WIDTH * 2 + 1];
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            buffer[i][j * 2] = board[i][j];
            buffer[i][j * 2 + 1] = ' ';
        }
        buffer[i][WIDTH * 2] = '\0';  
    }

    char score[50]; 
    sprintf(score, "POINTS : %d", points);
    snprintf(buffer[HEIGHT], WIDTH * 2 + 1, "%s", score);
    gotoxy(0, 0);
    
    for (int i = 0; i < HEIGHT + 1; i++) {
        printf("%s\n", buffer[i]);
    }
    
    fflush(stdout); 
}

int isvalid(int x, int y){
    for(int i=0;i<player.len;i++){
        if(player.cells[i].x == x && player.cells[i].y == y){
            return 0;
        }
    }
    for(int i=0;i<3;i++){
        if(applecords[i][0] == x && applecords[i][1] == y){
            return 0;
        }
    }
    return 1;
}

void spawn(int index){
    int x = random(0, HEIGHT - 1);
    int y = random(0, WIDTH - 1);
    while(!isvalid(x, y)){
        x = random(0, HEIGHT - 1);
        y = random(0, WIDTH - 1);
    }
    applecords[index][0] = x;
    applecords[index][1] = y;
    board[x][y] = APPLE;

    player.len += 1;
    realloc(player.cells, sizeof(struct cell) * player.len);
}

void pause(){
    while(1){
        if (_kbhit()) {
            char ch = _getch();
            if (ch == ' ') {
                break;
            }
        }
    }
}

void move() {
    int newX = player.cells[0].x + dirs[player.movedir][0];
    int newY = player.cells[0].y + dirs[player.movedir][1];

    if (newX < 0 || newX >= HEIGHT || newY < 0 || newY >= WIDTH) {
        gameover = 1; 
        return;
    }

    int grow = 0;
    for(int i=0;i<3;i++){
        if(applecords[i][0] == newX && applecords[i][1] == newY){
            points += 1;
            spawn(i);
            grow = 1;
        }
    }

    if(!grow){
        board[player.cells[player.len - 1].x][player.cells[player.len - 1].y] = EMPTY;
    }
    
    for (int i = player.len - 1; i > 0; i--) {
        player.cells[i] = player.cells[i - 1];
    }

    player.cells[0].x = newX;
    player.cells[0].y = newY;

    board[player.cells[0].x][player.cells[0].y] = SNAKEBODY;
}

int main() {
    srand(time(NULL));
    init();
    for(int i=0;i<player.len;i++){
        board[player.cells[i].x][player.cells[i].y] = SNAKEBODY;
    }
    

    while (!gameover) {
        Sleep(30);

        if (_kbhit()) {
            char ch = _getch();  
            switch (ch) {
                case 'w': 
                    if (player.movedir != 2) player.movedir = 0;
                    break;
                case 'a': 
                    if (player.movedir != 1) player.movedir = 3;
                    break;
                case 's': 
                    if (player.movedir != 0) player.movedir = 2;
                    break;
                case 'd': 
                    if (player.movedir != 3) player.movedir = 1;
                    break;
                case ' ': 
                    pause();
                    break;
            }
        }

        move();
        if (gameover) {
            printf("Game Over!\n");
            break;
        }
        display(); 
    }

    free(player.cells);
    return 0;
}
