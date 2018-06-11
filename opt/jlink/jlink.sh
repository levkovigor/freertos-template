#!/bin/bash

exec /opt/SEGGER/JLink/JLinkGDBServerCLExe -USB -device AT91SAM9G20 -endian little -if JTAG -speed auto -noLocalhostOnly
