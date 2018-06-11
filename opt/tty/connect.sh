#!/bin/bash

exec picocom --baud 115200 --parity n --stop 1 --flow none /dev/ttyUSB0
