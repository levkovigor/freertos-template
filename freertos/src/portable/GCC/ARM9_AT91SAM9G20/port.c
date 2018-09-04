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

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the Atmel ARM7 port.
 *----------------------------------------------------------*/


/* Standard includes. */
#include <stdlib.h>

/* Scheduler includes. */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Hardware includes. */
#include <commons.h>
#include <board.h>
#include <pit/pit.h>
#include <irq/irq.h>
#include <portmacro.h>
/*-----------------------------------------------------------*/

/* Constants required to setup the initial stack. */
#define portINITIAL_SPSR                        ((StackType_t) 0x1f)
/* System mode, ARM mode, interrupts enabled. */
#define portTHUMB_MODE_BIT          		((StackType_t) 0x20)
#define portINSTRUCTION_SIZE			((StackType_t) 4)

/* Constants required to setup the PIT. */
#define portPIT_PERIOD                              1000

/* Constants required to handle critical sections. */
#define portNO_CRITICAL_NESTING 		((StackType_t) 0)


#define portINT_LEVEL_SENSITIVE                 0
#define portPIT_ENABLE                  	((uint16_t) 0x1 << 24)
#define portPIT_INT_ENABLE              	((uint16_t) 0x1 << 25)
/*-----------------------------------------------------------*/

/* Setup the PIT to generate the tick interrupts. */
static void prvSetupTimerInterrupt( void );

/*-----------------------------------------------------------*/

/*
 * Initialise the stack of a task to look exactly as if a call to
 * portSAVE_CONTEXT had been called.
 *
 * See header file for description.
 */
portSTACK_TYPE *pxPortInitialiseStack
(portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters) {

  portSTACK_TYPE *pxOriginalTOS;

  pxOriginalTOS = pxTopOfStack;

  /* To ensure asserts in tasks.c don't fail, although in this case the assert
     is not really required. */
  pxTopOfStack--;
  
  /* Setup the initial stack of the task.  The stack is set exactly as
     expected by the portRESTORE_CONTEXT() macro. */

  /* First on the stack is the return address - which in this case is the
     start of the task.  The offset is added to make the return address appear
     as it would within an IRQ ISR. */
  *pxTopOfStack = ( portSTACK_TYPE ) pxCode + portINSTRUCTION_SIZE;		
  pxTopOfStack--;
  
  *pxTopOfStack = ( portSTACK_TYPE ) 0x00000000;	/* R14 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) pxOriginalTOS; /* Stack used when task starts goes in R13. */
  pxTopOfStack--;
  *pxTopOfStack = ( portSTACK_TYPE ) 0x12121212;	/* R12 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x11111111;	/* R11 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x10101010;	/* R10 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x09090909;	/* R9 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x08080808;	/* R8 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x07070707;	/* R7 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x06060606;	/* R6 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x05050505;	/* R5 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x04040404;	/* R4 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x03030303;	/* R3 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x02020202;	/* R2 */
  pxTopOfStack--;	
  *pxTopOfStack = ( portSTACK_TYPE ) 0x01010101;	/* R1 */
  pxTopOfStack--;	

  /* When the task starts is will expect to find the function parameter in
     R0. */
  *pxTopOfStack = ( portSTACK_TYPE ) pvParameters; /* R0 */
  pxTopOfStack--;

  /* The status register is set for system mode, with interrupts enabled. */
  *pxTopOfStack = ( portSTACK_TYPE ) portINITIAL_SPSR;
	
#ifdef THUMB_INTERWORK
  {
    /* We want the task to start in thumb mode. */
    *pxTopOfStack |= portTHUMB_MODE_BIT;
  }
#endif
	
  pxTopOfStack--;

  /* Interrupt flags cannot always be stored on the stack and will
     instead be stored in a variable, which is then saved as part of the
     tasks context. */
  *pxTopOfStack = portNO_CRITICAL_NESTING;

  return pxTopOfStack;	
}
/*-----------------------------------------------------------*/

portBASE_TYPE xPortStartScheduler( void )
{
  extern void vPortStartFirstTask( void );

  /* Start the timer that generates the tick ISR.  Interrupts are disabled
     here already. */
  prvSetupTimerInterrupt();

  /* Start the first task. */
  vPortStartFirstTask();	

  /* Should not get here! */
  return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
  /* It is unlikely that the ARM port will require this function as there
     is nothing to return to.  */
}
/*-----------------------------------------------------------*/

static void prvSetupTimerInterrupt( void )
{
  extern void vPortTickISR (void);
    // Initialize the PIT to the desired frequency
    PIT_Init(portPIT_PERIOD, BOARD_MCK / 1000000);

    // Configure interrupt on PIT
    IRQ_DisableIT(AT91C_ID_SYS);
    IRQ_ConfigureIT(AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, vPortTickISR);
    IRQ_EnableIT(AT91C_ID_SYS);
    PIT_EnableIT();

    // Enable the pit
    PIT_Enable();
}
/*-----------------------------------------------------------*/

volatile unsigned int portTimestamp = 0;

void TC0_IrqHandler(void)
{
    volatile unsigned int dummy;
    // Clear status bit to acknowledge interrupt
    dummy = AT91C_BASE_TC0->TC_SR;
    dummy;

    portTimestamp++;
}

void prvConfigugre_Timer_For_Run_Time_Stats(void){
  unsigned int div;
  unsigned int tcclks;
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_TC0;
  TC_FindMckDivisor(100000, BOARD_MCK, &div, &tcclks);
  TC_Configure(AT91C_BASE_TC0, tcclks | AT91C_TC_CPCTRG);
  AT91C_BASE_TC0->TC_RC = (BOARD_MCK / div) / 100000;
  IRQ_ConfigureIT(AT91C_ID_TC0, 0, TC0_IrqHandler);
  AT91C_BASE_TC0->TC_IER = AT91C_TC_CPCS;
  IRQ_EnableIT(AT91C_ID_TC0);
  TC_Start(AT91C_BASE_TC0);
}

unsigned int getTimeStamp(void){
  return portTimestamp;
}