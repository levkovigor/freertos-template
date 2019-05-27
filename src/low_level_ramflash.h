#ifndef __LOW_LEVEL_RAMFLASH

#include <at91/peripherals/pmc/pmc.h>
#include <at91/utility/trace.h>
#include <at91/peripherals/cp15/cp15.h>
#include <at91/utility/exithandler.h>
#include <at91/commons.h>

#define __LOW_LEVEL_RAMFLASH

#define BOARD_OSCOUNT           (AT91C_CKGR_OSCOUNT & (64 << 8))
#define BOARD_CKGR_PLLA         (AT91C_CKGR_SRCA | AT91C_CKGR_OUTA_0)
#define BOARD_PLLACOUNT         (0x3F << 8)
#define BOARD_MULA              (AT91C_CKGR_MULA & (0x2A << 16))
#define BOARD_DIVA              (AT91C_CKGR_DIVA & 1)
#define BOARD_PRESCALER         ((BOARD_PRES<<2) | (BOARD_MDIV<<8) | (BOARD_PDIV<<12))

#define BOARD_USBDIV            AT91C_CKGR_USBDIV_1
#define BOARD_CKGR_PLLB         AT91C_CKGR_OUTB_0
#define BOARD_PLLBCOUNT         BOARD_PLLACOUNT
#define BOARD_MULB              (25 << 16)
#define BOARD_DIVB              5

void low_level_init_ramflash();

#endif
