#include <pio/pio.h>
#include <utility/led.h>
#include <utility/trace.h>
#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>

/* type nom_de_fonction (type2 param1, type3 param2) { */
/* } */

typedef struct structure_struct {
  int val1;
} structure_type;


structure_type inc(structure_type x) {
  structure_type a;
  a.val1 = x.val1+1;
  return a;
}

int adds_one (int x) {
  return x++;
}

int times_two (int x) {
  return (x * 2);
}

int times_two_plus_one (int x){
  return (adds_one(times_two(x)));
}


int test_times_two (int input , int expected){
  int x = times_two (input);
  return (x == expected);
}

/* int many_test (void) { */
/*   printf("many test"); */
/*   int i; */
/*   for (i=0; i<10; i++){ */
/*     int x = i * 2; */
/*     if (test_times_two(i, x)){ */
/*       printf("Test succeeded with %d\n\r", i); */
/*     }else{ */
/*       printf("Test didn't succeed with %d\n\r", i); */
/*       return 0; */
/*     } */
/*   } */
/*   return 1; */
/* } */


void task1(void *pParam) {

	int i = 0;
	while(1) {
		i++;
	        LED_Set(1);
		vTaskDelay(200);
	}
}

void task2(void *pParam) {

	int i = 0;
	while(1) {
		i++;
		vTaskDelay(100);
		LED_Clear(1);
		vTaskDelay(100);
	}
}

void ConfigureLeds (void) {
    LED_Configure(1);
}

static void prvSetupHardware (void) {
  AT91C_BASE_AIC->AIC_EOICR = 0;
  ConfigureLeds();
}

int main () {
  prvSetupHardware();
  LED_Set(1);
  
  /* TRACE_CONFIGURE(DBGU_STANDARD, 115200, BOARD_MCK); */
  /* printf("-- Template project %s --\n\r", SOFTPACK_VERSION); */
  /* printf("-- %s\n\r", BOARD_NAME); */
  /* printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__); */
  /* printf("--Testish\n\r"); */
  /* int x = many_test(); */
  /* printf("--Testouch %d \n\r", x); */

  /* xTaskCreate(task1, "LED_0", 128, NULL, 0, NULL); */
  /* xTaskCreate(task2, "LED_1", 128, NULL, 0, NULL); */
  vTaskStartScheduler();

  /*
   *	We should never get here, but just in case something goes wrong,
   *	we'll place the CPU into a safe loop.
   */
  while (1) {
  }
}
