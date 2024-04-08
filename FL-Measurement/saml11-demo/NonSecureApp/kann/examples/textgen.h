/*
 * textgen.h
 *
 * Created: 3/28/2024 1:57:35 PM
 *  Author: super
 */ 


#ifndef TEXTGEN_H_
#define TEXTGEN_H_

#include "../kann.h"
#include "../../gpio/uart.h"


typedef struct {
	int len, n_char;//, n_para, *para_len;
	uint8_t *data;//, **para;
	int c2i[256];
} tg_data_t;


void tg_train(kann_t *ann, const tg_data_t *tg, float lr, int ulen, int vlen, int cs, int mbs, int max_epoch, float grad_clip, int batch_len, int n_threads);

kann_t *model_gen(int model, int n_char, int n_h_layers, int n_h_neurons, float h_dropout, int use_norm);




#endif /* TEXTGEN_H_ */