#include "trustzone_config.h"

#if CONF_UROW_PROGRAMMING_ENABLE
#if defined(__GNUC__) && (__ARMCOMPILER_VERSION > 6000000) /*  Keil MDK with ARM Compiler 6 */
/* Use debug configuration file in Keil MDK to setup UROW programing */
/* const uint32_t nvm_user_row[] __attribute__((used)) __attribute__((section(".userrowsec"))) = */
const uint32_t nvm_user_row[] =
#elif defined(__GNUC__)   /* GCC */
/* "userrowsec" section should be correctly defined in your linker file */
__attribute__((section(".userrowsec"))) const uint32_t nvm_user_row[] =
#elif defined(__ICCARM__) /* IAR EWARM */
/* "nvm_user_page" section should be correctly defined in your linker file */
__root const uint32_t nvm_user_row[] @ ".nvm_user_page" =
#else
#warning "This IDE can't support USER Row fuse programming."
#endif
#else
const uint32_t nvm_user_row[] =
#endif
{
	NVM_USER_WORD_0,
	NVM_USER_WORD_1,
	NVM_USER_WORD_2,
	NVM_USER_WORD_3,
	NVM_USER_WORD_4,
	NVM_USER_WORD_5,
	NVM_USER_WORD_6,
	0 /* reserved for CRC */
};
