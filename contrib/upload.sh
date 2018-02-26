#!/bin/sh

cp build/calliope-mini-classic-gcc/source/*-combined.hex /Volumes/MINI/

if [ -x $HOME/Shared/Manolis ]; then
  cp build/calliope-mini-classic-gcc/source/*-combined.hex $HOME/Shared/Manolis
fi
