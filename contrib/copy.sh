#!/bin/sh

SRC=../../calliope_playbook_mini/source/
DST=`pwd`/source/

rsync -rtuv --progress $SRC $DST --exclude Main.*
#rsync -rtuv --progress $DST $SRC --exclude Main.*

SRC=../../calliope_playbook_mini/contrib/
DST=`pwd`/contrib/

rsync -rtuv --progress $SRC $DST
#rsync -rtuv --progress $DST $SRC
