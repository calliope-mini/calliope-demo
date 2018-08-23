# How to debug the RAM for HEAP and STACK overflow

The simple way to debug the memory usage in the RAM, is to fill it with canary 
and afterwards to dump the RAM into a file and check for the canary. 
This can be done manually, how I did it, so I will explain this method.

**The Goal is to find unused RAM space, or to see, when the STACK overflows**

## How to fill the RAM with canary

The following files have to be modified:

### [defines.json](defines.json)
Insert the following define to execute the Canary filling:
```[bash]
"DEBUG_ISR_STACK_USAGE": 1
```

### [startup_NRF51822.S](yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/startup_NRF51822.S) 

Insert the following lines between `#if` and `#endif` into the startup file.
This will make sure, the function `fill_isr_stack_with_canary()` 
is executed before the main, so the stack pointer will not be touched.

```[bash]
    LDR     R0, =SystemInit
    BLX     R0
#if DEBUG_ISR_STACK_USAGE
    LDR     R0, =fill_isr_stack_with_canary
    BLX     R0
#endif
    LDR     R0, =_start
    BX      R0
```

### [system_nrf51.c](yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/system_nrf51.c)

Insert the following code into this file, which will take care of the rest.

```[bash]
#include "compiler_abstraction.h"

// Value is sprayed into all of the ISR stack at boot time.
static const uint32_t ISR_STACK_CANARY = 0xEDC7FEAF; // AFFECTED

// Refers to linker script defined symbol, may not be available
// on all platforms.
extern uint32_t __HeapBase;
extern uint32_t __StackTop;

void fill_isr_stack_with_canary(void)
{
    uint32_t * bottom = &__HeapBase;
    uint32_t * top    = (uint32_t *) GET_SP();

    for (; bottom < top; bottom++)
    {
        *bottom = ISR_STACK_CANARY;
    }
}

```


### Canary 0xAFFEC7ED

Now the RAM will be filled with the canary AFFEC7ED before the system starts, so you can see
which regions have been touched by the application.

## Reading the RAM

To read the RAM use the following bash command:
```[bash]
nrfjprog --readram RAM-dump.txt
```
This file will affect the application, so the controller might crash, 
but it will store the complete RAM in the `RAM-dump.txt` file, which afterwards can be analyzed.

To see where the STACK and HEAP regions are in the RAM, use following command:
```[bash] 
readelf -S build/calliope-mini-classic-gcc/source/calliope-demo
```

The output then can look like this:
```[bash]
There are 22 section headers, starting at offset 0x312764:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        00018000 008000 015820 00  AX  0   0  8
  [ 2] .ARM.exidx        ARM_EXIDX       0002d820 01d820 000008 00  AL  1   0  4
  [ 3] .data             PROGBITS        20002000 022000 0000b0 00  WA  0   0  4
  [ 4] .bss              NOBITS          200020b0 0220b0 0009e0 00  WA  0   0  8
  [ 5] .heap             PROGBITS        20002a90 0220b0 000800 00      0   0  8
  [ 6] .stack_dummy      PROGBITS        20002a90 0228b0 000800 00      0   0  8
  [ 7] .ARM.attributes   ARM_ATTRIBUTES  00000000 0230b0 000028 00      0   0  1
  [ 8] .comment          PROGBITS        00000000 0230d8 00007f 01  MS  0   0  1
  [ 9] .debug_info       PROGBITS        00000000 023157 20e74f 00      0   0  1
  [10] .debug_abbrev     PROGBITS        00000000 2318a6 027ed1 00      0   0  1
  [11] .debug_loc        PROGBITS        00000000 259777 027c6e 00      0   0  1
  [12] .debug_aranges    PROGBITS        00000000 2813e8 0031c8 00      0   0  8
  [13] .debug_ranges     PROGBITS        00000000 2845b0 006820 00      0   0  1
  [14] .debug_line       PROGBITS        00000000 28add0 0362d2 00      0   0  1
  [15] .debug_str        PROGBITS        00000000 2c10a2 02f1ec 01  MS  0   0  1
  [16] .debug_frame      PROGBITS        00000000 2f0290 009554 00      0   0  4
  [17] .stab             PROGBITS        00000000 2f97e4 00003c 0c     18   0  4
  [18] .stabstr          STRTAB          00000000 2f9820 000076 00      0   0  1
  [19] .symtab           SYMTAB          00000000 2f9898 00d3d0 10     20 2278  4
  [20] .strtab           STRTAB          00000000 306c68 00ba24 00      0   0  1
  [21] .shstrtab         STRTAB          00000000 31268c 0000d8 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
```

The `.heap` and `.stack_dummy` will tell you the address range.
 
 
## More sophisticated Approach, but for MBED 5 

The following git repo will give you more information:
[mbed-memory-status](https://github.com/nuket/mbed-memory-status) 