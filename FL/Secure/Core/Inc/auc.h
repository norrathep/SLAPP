#ifndef INC_AUC_C_
#define INC_AUC_C_

#define TIMER_INTERRUPT						htim3

#define ti_set_prescaler(htim, value)       htim.Init.Prescaler = value;\
											HAL_TIM_Base_Init(&htim);

#define ti_start_timer_interrupt(htim) 		HAL_TIM_Base_Start(&htim);\
									   		HAL_TIM_Base_Start_IT(&htim)

#define ti_stop_timer_interrupt(htim) 		HAL_TIM_Base_Stop(&htim);\
											HAL_TIM_Base_Stop_IT(&htim)

#define ti_set_timer_counter(htim,val) 		htim.Instance->CNT = val

#define ti_reset_timer_counter(htim) 		htim.Instance->CNT = 0

#define ti_set_period(htim,val) 			htim.Instance->ARR = val % 65535

//#define ti_set_prescaler(htim,val) 			htim.Instance->Prescaler = val

void ti_syncDebbugTimer();


#endif /* INC_AUC_C_ */
