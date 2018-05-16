
#!/bin/sh

ROOT="$( cd "$(dirname "$0")" ; pwd -P )"
DEVICE=/dev/ttyACM0

( stty speed 115200 cs8 1>/dev/null 2>&1; python2 $ROOT/memsum.py Calliope) <$DEVICE
