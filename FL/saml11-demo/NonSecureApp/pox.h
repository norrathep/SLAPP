/*
 * pox.h
 *
 * Created: 6/22/2022 3:24:43 PM
 *  Author: Oak
 */ 


#ifndef POX_H_
#define POX_H_

typedef struct pox_call {
	uint32_t addr;
	uint8_t *input;
	int input_size;
	uint8_t *output;
	int output_size;
	uint32_t counter;
	uint8_t *command;
	uint8_t command_size;
} pox_call_t;

#endif /* POX_H_ */