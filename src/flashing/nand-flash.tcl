set bootstrap    "/home/colin/Documents/apc/at91bootstrap/binaries/at91sam9g20ek-nandflashboot--3.8.10.bin"
set img          "/home/colin/Documents/apc/template-freertos/bin/img-sdram.bin"


NANDFLASH::Init
NANDFLASH::EraseAll
NANDFLASH::sendBootFile $bootstrap
send_file {NandFlash} "$img" 0x40000 0
