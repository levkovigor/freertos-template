#include <board.h>
#include <pio/pio.h>
#include <pio/pio_it.h>
#include <pit/pit.h>
#include <tc/tc.h>
#include <irq/irq.h>
#include <utility/led.h>
#include <utility/trace.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>

void task1(void *pParam) {
	int i = 0;
	while(1) {
		i++;
	        LED_Toggle(1);
		vTaskDelay(500);
	}
}


void ConfigureLeds (void) {
  LED_Configure(1);
}

int main () {
  TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK);
  printf("-- Template project %s --\n\r", SOFTPACK_VERSION);
  printf("-- %s\n\r", BOARD_NAME);
  printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
  ConfigureLeds();
  LED_Set(1);
  
  xTaskCreate(task1, "TASK_1", 128, NULL, 0, NULL);
  vTaskStartScheduler();
  return 1;
}
