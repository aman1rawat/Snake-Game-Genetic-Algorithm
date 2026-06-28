#include "snakegame.h"

void initNetwork(Network *network){
	float lim = sqrtf(6.0f/(INPUT_SIZE+HIDDEN_SIZE));
	int n = INPUT_SIZE*HIDDEN_SIZE;
	for(int i=0;i<n;i++){
		float num = (float)rand()/RAND_MAX;
		network->w1[i] = lim*(2.0f*num - 1.0f);
	}

	lim = sqrtf(6.0f/(HIDDEN_SIZE+OUTPUT_SIZE));
	n = HIDDEN_SIZE*OUTPUT_SIZE;
	for(int i=0;i<n;i++){
		float num = (float)rand()/RAND_MAX;
		network->w2[i] = lim*(2.0f*num - 1.0f);
	}

	for(int i=0;i<HIDDEN_SIZE;i++){
		network->b1[i] = 0.0f;
	}
	for(int i=0;i<OUTPUT_SIZE;i++){
		network->b2[i] = 0.0f;
	}
}

static inline float relu(float x){
	return (x<0.0f)?0.0f:x;
}

void forwardProp(Network *network, const float *input, float *output){
	float hidden_output[HIDDEN_SIZE] = {0.0f};
	for(int i=0;i<HIDDEN_SIZE;i++){
		float sum = 0.0f;
		for(int j=0;j<INPUT_SIZE;j++){
			sum += input[j]*network->w1[i*INPUT_SIZE + j];
		}
		hidden_output[i] += sum;
		hidden_output[i] += network->b1[i];
		hidden_output[i] = relu(hidden_output[i]);
	}

	for(int i=0;i<OUTPUT_SIZE;i++){
		float sum = 0.0f;
		for(int j=0;j<HIDDEN_SIZE;j++){
			sum += hidden_output[j]*network->w2[i*HIDDEN_SIZE + j];
		}
		output[i] = sum+network->b1[i];
		// output[i] = relu(output[i]);
	}
}

int getMove(Network *network, float *vision_array){
	float output[OUTPUT_SIZE];
	forwardProp(network, vision_array, output);

	int max_i=0;
	float max_val=output[0];
	for(int i=1;i<OUTPUT_SIZE;i++){
		if(output[i]>max_val){
			max_i = i;
			max_val = output[i];
		}
	}

	return max_i;
}

void crossover(Network *parent1, Network *parent2, Network *child){
    for(int i=0;i<INPUT_SIZE*HIDDEN_SIZE;i++){ 
        child->w1[i] = (rand()%2==0)?parent1->w1[i]:parent2->w1[i];
    }
    
    for(int i=0;i<HIDDEN_SIZE;i++){
    	child->b1[i] = (rand()%2==0)?parent1->b1[i]:parent2->b1[i];
	}
        
    for(int i=0;i<HIDDEN_SIZE*OUTPUT_SIZE;i++){
        child->w2[i] = (rand()%2==0)?parent1->w2[i]:parent2->w2[i];
    }
        
    for(int i=0;i<OUTPUT_SIZE;i++){
        child->b2[i] = (rand()%2==0)?parent1->b2[i]:parent2->b2[i];
    }
}

void mutate(Network *net){
    for(int i=0;i<INPUT_SIZE*HIDDEN_SIZE;i++){
        if((float)rand()/RAND_MAX < MUTATION_RATE) 
            net->w1[i] += (((float)rand()/RAND_MAX)*2.0f-1.0f)*0.1f;
    }
    for(int i=0;i<HIDDEN_SIZE;i++){
        if((float)rand()/RAND_MAX < MUTATION_RATE) 
            net->b1[i] += (((float)rand()/RAND_MAX)*2.0f-1.0f)*0.1f;
    }
    for(int i=0;i<HIDDEN_SIZE*OUTPUT_SIZE;i++){
        if((float)rand()/RAND_MAX < MUTATION_RATE) 
            net->w2[i] += (((float)rand()/RAND_MAX)*2.0f-1.0f)*0.1f;
    }
    for(int i=0;i<OUTPUT_SIZE;i++){
        if((float)rand()/RAND_MAX < MUTATION_RATE) 
            net->b2[i] += (((float)rand()/RAND_MAX)*2.0f-1.0f)*0.1f;
    }
}

