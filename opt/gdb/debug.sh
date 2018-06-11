#!/bin/bash

img=$1

cmd=cmd.gdb

sdram_config="./opt/gdb/at91sam9g20-ek-sdram.gdb"

sram_config="./opt/gdb/at91sam9g20-ek-sram.gdb"

if echo $img | grep sdram > /dev/null
then
    config=$sdram_config
else
    config=$sram_config
fi

echo "define reset" > $cmd
echo "    target remote localhost:2331" >> $cmd
echo "    monitor reset" >> $cmd
echo "    source" $config >> $cmd
echo "    load" >> $cmd
echo "end" >> $cmd

if !(file $img | grep ELF > /dev/null)
then
    echo "ELF file is expected."
    exit
fi

exec gdb-multiarch -x cmd.gdb -ex reset $img
