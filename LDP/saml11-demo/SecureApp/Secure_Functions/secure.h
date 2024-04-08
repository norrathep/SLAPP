/**
 * Copyright (c) 2019 Microchip Technology Inc.
 *
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef SECURE_H_
#define SECURE_H_

#include <stdint.h>


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

extern void pox(pox_call_t *pox_fn, uint8_t pox_token[32]);

#endif /* SECURE_H_ */