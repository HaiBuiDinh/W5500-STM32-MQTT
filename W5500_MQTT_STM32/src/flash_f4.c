#include "flash_f4.h"

pFunction Jump_To_Application;
uint32_t JumpAddress;

void runApp(void)
{
  //__disable_irq();
  JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
  Jump_To_Application = (pFunction) JumpAddress;
  /* Initialize user application's Stack Pointer */
  __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
  Jump_To_Application();
}
