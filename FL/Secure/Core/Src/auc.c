#include<tim.h>
#include "main.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

void ti_syncDebbugTimer(){
	HAL_DBGMCU_EnableDBGStandbyMode();
	HAL_DBGMCU_EnableDBGStopMode();
	htim3.Instance->CNT = 0;
	htim4.Instance->CNT = 0;
	DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM3_STOP;
	DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM4_STOP;
	return;
}
