#How to configure the Stack and heap 

The Stack and the heap are configured in 2 different ways.

## 1: Standard memory configuration
The basic file is [startup_NRF51822.S](yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/startup_NRF51822.S), 
which should not be modified, if you do not exactly know what to do.

However in this file there are two defines, `__STACK_SIZE` and `__HEAP_SIZE` 
which can be used to modify the stack and the heap from outside this function.

### 1.1: Modifying toolchain.cmake
One option to do so, is to extend the file [toolchain.cmake](yotta_targets/calliope-mini-classic-gcc/CMake/toolchain.cmake). Add `-D__STACK_SIZE=2632 -D__HEAP_SIZE=1464` at the end of `add_definitions("...` 
and the stack will have the size of 2632 Bytes and the heap will have the size of 1464 Bytes.

### 1.2: Adding defines.json 
The alternative option is to add a [defines.json](defines.json) file to the same directory, where [module.json](module.json) is located.
Therein macro definitions can be added for the application. In this case the file would look like this:
```[bash]
{
  "__STACK_SIZE": 2632,
  "__HEAP_SIZE": 1464
}
```
The advantage of this option is, that you do not have to modify any dependencies, but can add extra definitions to the application.
The disadvantage is, that these definitions will apply to all application code, which can lead multiple definitions.

**NOTE: The size of Stack + Heap has to fit into the RAM**

#### 1.2.1 How much space for Heap and Stack is in the RAM

To find out how much space is in the RAM, you have to read the [calliope-demo.elf](build/calliope-mini-classic-gcc/source/calliope-demo)
file. Therefore enter the following command in the terminal:
```[bash]
$ readelf -S calliope-demo
```
The output will look like this:
```[bash]
There are 22 section headers, starting at offset 0x326b10:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        00018000 008000 016494 00  AX  0   0  8
  [ 2] .ARM.exidx        ARM_EXIDX       0002e494 01e494 000008 00  AL  1   0  4
  [ 3] .data             PROGBITS        20002000 022000 0000fc 00  WA  0   0  4
  [ 4] .bss              NOBITS          20002100 022100 000a20 00  WA  0   0  8
  [ 5] .heap             PROGBITS        20002b20 022100 000acc 00      0   0  8
  [ 6] .stack_dummy      PROGBITS        20002b20 022bd0 000a00 00      0   0  8
  [ 7] .ARM.attributes   ARM_ATTRIBUTES  00000000 0235d0 000028 00      0   0  1
  [ 8] .comment          PROGBITS        00000000 0235f8 00007e 01  MS  0   0  1
  [ 9] .debug_info       PROGBITS        00000000 023676 220d76 00      0   0  1
  [10] .debug_abbrev     PROGBITS        00000000 2443ec 028b2e 00      0   0  1
  [11] .debug_loc        PROGBITS        00000000 26cf1a 02800e 00      0   0  1
  [12] .debug_aranges    PROGBITS        00000000 294f28 003268 00      0   0  8
  [13] .debug_ranges     PROGBITS        00000000 298190 006958 00      0   0  1
  [14] .debug_line       PROGBITS        00000000 29eae8 035e48 00      0   0  1
  [15] .debug_str        PROGBITS        00000000 2d4930 02f121 01  MS  0   0  1
  [16] .debug_frame      PROGBITS        00000000 303a54 009790 00      0   0  4
  [17] .stab             PROGBITS        00000000 30d1e4 00003c 0c     18   0  4
  [18] .stabstr          STRTAB          00000000 30d220 000076 00      0   0  1
  [19] .symtab           SYMTAB          00000000 30d298 00dac0 10     20 2352  4
  [20] .strtab           STRTAB          00000000 31ad58 00bcde 00      0   0  1
  [21] .shstrtab         STRTAB          00000000 326a36 0000d8 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
```
The important number here is at `[ 5] .heap             PROGBITS        20002b20`, 
which is the hexadecimal address = **0x2002b20** where the **Heap starts**. Since the **Stack ends** at the address **0x20004000**, 
the space in beetween can be used for Heap and Stack and defined in [defines.json](defines.json) and [config.json](config.json)

**Important for config.json:**
The following settings have to be set:


```[bash]
{
 ...
    "debug": 0,
    "heap_debug": 0,
    "reuse_sd": 0,
    "heap_allocator": 0,
    "stack_size": 2560,
  }
}
```
where `stack_size` has to be the same as `__STACK_SIZE` 




# !!!DEPRECATED!!!

## 2: MicroBit custom heap configuration
Since the [microbit-dal](yotta_modules/microbit-dal) 
has a custom heap allocator [MicroBitHeapAllocator.cpp](yotta_modules/microbit-dal/source/core/MicroBitHeapAllocator.cpp)
and [MicroBitHeapAllocator.h](yotta_modules/microbit-dal/inc/core/MicroBitHeapAllocator.h),
which is configured from [config.json](config.json), this file has to be modified to fit the standard configuration,
by adding
 
```[bash]
"microbit-dal": {
    ...
    "nested_heap_proportion": 0.6,
    "stack_size": 2632,
    ...
    }
```

### 2.1: Calculation of the custom heap configuration
(this chapter is based on my findings and has to be approved by the people who made coded the microbit-dal)

Currently the custom heap assumes that Heap + Stack = 4672 Bytes.
Unfortunatelly this seems to be not the case.

Running the [memsum.py](contrib/memsum.py) script with debug and hepa-debug enabled, 
it shows that the custom heap starts from `0x200030F0` and from [MicroBitConfig.h](yotta_modules/microbit-dal/inc/core/MicroBitConfig.h)
we know that the Stack ends at `0x20004000` which is the upper bound of the RAM. 

This means the real space for Heap + Stack is 3856 Bytes.

During testing, I found out, that whenever the Heap goes out of bounds, the controller crashes. 
Also the overlapping of the stack and the Heap region brings the controller to crash sometimes.

The calculation follows:

`heap_size = (4672 - stack_size) * nested_heap_proportion`

However with the given knowledge the calculation should be:
`nested_heap_proportion' = (stack_size - 3856) / (stack_size - 4672)`
in which case the stack and the nested heap will not overlap.


## Further remarks
The option `"reuse_sd": 0` has to be 0 if BLE is used, otherwise heap will be allocated in the Softdevice RAM region, 
for BLE nad thus this region will be overriden by the Softdevice and caus the controller to crash.


