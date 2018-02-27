#!/bin/sh

SRC=../../calliope_playbook_mini/source/
DST=`pwd`/source/

rsync -rtuv --progress $SRC $DST \
  --exclude Main.*
