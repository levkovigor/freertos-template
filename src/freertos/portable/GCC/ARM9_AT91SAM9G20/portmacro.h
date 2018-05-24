/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint32_t
#define portBASE_TYPE	long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;


#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

/* Hardware specifics. */
#define portSTACK_GROWTH			( -1 )
#define portTICK_PERIOD_MS			( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			8
#define portYIELD()				      __asm volatile ( "SWI 0" )
#define portNOP()				      __asm volatile ( "NOP" )
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


/* Scheduler utilities. */

/*
 * portRESTORE_CONTEXT, portRESTORE_CONTEXT, portENTER_SWITCHING_ISR
 * and portEXIT_SWITCHING_ISR can only be called from ARM mode, but
 * are included here for efficiency.  An attempt to call one from
 * THUMB mode code will result in a compile time error.
 */

#define portRESTORE_CONTEXT()\
  {\
  extern volatile void * volatile pxCurrentTCB;\
  extern volatile uint32_t ulCriticalNesting;\
  __asm volatile (			     \
  "LDR R1, =pxCurrentTCB\n"\
  "LDR R0, [R1]\n"\
  "LDR LR, [R0]\n"\
  "LDR R0, =ulCriticalNesting\n"\
  "LDMFD LR!, {R1}\n"\
  "STR R1, [R0]\n"\
  "LDMFD LR!, {R0}\n"\
  "MSR SPSR_cxsf, R0\n"\
  "LDMFD LR, {R0-R14}^\n"\
  "NOP\n"\
  "LDR LR, [LR, #+60]\n"\
  "SUBS	PC, LR, #4\n");\
  (void) ulCriticalNesting; \
  (void) pxCurrentTCB; \
  }
  
#define portSAVE_CONTEXT() \
    {\
  extern volatile void * volatile pxCurrentTCB;\
  extern volatile uint32_t ulCriticalNesting;\
  __asm volatile (			     \
  "STMDB	SP!, {R0}\n"\
  "STMDB	SP, {SP}^\n"\
  "NOP\n"\
  "SUB		SP, SP, #4\n"\
  "LDMIA	SP!, {R0}\n"\
  "STMDB	R0!, {LR}\n"\
  "MOV		LR, R0\n"\
  "LDMIA	SP!, {R0}\n"\
  "STMDB	LR, {R0-LR}^\n"\
  "NOP\n"\
  "SUB		LR, LR, #60\n"\
  "MRS		R0, SPSR\n"\
  "STMDB	LR!, {R0}\n"\
  "LDR		R0, =ulCriticalNesting\n"\
  "LDR		R0, [R0]\n"\
  "STMDB	LR!, {R0}\n"\
  "LDR		R1, =pxCurrentTCB\n"\
  "LDR		R0, [R1]\n"\
  "STR		LR, [R0]\n");\
  (void) ulCriticalNesting;  \
  (void) pxCurrentTCB;	     \
    }

  
/* Critical section handling. */

#ifdef THUMB_INTERWORK

  extern void vPortDisableInterruptsFromThumb( void ) __attribute__ ((naked));
  extern void vPortEnableInterruptsFromThumb( void ) __attribute__ ((naked));
#define portDISABLE_INTERRUPTS() vPortDisableInterruptsFromThumb()
#define portENABLE_INTERRUPTS() vPortEnableInterruptsFromThumb()

#else

#define portDISABLE_INTERRUPTS()\
  __asm volatile (\
  "STMDB SP!, {R0}\n"\
  "MRS R0, CPSR\n"\
  "ORR R0, R0, #0xC0\n"\
  "MSR CPSR, R0\n"\
  "LDMIA SP!, {R0}\n")

#define portENABLE_INTERRUPTS()\
  __asm volatile (\
  "STMDB SP!, {R0}\n"\
  "MRS R0, CPSR\n"\
  "BIC R0, R0, #0xC0\n"\
  "MSR CPSR, R0\n"\
  "LDMIA SP!, {R0}\n")

#endif
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portENTER_CRITICAL()		vPortEnterCritical()
#define portEXIT_CRITICAL()			vPortExitCritical()

/*-----------------------------------------------------------*/
  
/* Task utilities. */
#define portEND_SWITCHING_ISR( xSwitchRequired ) 	\
{													\
extern void vTaskSwitchContext( void );				\
													\
	if( xSwitchRequired ) 							\
	{												\
		vTaskSwitchContext();						\
	}												\
}
/*-----------------------------------------------------------*/


/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void * pvParameters )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */


