set bootstrap    "./opt/flashing/at91sam9g20ek-nandflashboot--3.8.10.bin"
set img          "./bin/img-at91sam9g20-ek-at91sam9g20-sdram.bin"


NANDFLASH::Init
NANDFLASH::EraseAll
NANDFLASH::sendBootFile $bootstrap
send_file {NandFlash} "$img" 0x40000 0
