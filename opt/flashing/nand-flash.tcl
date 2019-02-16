set bootstrap    "./opt/flashing/at91sam9g20ek-nandflashboot--3.8.10.bin"
set img          "./bin/img-ISIS_OBC_G20-at91sam9g20-sdram.bin"


NANDFLASH::Init
NANDFLASH::EraseAll
NANDFLASH::sendBootFile $bootstrap
send_file {NandFlash} "$img" 0x40000 0
