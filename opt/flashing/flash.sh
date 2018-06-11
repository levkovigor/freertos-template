#! /bin/sh

exec sam-ba /dev/ttyACM0 at91sam9g20-ek src/flashing/$1-flash.tcl > flash.log 2>&1

true
