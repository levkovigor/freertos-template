#include <board.h>
#include <cp15/cp15.h>
#include <pmc/pmc.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <pit/pit.h>
#include <tc/tc.h>
#include <aic/aic.h>
#include <utility/led.h>
#include <utility/trace.h>
#include <twi/twi.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/projdefs.h>

#include <hal/boolean.h>
#include <hal/version/version.h>

#include <hal/Drivers/UART.h>
#include <hal/interruptPriorities.h>
#include <hal/boolean.h>
#include <hal/Utility/util.h>
#include "I2Ctest.h"

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

const Pin twi_pins = PINS_TWI;

Twid twid;

void led_blink(void *pParams){
//WDT_startWatchdogKickTask(10 / portTICK_RATE_MS, FALSE);
  I2Ctest();
  do{
    LED_Toggle(1);
    vTaskDelay(2000);
  }
  while(1);
}


int main(void)
{
  // DBGU output configuration
  TRACE_CONFIGURE_ISP(DBGU_STANDARD, 115200, BOARD_MCK);
  CP15_EnableIcache();

  printf("-- Getting Started Project %s --\n\r", SOFTPACK_VERSION);
  printf("-- %s\n\r", BOARD_NAME);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  
  PMC_EnablePeripheral(AT91C_ID_TWI);
  PIO_Configure(&twi_pins,1);
  TWI_ConfigureMaster(AT91C_BASE_TWI,100000,BOARD_MCK);
  twid.pTwi = AT91C_BASE_TWI;
  twid.pTwi = NULL;
  TWID_Initialize(&twid, AT91C_BASE_TWI);
  TWID_Write(&twid, 0x22, 0x0, 0x0, (unsigned char *) "hello\n\r",7,NULL);

  
  LED_Configure(1);

  xTaskCreate(led_blink, (const signed char*) "led_blink", configMINIMAL_STACK_SIZE,
	      NULL, configMAX_PRIORITIES - 1, NULL );
  vTaskStartScheduler();
}
