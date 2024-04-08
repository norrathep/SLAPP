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

#include "secure.h"    /* Header file with secure interface API */


void __attribute__((cmse_nonsecure_entry)) secure_checkState(uint8_t *state, int state_size)
{
	return checkState(state, state_size);
}

void __attribute__((cmse_nonsecure_entry)) secure_setState(uint8_t *state, int state_size)
{
	return setState(state, state_size);
}

void __attribute__((cmse_nonsecure_entry)) secure_pox(pox_call_t *pox_fn, uint8_t *pox_token)
{
	return pox(pox_fn, pox_token);
}