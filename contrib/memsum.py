import re
import sys
import time


mem = {}
allocated = 0

reset = None
if len(sys.argv) > 1:
    reset = sys.argv[1]
    print("RESETTING tracer on '%s'" % reset)

r_malloc = re.compile("^(microbit_)malloc:\\s+(NATIVE\\s+)?(ALLOCATED:)\\s+(\\d+)\\s+\\[(0x[0-9a-f]+)\\]")
r_free = re.compile("^(microbit_)free:\\s+(0x[0-9a-f]+)")

partial = ""
#broken in python2
#for line in sys.stdin:
for line in iter(sys.stdin.readline, ''):
    # we sometimes get incomplete lines, wait for a full line
    if not (line[-1] == '\n' or line[-1] == '\r'):
        partial = line
        continue
    else:
        line = partial + line
        partial = ""

    # strip newline and carriage return
    line = line.rstrip('\n').rstrip('\r')

    # if we detect the reset keyword, rest the map and memory counter
    if reset != None and reset in line:
        mem = {}
        allocated = 0
        # print("\n\n\033[91m>> RESET >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\033[0m")
        print("\033[H\033[J")

    # match malloc, realloc and free
    m = r_malloc.search(line)
    if m:
        mem[m.group(5)] = int(m.group(4))
        allocated += int(m.group(4))
        print("\033[1m== (%03d) \033[34m%8d\033[0m [%8x] \033[31m+%-6d\033[0m (%s)" % \
              (len(mem), allocated, allocated, int(m.group(4)),
               m.group(0).replace(m.group(1), "").replace(m.group(3), "")))
        continue

    m = r_free.search(line)
    if m:
        # print "f", m.group(3)
        freed = 0
        if mem.has_key(m.group(2)):
            freed = mem[m.group(2)]
            allocated -= freed
            del mem[m.group(2)]
        else:
            print ("\033[33m!! WARN: %s\033[0m" % (line))
        print ("\033[1m== (%03d) \033[34m%8d\033[0m [%8x] \033[92m-%-6d\033[0m (%s)" % \
              (len(mem), allocated, allocated, freed, m.group(0).replace(m.group(1), "")))
        continue

    # print all other lines as is, so we can still use the log functionality
    print(line)
    sys.stdout.flush()
