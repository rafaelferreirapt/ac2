#!/bin/bash
pcompile prog.c ../send2displays.c ../delay.c ../tobcd.c
echo "compiled"

sleep 1
ldpic32 -w prog.hex
echo "loaded"

sleep 1
pterm