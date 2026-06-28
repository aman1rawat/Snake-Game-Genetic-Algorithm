#include "./src/snakegame.h"

typedef struct{
    int id;
    float fitness;
}Agent;

SnakeGame games[POPULATION_SIZE];
Network networks[POPULATION_SIZE]; 
Network next_generation[POPULATION_SIZE];
Agent agents[POPULATION_SIZE];

int compare_agents(const void* a, const void* b) {
    Agent* agentA = (Agent*)a;
    Agent* agentB = (Agent*)b;
    if(agentB->fitness>agentA->fitness) return 1;
    if(agentB->fitness<agentA->fitness) return -1;
    return 0;
}

void showcase_best_snake(Network* network){
    SnakeGame *showcase = (SnakeGame*)malloc(sizeof(SnakeGame));
    initGame(showcase);
    float vision[16];

    while(!showcase->isGameOver){
        get_snake_vision(showcase, vision);
        int move = getMove(network, vision); 
        updateSnake(showcase, move);
        renderGame(showcase);
        Sleep(50);
    }

    free(showcase);
}

int main(){
    srand(time(NULL));
    
    for(int gen=1;gen<=TOTAL_GENERATIONS;gen++){
        //initialize networks
        if(gen==1){
            for(int i=0;i<POPULATION_SIZE;i++){
                initNetwork(&networks[i]);
            }
        }

        //initialize games
        for(int i=0;i<POPULATION_SIZE;i++){
            initGame(&games[i]);
        }

        //play all games
        int games_running = POPULATION_SIZE;
        while(games_running>0){
            games_running = 0;
            for(int i=0;i<POPULATION_SIZE;i++){
                if(games[i].isGameOver) continue;
                games_running++;

                float vision[16];
                get_snake_vision(&games[i], vision);
                int move = getMove(&networks[i], vision);
                updateSnake(&games[i], move);
            }
        }

        //calculate fitness
        for(int i=0;i<POPULATION_SIZE;i++){
            agents[i].id = i;
            agents[i].fitness = games[i].score*100.0f + (MAX_MOVES-games[i].moves_left);
        }

        //sort with respect to fitness
        qsort(agents, POPULATION_SIZE, sizeof(Agent), compare_agents);

        //play best performing snake
        showcase_best_snake(&networks[agents[0].id]);

        //copy top 15% networks directly
        int topK = POPULATION_SIZE/15;
        for(int i=0;i<topK;i++){
            next_generation[i] = networks[agents[i].id];
        }

        //crossover rest of the networks
        for(int i=topK;i<POPULATION_SIZE;i++){
            int p1 = agents[rand()%topK].id;
            int p2 = agents[rand()%topK].id;
            crossover(&networks[p1], &networks[p2], &next_generation[i]);
            mutate(&next_generation[i]);
        }

        //populate new generation
        for(int i=0;i<POPULATION_SIZE;i++){
            networks[i] = next_generation[i];
        }
    }

    return 0;
}