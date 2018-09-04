#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <pit/pit.h>
#include <tc/tc.h>
#include <aic/aic.h>
#include <utility/led.h>
#include <utility/trace.h>
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <Drivers/I2C.h>
#include <Drivers/UART.h>

void led_blink(void *pParams){
  do{
    LED_Toggle(1);
    vTaskDelay(2000);
  }
  while(1);
}


int main(void)
{
  // DBGU output configuration
  TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
  printf("-- Getting Started Project %s --\n\r", SOFTPACK_VERSION);
  printf("-- %s\n\r", BOARD_NAME);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);

  LED_Configure(1);

  xTaskCreate(led_blink, (const signed char*) "led_blink", configMINIMAL_STACK_SIZE,
	      NULL, configMAX_PRIORITIES - 1, NULL );
  vTaskStartScheduler();
}
