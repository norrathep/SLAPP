#pragma once

#define INPUT_SIZE 16
#define HIDDEN_SIZE 4
#define OUTPUT_SIZE 1
#define FEATURE_SIZE 24 // how many readings per row

typedef struct
{
    // All the weights.
	//float *w;
    float *w;
    // Hidden to output layer weights.
    float* x;
    // Biases.
    //float *b;
    float *b;
    // Hidden layer.
    float *h;
    //float h[HIDDEN_SIZE];
    // Output layer.
    float *o;
    // Number of biases - always two - Tinn only supports a single hidden layer.
    int nb;
    // Number of weights.
    int nw;
    // Number of inputs.
    int nips;
    // Number of hidden neurons.
    int nhid;
    // Number of outputs.
    int nops;
} Tinn;

float* xtpredict(Tinn, const float* in);

float xttrain(Tinn, const float* in, const float* tg, float rate);

Tinn xtbuild(int nips, int nhid, int nops);

void xtsave(Tinn, const char* path);

Tinn xtload(const char* path);

void xtfree(Tinn);

void xtprint(const float* arr, const int size);

void wbrand(Tinn t);

void setWeight(Tinn t, float init_val);
