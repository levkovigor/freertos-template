#include <board.h>
#include <cp15/cp15.h>
#include <pmc/pmc.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <pit/pit.h>
#include <tc/tc.h>
#include <aic/aic.h>
#include <utility/trace.h>
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/projdefs.h>

#include <hal/boolean.h>
#include <hal/version/version.h>

#include <hal/Drivers/LED.h>
#include <hal/interruptPriorities.h>
#include <hal/boolean.h>
#include <hal/Utility/util.h>
#include <hal/Timing/WatchDogTimer.h>
#include "checksumTest.h"

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


const signed char * const task_name_led_blink =
  (const signed char * const ) "led_blink";

void *task_led_blink(){
  WDT_startWatchdogKickTask(10 / portTICK_RATE_MS, FALSE);
  checksumTest();
  LED_start();
  LED_glow(led_1);
  do{
    printf("Test\n\r");
    LED_toggle(led_1);
    vTaskDelay(2000);
  }
  while(1);
  return NULL;
}


int main(void)
{
  // DBGU output configuration
  TRACE_CONFIGURE_ISP(DBGU_STANDARD, 115200, BOARD_MCK);
  CP15_Enable_I_Cache();

  printf("-- ISIS Template Project %s --\n\r", SOFTPACK_VERSION);
  printf("-- %s\n\r", BOARD_NAME);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  WDT_start();
  
  xTaskCreate(task_led_blink, task_name_led_blink, configMINIMAL_STACK_SIZE,
	      NULL, configMAX_PRIORITIES - 1, NULL );
  vTaskStartScheduler();
}
