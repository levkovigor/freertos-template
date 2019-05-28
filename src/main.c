#include <at91/utility/trace.h>
#include <at91/peripherals/cp15/cp15.h>
#include <at91/utility/exithandler.h>
#include <at91/commons.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/WatchDogTimer.h>
#include <hal/Drivers/LED.h>
#include <hal/boolean.h>
#include <hal/Utility/util.h>

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define FLASH_RAM

#ifdef FLASH_RAM
#include "low_level_ramflash.h"
#endif



#define ENABLE_MAIN_TRACES 1
#if ENABLE_MAIN_TRACES
#define MAIN_TRACE_INFO			TRACE_INFO
#define MAIN_TRACE_DEBUG		TRACE_DEBUG
#define MAIN_TRACE_WARNING		TRACE_WARNING
#define MAIN_TRACE_ERROR		TRACE_ERROR
#define MAIN_TRACE_FATAL		TRACE_FATAL
#else
#define MAIN_TRACE_INFO(...)	{ }
#define MAIN_TRACE_DEBUG(...)	{ }
#define MAIN_TRACE_WARNING(...)	{ }
#define MAIN_TRACE_ERROR		TRACE_ERROR
#define MAIN_TRACE_FATAL		TRACE_FATAL
#endif

void task (void * args) {
  
  configASSERT(args==NULL);
  
  LED_glow (led_1);
  
  do {
    LED_toggle(led_1);
    printf("This is the main task\n\r");
    vTaskDelay(2000);
  } while(1);

}


int main(void)
{

#ifdef FLASH_RAM
  low_level_init_ramflash();
#endif
  
  TRACE_CONFIGURE_ISP(DBGU_STANDARD, 115200, BOARD_MCK);
  
  CP15_Enable_I_Cache();

  printf("-- ISIS Template Project %s --\n\r", SOFTPACK_VERSION);
  printf("-- %s\n\r", BOARD_NAME);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  WDT_startWatchdogKickTask(10 / portTICK_RATE_MS, FALSE);

  LED_start();

  xTaskCreate(task,
	      (const signed char*) "task", configMINIMAL_STACK_SIZE,
	      NULL, configMAX_PRIORITIES - 1, NULL );

  vTaskStartScheduler();
  
}
