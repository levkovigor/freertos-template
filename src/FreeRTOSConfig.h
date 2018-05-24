#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configCPU_CLOCK_HZ                      ( ( unsigned long ) BOARD_MCK )
#define configTICK_RATE_HZ                      ( (TickType_t) 1000 )
#define configUSE_PREEMPTION                    1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configMAX_PRIORITIES                    ( 6 )
#define configMINIMAL_STACK_SIZE                ( (unsigned short) 128 )
#define configTOTAL_HEAP_SIZE                   ( (size_t) (42 * 1024) )
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configUSE_TRACE_FACILITY                0
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 0
#define configUSE_APPLICATION_TASK_TAG          1

#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

#define INCLUDE_vTaskPrioritySet	        1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete			0
#define INCLUDE_vTaskCleanUpResources	        0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay			1

/* #define configYIELD_INTERRUPT_VECTOR		16UL */
/* #define configKERNEL_INTERRUPT_PRIORITY		1 */
/* #define configMAX_SYSCALL_INTERRUPT_PRIORITY 	4 */

/* void vApplicationSetupInterrupts( void ); */

#endif
