#How to configure the Stack and heap 

The Stack and the heap are configured in 2 different ways.

## 1: Standard memory configuration
The basic file is [startup_NRF51822.S](yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/startup_NRF51822.S), 
which should not be modified, if you do not exactly know what to do.

However in this file there are two defines, `__STACK_SIZE` and `__HEAP_SIZE` 
which can be used to modify the stack and the heap from outside this function.

To do so, the file [toolchain.cmake](yotta_targets/calliope-mini-classic-gcc/CMake/toolchain.cmake)
can be extended. Add `-D__STACK_SIZE=2632 -D__HEAP_SIZE=1464` at the end of `add_definitions("...` 
and the stack will have the size of 2632 Bytes and the heap will have the size of 1464 Bytes.
**note: The size of Stack + Heap has to be 4096 Bytes or less**

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


