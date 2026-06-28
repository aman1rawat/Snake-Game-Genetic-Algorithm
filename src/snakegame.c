#include "snakegame.h"

int dir[][2] = {{0,-1},{-1,0},{0,1},{1,0}};
char buffer[4096];

void initGame(SnakeGame *game){
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            if(i==0 || i==BOARD_SIZE-1 || j==0 || j==BOARD_SIZE-1) game->board[i*BOARD_SIZE+j] = '#';
            else game->board[i*BOARD_SIZE+j] = ' ';
        }
    }
    game->score = 0;
    game->isGameOver = false;
    game->direction = 1; 
    game->snake_len = 1;
    game->snake_x[0] = game->snake_y[0] = BOARD_SIZE/2;
    game->board[game->snake_x[0] * BOARD_SIZE + game->snake_y[0]] = '0';
    game->moves_left = MAX_MOVES;

    generateFood(game);
}

void generateFood(SnakeGame *game){
    int x,y;
    do{
        x = rand()%(BOARD_SIZE);
        y = rand()%(BOARD_SIZE);
    }while(game->board[x*BOARD_SIZE+y]!=' ');
    game->board[x*BOARD_SIZE+y] = 'Q';
    
    game->food_x = x;
    game->food_y = y;
}

void updateSnake(SnakeGame *game, int move){
    game->direction = move;
    int new_x = game->snake_x[0] + dir[move][0];
    int new_y = game->snake_y[0] + dir[move][1];

    if(game->moves_left==0 || game->board[new_x*BOARD_SIZE+new_y]=='#' || game->board[new_x*BOARD_SIZE+new_y]=='o'){
        game->isGameOver = true;
        return;
    }

    game->moves_left--;
    if(game->board[new_x*BOARD_SIZE+new_y]=='Q'){
        int i = game->snake_len;
        while(i>0){
            game->snake_x[i] = game->snake_x[i-1];
            game->snake_y[i] = game->snake_y[i-1];
            game->board[game->snake_x[i]*BOARD_SIZE+game->snake_y[i]] = 'o';
            i--;
        }
        game->snake_x[0] = new_x;
        game->snake_y[0] = new_y;
        game->board[game->snake_x[0]*BOARD_SIZE+game->snake_y[0]] = '0';

        game->snake_len++;
        game->score++;
        game->moves_left = MAX_MOVES;
        generateFood(game);
    }
    else{
        int i = game->snake_len-1;
        game->board[game->snake_x[i]*BOARD_SIZE+game->snake_y[i]] = ' ';

        while(i>0){
            game->snake_x[i] = game->snake_x[i-1];
            game->snake_y[i] = game->snake_y[i-1];
            game->board[game->snake_x[i]*BOARD_SIZE+game->snake_y[i]] = 'o';
            i--;
        }
        game->snake_x[0] = new_x;
        game->snake_y[0] = new_y;
        game->board[game->snake_x[0]*BOARD_SIZE+game->snake_y[0]] = '0';
    }
}

void get_snake_vision(SnakeGame *game, float *vision){
    int head_x = game->snake_x[0];
    int head_y = game->snake_y[0];

    int dx[] = {0, -1, -1, -1, 0, 1, 1, 1};
    int dy[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    
    for (int i = 0; i < 8; i++) {
        int nx = head_x + dx[i];
        int ny = head_y + dy[i];
        char tile = game->board[nx * BOARD_SIZE + ny];
        vision[i] = (tile == '#' || tile == 'o') ? 1.0f : 0.0f;
    }

    // 2. Current Direction (One-hot encoded: Left, Top, Right, Down)
    for (int i = 0; i < 4; i++) {
        vision[8 + i] = (game->direction == i) ? 1.0f : 0.0f;
    }

    vision[12] = (game->food_y < head_y) ? 1.0f : 0.0f; // Left
    vision[13] = (game->food_x < head_x) ? 1.0f : 0.0f; // Top
    vision[14] = (game->food_y > head_y) ? 1.0f : 0.0f; // Right
    vision[15] = (game->food_x > head_x) ? 1.0f : 0.0f; // Down
}

void renderGame(SnakeGame *game){
    memcpy(buffer, CLEAR_CMD, CLEAR_CMD_LEN);
    int pos = CLEAR_CMD_LEN;
    for(int i=0; i<BOARD_SIZE; i++){
        for(int j=0; j<BOARD_SIZE; j++) {
            buffer[pos++] = game->board[i * BOARD_SIZE + j];
            buffer[pos++] = ' '; 
        }
        buffer[pos++] = '\n';
    }
    pos += snprintf(buffer+pos, 4096-pos, "Score : %d\tMoves Left : %d\n", game->score, game->moves_left);  
    fwrite(buffer, sizeof(char), pos, stdout);
    fflush(stdout); 
}