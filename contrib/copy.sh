#!/bin/sh

SRC=../../calliope_playbook_mini/source/
DST=`pwd`/source/

rsync -rtuv --progress $SRC $DST --exclude Main.*
# rsync -rtuv --progress $DST $SRC --exclude Main.* --exclude Run*.* --exclude Localization.* --exclude Utils*.* --exclude Menu.* --exclude Storage.*

SRC=../../calliope_playbook_mini/contrib/
DST=`pwd`/contrib/

# rsync -rtuv --progress $SRC $DST
# rsync -n -rtuv --progress $DST $SRC
