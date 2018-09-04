#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <utility/trace.h>
#include <utility/assert.h>


#define configUSE_PREEMPTION                    0
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      ( ( unsigned long ) BOARD_MCK )
#define configTICK_RATE_HZ                      ( (portTickType) 1000 )
#define configMAX_PRIORITIES                    ( (unsigned portBASE_TYPE) 5 )
#define configMINIMAL_STACK_SIZE                ( (unsigned short) 190 )
#define configTOTAL_HEAP_SIZE                   ( (size_t) (40000) )
#define configMAX_TASK_NAME_LEN                 ( 32 )
#define configUSE_TRACE_FACILITY                0
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 0
#define configUSE_MUTEXES                       1
#define configCHECK_STACK_OVERFLOW              1
#define configUSE_RECURSIVE_MUTEXES        	1
#define configQUEUE_REGISTRY_SIZE		32
#define configUSE_COUNTING_SEMAPHORES	        1
  
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

#define configASSERT( x )  SANITY_CHECK( ( x ) )

#define configUSE_APPLICATION_TASK_TAG          1

#define configGENERATE_RUN_TIME_STATS           1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1


#define INCLUDE_vTaskPrioritySet	        1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete			1
#define INCLUDE_vTaskCleanUpResources	        0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay			1
#define INCLUDE_xTaskResumeFromISR		1
#define INCLUDE_xTaskGetCurrentTaskHandle	1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xTaskGetSchedulerState		1
#define INCLUDE_uxTaskGetStackHighWaterMark     1

#define configYIELD_INTERRUPT_VECTOR		16UL
#define configKERNEL_INTERRUPT_PRIORITY		1
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	4

#endif
