#ifndef __cfa_engine
#define __cfa_engine

#include "cfa_engine.h"
#include <string.h>
#include "usart.h"
#include "serial_messages.h"
#include "auc.h"
#include "tim.h"
#include "uECC.h"
#include "hash.h"
#include "Hacl_SHA2_256.h"
#include "Hacl_HMAC_SHA2_256.h"
#include "stm32l5xx_hal.h"
#include <stm32l5xx.h>
#include "hash.h"
/*
    ---------------------------------------------------------------------------------
	----------------------------- FUNCTION DEFINITIONS ------------------------------
	---------------------------------------------------------------------------------
*/

// Public
void CFA_ENGINE_new_log_entry(uint32_t value);
void CFA_ENGINE_run_attestation();
void CFA_ENGINE_initialize(); // Initial setup of attestation variables
void CFA_ENGINE_register_callback(); // Register the function to be attested
void CFA_ENGINE_wait_request();
void CFA_time_interrupt_handler();

// Private
void _generate_partial_report(); // emptya
void _full_report_routine(); // empty
void _INTERRUPT_cfa_engine_timeout(); // empty
void _generate_final_report(); // empty
void _send_final_report_to_vrf(); // empty
void _send_partial_report_to_vrf(); // empty
void _software_reset();
void _activate_timeout_interrupt();
void _deactivate_timer_interrupt();
void _run_application();
void _calculate_log_hash();
void _recalculate_log_table();
void _clean();
void _send_report();
void _read_serial_loop();
void _heal_function();
void _attest_memory();
void _sign_report();
void _send_report_message();
void _receive_request(int size,uint8_t *read_char);
uint8_t _receive_challenge();
uint8_t _authenticate();
void update_flash();
/*
    ---------------------------------------------------------------------------------
	----------------------------- LOCAL DEFS  ---------------------------------------
	---------------------------------------------------------------------------------
*/

#define RUN_APP_CALLBACK() cfa_engine_conf.iac.app_start_address()
#define SIZEOF_MESSAGE_FACTOR (sizeof(unsigned int)/sizeof(char))

/*
    ---------------------------------------------------------------------------------
	----------------------------- GLOBAL VARIABLES ----------------------------------
	---------------------------------------------------------------------------------
*/


CFA_ENGINE_CONFIG cfa_engine_conf;
CFA_REPORT report_secure __attribute__((section(".cfa_data")));
VRF_RESPONSE vrf_resp;
DMA_HandleTypeDef hdma;

#if CFLOG_TYPE == CFLOG_FLASH
FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t FLASH_CFLog[MAX_CF_LOG_SIZE] __attribute__((section(".cfa_data")));  //
#endif

extern void *pAttestationFunctionCallback;



uint8_t private_key[32] = {0x89, 0x8b, 0xc, 0xde, 0xab, 0xb5, 0x86, 0x97,
						 0x3c, 0x0d, 0xa9, 0x15, 0x8e, 0x01, 0x84, 0xe7,
						 0x43, 0xf7, 0x7c, 0x35, 0xb9, 0xc2, 0xb8, 0xf7,
						 0x93, 0x62, 0x92, 0x87, 0xca, 0x59, 0x8b, 0xb2};

uint8_t public_key[SIGNATURE_SIZE_BYTES] = {0x74, 0x2f, 0xbe, 0xaf, 0x4f, 0xf4, 0x1d, 0x27,
										  0x48, 0xad, 0x4e, 0x23, 0x84, 0x79, 0xa4, 0x42,
										  0x68, 0x8b, 0xe6, 0x18, 0x98, 0x45, 0x99, 0xf1,
										  0x10, 0x3, 0x4e, 0xf6, 0x5e, 0xe1, 0x1e, 0x4f,
										  0x49, 0xd0, 0x8e, 0xe0, 0x53, 0xa0, 0xe2, 0x7d,
										  0xb5, 0x9, 0xf1, 0xfd, 0x2e, 0xa5, 0x63, 0x70,
										  0xc0, 0x5e, 0x20, 0x31, 0xc5, 0xc0, 0xcb, 0x4b,
										  0x1, 0x75, 0xd7, 0x38, 0xeb, 0x2b, 0x44, 0x1b};


uint8_t * att_key = private_key;

uint8_t report_hash[HASH_SIZE_BYTES]; //internal for signature
uint8_t hash_output[HASH_SIZE_BYTES]; //internal for signature
uint8_t ecdsa_signature[SIGNATURE_SIZE_BYTES];
uint8_t response_hash[HASH_SIZE_BYTES]; // internal for vrf_signature
const struct uECC_Curve_t * curve;


// TIMING COUNTERS
uint32_t app_exec_time = 0;
uint32_t time_sign_report = 0;
uint32_t send_report_time = 0;
uint32_t receive_resp_time = 0;
uint32_t verify_resp_time = 0;
//
uint32_t ticks_sign_report;
uint32_t recv_verify_response_time;
uint32_t start;
uint32_t end;
uint32_t receive_resp_start;
uint32_t receive_resp_stop;
uint32_t verify_resp_start;
uint32_t verify_resp_stop;
uint32_t hmac_time;
uint32_t send_report_start;
uint32_t send_report_stop;
uint32_t time_sign_report_start;
uint32_t time_sign_report_end;
/*
    ---------------------------------------------------------------------------------
	----------------------------- SUPERVISOR  --------------------------------------------
	---------------------------------------------------------------------------------
*/

//-------------------------- UTILS -------//
void _setup_data(){
	for(unsigned int i = 0; i < CHAL_SIZE; ++i ){
		cfa_engine_conf.hash_key[i] = i;
		cfa_engine_conf.challenge[i] = 0x65+i;
	}
}

void _update_challenge(uint8_t* chl){
	memcpy(cfa_engine_conf.challenge,chl, CHAL_SIZE);
	memcpy(vrf_resp.new_challenge,chl, CHAL_SIZE);
	cfa_engine_conf.challenge_renewed = TRUE;
}

void _clean(){
	cfa_engine_conf.log_counter = 0;
	report_secure.number_of_logs_sent = 0;
	cfa_engine_conf.initialized = INITIALIZED;
	cfa_engine_conf.attestation_status = INACTIVE;
	report_secure.num_CF_Log_size = 0;
	cfa_engine_conf.challenge_renewed = FALSE;
	report_secure.isFinal = FALSE;
	for(int i=0; i<MAX_CF_LOG_SIZE; i++){
		report_secure.CFLog[i] = 0;
	}
}

void _run_application(){
	//start app
	if (cfa_engine_conf.iac.app_start_address != NULL){
		cfa_engine_conf.iac.app_start_address();
	}
	return;
}

uint32_t output_data = 0;
void record_output_data(uint32_t value){
	output_data = value;
}

/* -------------  NON SECURE CALLABLES */

void CFA_ENGINE_start(){
	while(1){
		_attest_memory();
		_setup_data();
		_clean();
		_read_serial_loop();
	}
}

void CFA_ENGINE_register_callback(){
	cfa_engine_conf.iac.app_start_address = (funcptr_NS)  pAttestationFunctionCallback;
	return;
}

/* --------------- - STATE HANDLING --------------------- */


uint32_t total_runtime = 0;
uint32_t total_start;
uint32_t total_stop;
int STATE_initialize_attestation(){
	total_start = HAL_GetTick();
	if (cfa_engine_conf.attestation_status == INACTIVE){

		// send response to vrf
		uint8_t ready_char[COMMAND_SIZE+1] = INIT_ATTESTATION;
		SecureUartTx(ready_char, COMMAND_SIZE);

		// receive challange
		if(_receive_challenge()) return 1;

		// start application
		cfa_engine_conf.attestation_status = ACTIVE;
		CFA_ENGINE_run_attestation();
		cfa_engine_conf.attestation_status = COMPLETE;

		// Send final report
		_send_report();
	}

	return CONTINUE_LOOP;
}

// put breakpoint here to get app end
int STATE_accept_report(){
	if (cfa_engine_conf.attestation_status == COMPLETE){
		cfa_engine_conf.attestation_status = INACTIVE; //temp
		_clean();
		total_stop = HAL_GetTick();
		total_runtime = total_stop-total_start;
		return EXIT_LOOP;
	}
	return CONTINUE_LOOP;
}

int STATE_heal(){
	if (cfa_engine_conf.attestation_status == COMPLETE || cfa_engine_conf.attestation_status == WAITING_PARTIAL){
		_heal_function();
		return EXIT_LOOP;
	}
	return CONTINUE_LOOP;
}

int STATE_continue(){

	_receive_challenge();

	cfa_engine_conf.attestation_status = ACTIVE;

//	_initialize_timer_interrupt();

	return EXIT_LOOP;
}

/* ---------------------------  PROTOCOL  ----------------------------------------- */


char translate_command(uint8_t command_received[]){
	command_received[COMMAND_SIZE] = '\0';
	if (!strcmp(command_received,INIT_ATTESTATION))
		return INIT_ATTESTATION_CHAR;
	else if (!strcmp(command_received,ACCEPTED_REPORT))
		return ACCEPT_REPORT_CHAR;
	else if (!strcmp(command_received,CONTINUE_REQUEST))
		return CONTINUE_CHAR;
	else if (!strcmp(command_received,HEAL_REQUEST))
		return HEAL_CHAR;
	return 'X';

}

void wait_for_command(char pattern[],int size){
	int found_size = 0;
	int i = 0;
	char read_char;
	while(found_size != size){
		SecureUartRx((uint8_t*)&read_char, 1);
		if (read_char == pattern[i]){
			i++;
			found_size++;
		}
		else{
			i = 0;
			found_size = 0;
		}
	}
}

void _read_serial_loop(){
	uint8_t command_received[COMMAND_SIZE+1];
	uint8_t state = CONTINUE_LOOP;
	char aaa;
	while(state == CONTINUE_LOOP){
		wait_for_command(BEGGINING_OF_COMMAND,COMMAND_SIZE);
		_receive_request(COMMAND_SIZE, command_received);
		aaa = translate_command(command_received);
		switch(aaa){

			case INIT_ATTESTATION_CHAR:
				state = STATE_initialize_attestation();
				break;

			case ACCEPT_REPORT_CHAR:
				state = STATE_accept_report();
				break;

			case HEAL_CHAR:
				state = STATE_heal();
				break;

			case CONTINUE_CHAR:
				state = STATE_continue();
				break;

			default:
				state = CONTINUE_LOOP;
				break;
		}
	}
}

/* ------------------------------ INTERRUPT OPERATIONS ----------------------------- */


#define TIMER_PERIOD 65535-1

void _initialize_timer_interrupt(){
	__HAL_RCC_TIM3_CLK_DISABLE();
	ti_syncDebbugTimer();
//	ti_set_prescaler(TIMER_PERIOD,0);
	ti_set_period(TIMER_INTERRUPT,TIMER_PERIOD);
	ti_start_timer_interrupt(TIMER_INTERRUPT);
	_activate_timeout_interrupt();
	ti_reset_timer_counter(TIMER_INTERRUPT);
	__HAL_RCC_TIM3_CLK_ENABLE();

}

void _activate_timeout_interrupt(){
	HAL_TIM_Base_Start(&htim3);
}


void _deactivate_timer_interrupt(){
	HAL_TIM_Base_Stop(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3, 0); // reset count
}

void CFA_time_interrupt_handler(){
	__HAL_RCC_TIM3_CLK_DISABLE();
	report_secure.isFinal = PARTIAL_REPORT;
	_sign_report();
	_send_report_message();
	report_secure.num_CF_Log_size = 0;
	_read_serial_loop();
	ti_reset_timer_counter(TIMER_INTERRUPT);
	__HAL_RCC_TIM3_CLK_ENABLE();
}

/* -----------------------------  SENDING REPORT ------------------------------------ */

uint8_t  _receive_challenge(){
	uint8_t chl[64];
//	uint8_t init_chal[] = BEGGINING_OF_CHALLANGE;

	SecureUartRx((uint8_t*)(&chl[0]), 1);
	receive_resp_start = HAL_GetTick();
	SecureUartRx((uint8_t*)(&chl[1]), CHAL_SIZE-1);
//	uint32_t recv_chal_stop = HAL_GetTick();
//	uint32_t recv_chal = recv_chal_stop - receive_resp_start;
//	SecureUartTx(init_chal, COMMAND_SIZE); // echo for debug
//	SecureUartTx((uint8_t*)chl, 64);

	// read verifier signature
	SecureUartRx((uint8_t*)(&vrf_resp.signature), 32);
//	SecureUartTx((uint8_t*)(&vrf_resp.signature), SIGNATURE_SIZE_BYTES);
//	uint32_t recv_sig_stop = HAL_GetTick();
//	uint32_t recv_sig = recv_sig_stop - recv_sig_start;
	receive_resp_stop = HAL_GetTick();
	receive_resp_time += receive_resp_stop-receive_resp_start;

	verify_resp_start = HAL_GetTick();

	// Check chal is greater than prev chal
	unsigned int i;
	#if MODE == AUD
	int valid_next_chal = 1;
	for(i=0; i<CHAL_SIZE; i++){
		if(chl[i] < cfa_engine_conf.challenge[i]){
			valid_next_chal = 0;
		}
	}
	#endif

	_update_challenge(chl);

	// check verifier signature by sig = ECDSA(private_key, hash(report))
	// 1) get SHA256 hash over report (excluding buffer that holds signature)
	uint32_t response_size = CHAL_SIZE + 1 + HASH_SIZE_BYTES;
//	Hacl_SHA2_256_hash(response_hash, (uint8_t*)(&vrf_resp), response_size);
	// 2) verify signature of hash
//    curve = uECC_secp256r1();
//    int valid_sig =  uECC_verify(public_key, response_hash, HASH_SIZE_BYTES, vrf_resp.signature, curve);

//	uint32_t hmac_start = HAL_GetTick();
	#ifdef HASH_ENGINE
    HMAC_SHA_265((uint8_t*)(&vrf_resp), response_size, hash_output);
	#else
    hmac(hash_output, att_key, 32, (uint8_t*)(&vrf_resp), (uint32_t) response_size);
	#endif
//    uint32_t hmac_stop = HAL_GetTick();
//    hmac_time = hmac_stop-hmac_start;

    int valid_sig = 1;
    for(i=0; i<32; i++){
    	if(hash_output[i] != vrf_resp.signature[i]){
    		valid_sig = 0;
    	}
    }
	#if MODE == AUD
    vrf_resp.verify_result = (valid_next_chal & valid_sig);
	#else
    	vrf_resp.verify_result = valid_sig;
	#endif
    verify_resp_stop = HAL_GetTick();
    verify_resp_time += verify_resp_stop-verify_resp_start;

	return vrf_resp.verify_result;
}

void _receive_request(int size,uint8_t* read_char){
	SecureUartRx(read_char, size);
	return;
}

void _send_report_message(){
	send_report_start = HAL_GetTick();
	uint8_t init_report[] = BEGGINING_OF_REPORT;
	SecureUartTx(init_report, COMMAND_SIZE);

	// Baseline End-to-end APP
	#if MODE == SENSE_APP
	SecureUartTx((uint8_t *)(&output_data), 4);
	SecureUartTx(report_secure.signature, SIGNATURE_SIZE_BYTES);
	#else
	// CFA or TRACES
	SecureUartTx(report_secure.signature, SIGNATURE_SIZE_BYTES+2);
	int data_size = 2 + 4*(report_secure.num_CF_Log_size);
	uint8_t * report_addr = (uint8_t*)(&(report_secure.num_CF_Log_size));
	SecureUartTx(report_addr, data_size);
	#endif
	send_report_stop = HAL_GetTick();
	send_report_time += send_report_stop - send_report_start;

	// timing for debug
	SecureUartTx((uint8_t *)(&send_report_time), 4);
}

uint32_t compute_send_report_time;
uint32_t compute_send_report_start;
uint32_t compute_send_report_stop;
void _send_report(){
	_deactivate_timer_interrupt();
	report_secure.number_of_logs_sent ++;
	if (cfa_engine_conf.attestation_status == COMPLETE){
		report_secure.isFinal = FINAL_REPORT;
		compute_send_report_start = HAL_GetTick();
		_sign_report(); // must come after isFinal is set since signs all report data
		_send_report_message();
		compute_send_report_stop = HAL_GetTick();
	}
	else if(cfa_engine_conf.attestation_status == WAITING_PARTIAL){
		report_secure.isFinal = PARTIAL_REPORT;
		compute_send_report_start = HAL_GetTick();
		_sign_report();
		_send_report_message();
		report_secure.num_CF_Log_size = 0;
		compute_send_report_stop = HAL_GetTick();
	}

	compute_send_report_time = compute_send_report_stop - compute_send_report_start;
	return;
}


/*
    ---------------------------------------------------------------------------------
	----------------------------- CFA ENGINE ----------------------------------------
	---------------------------------------------------------------------------------
*/

/* ----------------------------- CRYPTO-OPERATIONS --------------------------------- */
uint32_t time_hash_memory;
uint32_t time_hash_memory_start;
uint32_t time_hash_memory_end;
void _attest_memory(){
	time_hash_memory_start = HAL_GetTick();
	// Compute sigle hash of non-secure memory
	Hacl_SHA2_256_hash(report_secure.mem_hash, (uint8_t*)(NONSECURE_FLASH_START), NONSECURE_FLASH_SIZE);
	time_hash_memory_end = HAL_GetTick();
	time_hash_memory = time_hash_memory_end-time_hash_memory_start;
}


void _sign_report(){
	time_sign_report_start = HAL_GetTick();
	// Copy from Flash to Report Struct
	#if CFLOG_TYPE == CFLOG_FLASH
	unsigned int i;
	for(i=0; i<report_secure.num_CF_Log_size; i++){
		report_secure.CFLog[i] = FLASH_CFLog[i];
	}
	#endif

	// Compute ECDSA signature by sig = ECDSA(private_key, hash(report))

	// Baseline End-to-end APP
	#if MODE == SENSE_APP
	uint32_t report_size = 4; // in bytes
	#ifdef HASH_ENGINE
	HMAC_SHA_265((uint8_t*)(&output_data), report_size, report_secure.signature);
	#else
	Hacl_SHA2_256_hash(report_hash, (uint8_t*)(&output_data), report_size);
	#endif
	#else
	// CFA or TRACES
	uint32_t report_size = 2 + HASH_SIZE_BYTES + 2 + 4*report_secure.num_CF_Log_size;
//	Hacl_SHA2_256_hash(report_hash, (uint8_t*)(&report_secure.isFinal), report_size);

	#ifdef HASH_ENGINE
	HMAC_SHA_265((uint8_t*)(&report_secure.isFinal), report_size, report_secure.signature);
	#else
	hmac(report_secure.signature, att_key, 32, (uint8_t*)(&report_secure.isFinal), (uint32_t) report_size);
	#endif

	#endif
//	if(HASH_SHA_265((uint8_t*)(&report_secure.isFinal), report_size, report_hash) != HAL_OK){
//		Error_Handler();
//	}
//
//	// sign hash
//    curve = uECC_secp256r1();
//    int t =  uECC_sign(private_key, report_hash, HASH_SIZE_BYTES, report_secure.signature, curve);

    time_sign_report_end = HAL_GetTick();
	time_sign_report += time_sign_report_end-time_sign_report_start;
}

uint8_t loop_detect = 0;
uint16_t loop_counter = 1;
uint32_t prev_entry;
uint32_t overflow = 0;
// VERBATIM STYLE
/**/
void CFA_ENGINE_new_log_entry(uint32_t value){
	if(report_secure.num_CF_Log_size >= MAX_CF_LOG_SIZE){
		end = HAL_GetTick();
		app_exec_time += end - start;
		cfa_engine_conf.attestation_status = WAITING_PARTIAL;
		report_secure.num_CF_Log_size = MAX_CF_LOG_SIZE; // might point over with loop overflow
		_send_report();

		if(overflow != 0){
			#if CFLOG_TYPE == CFLOG_RAM
			report_secure.CFLog[report_secure.num_CF_Log_size] = overflow;
			#else
			uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
			//		update_flash(addr, value);
			FLASH_CFLog[report_secure.num_CF_Log_size] = overflow;
			#endif
			overflow = 0;
			report_secure.num_CF_Log_size++;
			cfa_engine_conf.log_counter++;
		}

		#if CFLOG_TYPE == CFLOG_RAM
		report_secure.CFLog[report_secure.num_CF_Log_size] = value;
		#else
		uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
//		update_flash(addr, value);
		FLASH_CFLog[report_secure.num_CF_Log_size] = value;
		#endif

		report_secure.num_CF_Log_size++;
		cfa_engine_conf.log_counter++;
		_read_serial_loop();
		start = HAL_GetTick();
	}
	else{
//
//		if(report_secure.num_CF_Log_size == MAX_CF_LOG_SIZE)
//			loop_detect = loop_detect;

		// compare current value to previous, if equal, replace with counter
		#if CFLOG_TYPE == CFLOG_RAM
		prev_entry = report_secure.CFLog[report_secure.num_CF_Log_size - 1];
		if(report_secure.num_CF_Log_size != 0 && prev_entry == value){

		#else
		prev_entry = FLASH_CFLog[report_secure.num_CF_Log_size - 1];
		if(report_secure.num_CF_Log_size != 0 && prev_entry == value){

		#endif
			if (loop_detect == 0){
				// since first instance of repeat, set flag
				loop_detect ^= 1;
			} else if (loop_detect == 1){
				// if more than one instance, increment counter
				loop_counter++;
			}
		}
		else{ // enter this block either because 1) not a loop or 2) loop exit
			if(loop_detect == 1){
				// if loop exit, clear flag and increment log size for next entry
				#if CFLOG_TYPE == CFLOG_FLASH
				uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
				uint32_t data = 0xffff0000 + loop_counter;
//				update_flash(addr, data);
				FLASH_CFLog[report_secure.num_CF_Log_size] = data;
				#else
					report_secure.CFLog[report_secure.num_CF_Log_size] = (0xffff0000 + loop_counter);
				#endif

				loop_detect = 0;
				cfa_engine_conf.log_counter++;
				report_secure.num_CF_Log_size++;
				loop_counter = 1;

				// Need to catch case when the counter is the last entry in the cflog
				if(report_secure.num_CF_Log_size == MAX_CF_LOG_SIZE){
					overflow = value;
				}
			}

			 if (overflow == 0){
				#if CFLOG_TYPE == CFLOG_RAM
				report_secure.CFLog[report_secure.num_CF_Log_size] = value;
				#else
				uint32_t addr = (uint32_t)(&FLASH_CFLog[report_secure.num_CF_Log_size]);
	//			update_flash(addr, value);
				FLASH_CFLog[report_secure.num_CF_Log_size] = value;
				prev_entry = value;
				#endif
				cfa_engine_conf.log_counter++;
				report_secure.num_CF_Log_size++;
			}

		}
	}
	return;
}

void CFA_ENGINE_run_attestation(){
	if (cfa_engine_conf.initialized != INITIALIZED){
	//	*ERROR = ERROR_CFA_ENGINE_NOT_INITIALIZED;
		return;
	}

	report_secure.num_CF_Log_size = 0;
	cfa_engine_conf.attestation_status = ACTIVE;
	cfa_engine_conf.log_counter = 0;
	report_secure.number_of_logs_sent = 0;


//	_initialize_timer_interrupt();

	// Call the application
	start = HAL_GetTick();
	_run_application();
	end = HAL_GetTick();
	app_exec_time += end - start;

	_deactivate_timer_interrupt();

	// Set Final report Flag
	report_secure.isFinal = TRUE;
	return;
}

void CFA_ENGINE_initialize(){

	if (cfa_engine_conf.initialized == INITIALIZED){
		return; //ERROR_cfa_engine_ALREADY_INITIALIZED;;
	}

	set_hmac_key(att_key, 32);

	_attest_memory();
	_setup_data();
	_clean();

	return;
}

/* ---------------------------  ------------------------------------------------------
 * --------------------------- CFV Resolver  -----------------------------------------
 * --------------------------------------------------------------------------------- */
void _software_reset(){
	HAL_NVIC_SystemReset();
	return;
}


void _heal_function(){
	while(1);
}


#endif /* __cfa_engine */
