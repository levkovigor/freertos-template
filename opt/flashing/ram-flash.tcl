set img          "./bin/img-ISIS_OBC_G20-at91sam9g20-sdram.bin"

GENERIC::Init $RAM::appletAddr $RAM::appletMailboxAddr $RAM::appletFileName [list $::target(comType) $::target(traceLevel) $BOARD::extRamVdd $BOARD::extRamType $BOARD::extRamDataBusWidth $BOARD::extDDRamModel]
send_file {SDRAM} "$img" 0x20000000 0
go 0x20000000
