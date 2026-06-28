#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include<stdio.h>
#include<stdbool.h>
#include<windows.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define BOARD_SIZE 25
#define MAX_MOVES 100

#define INPUT_SIZE 16
#define HIDDEN_SIZE 16
#define OUTPUT_SIZE 4

#define TOTAL_GENERATIONS 1000
#define POPULATION_SIZE 500
#define MUTATION_RATE 0.05f

#define CLEAR_CMD "\033[2J\033[3J\033[H"
#define CLEAR_CMD_LEN (sizeof(CLEAR_CMD) - 1)

typedef struct{
    float w1[INPUT_SIZE * HIDDEN_SIZE];
    float b1[HIDDEN_SIZE];
    float w2[HIDDEN_SIZE * OUTPUT_SIZE];
    float b2[OUTPUT_SIZE];
}Network;

typedef struct{
    char board[BOARD_SIZE * BOARD_SIZE];
    int direction;
    int snake_x[BOARD_SIZE * BOARD_SIZE], snake_y[BOARD_SIZE * BOARD_SIZE];
    int snake_len;
    int moves_left;
    bool isGameOver;
    int score;
    int food_x, food_y;
}SnakeGame;

void initGame(SnakeGame *game);
void generateFood(SnakeGame *game);
void updateSnake(SnakeGame *game, int move);
void get_snake_vision(SnakeGame *game, float *vision_array);
void renderGame(SnakeGame *game);

void initNetwork(Network *network);
void forwardProp(Network *network, const float *input, float *output);
int getMove(Network *network, float *vision_array);
void mutate(Network *net);
void crossover(Network *parent1, Network *parent2, Network *child);

#endif