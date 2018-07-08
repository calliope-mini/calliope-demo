
#!/bin/sh

ROOT="$( cd "$(dirname "$0")" ; pwd -P )"
DEVICE=/dev/cu.usbmodem1422

( stty speed 115200 cs8 1>/dev/null 2>&1; cat ) <$DEVICE
#( stty speed 115200 cs8 1>/dev/null 2>&1; hexdump -C ) <$DEVICE
