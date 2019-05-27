#include "low_level_ramflash.h"

void low_level_init_ramflash() {
/* Initialize main oscillator
   ****************************/
  AT91C_BASE_PMC->PMC_MOR = BOARD_OSCOUNT | AT91C_CKGR_MOSCEN;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));
  
  /* Initialize PLLA at 200MHz (198.656) */
  AT91C_BASE_PMC->PMC_PLLAR = BOARD_CKGR_PLLA
    | BOARD_PLLACOUNT
    | BOARD_MULA
    | BOARD_DIVA;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKA));
  
  // Initialize PLLB for USB usage (if not already locked)
  if (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB)) {
    AT91C_BASE_PMC->PMC_PLLBR = BOARD_USBDIV
      | BOARD_CKGR_PLLB
      | BOARD_PLLBCOUNT
      | BOARD_MULB
      | BOARD_DIVB;
    while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCKB));
  }
  
  /* Wait for the master clock if it was already initialized */
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
  
  /* Switch to fast clock
   **********************/
  /* Switch to main oscillator + prescaler */
  AT91C_BASE_PMC->PMC_MCKR = BOARD_PRESCALER;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
  
  /* Switch to PLL + prescaler */
  AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLLA_CLK;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

}
