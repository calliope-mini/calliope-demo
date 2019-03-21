cmake_minimum_required(VERSION 3.5)


#set(CMAKE_C_COMPILER_WORKS TRUE)
#set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(CMAKE_TOOLCHAIN_FILE yotta_targets/mbed-gcc/CMake/toolchain.cmake)
set(CMAKE_BUILD_TYPE Debug)

project(calliope-demo C CXX)
enable_language(ASM)

add_definitions(
        -DNRF51
        -DTARGET_NRF51
        -DTARGET_NRF51_CALLIOPE
)
include(yotta_targets/calliope-mini-classic-gcc/CMake/toolchain.cmake)


#microbit-dal
file(GLOB MICROBIT_DAL_INC
        #yotta_modules/microbit-dal
        yotta_modules/microbit-dal/inc
        yotta_modules/microbit-dal/inc/bluetooth
        yotta_modules/microbit-dal/inc/drivers
        yotta_modules/microbit-dal/inc/core
        yotta_modules/microbit-dal/inc/platform
        yotta_modules/microbit-dal/inc/types
        yotta_modules/microbit-dal/source
        )
file(GLOB_RECURSE MICROBIT_DAL_SRC
        yotta_modules/microbit-dal/source/*.cpp
        yotty_modules/microbit-dal/inc/*.h
        yotta_modules/microbit-dal/inc/bluetooth/*.h
        yotta_modules/microbit-dal/inc/drivers/*.h
        yotta_modules/microbit-dal/inc/core/*.h
        yotta_modules/microbit-dal/inc/platform/*.h
        yotta_modules/microbit-dal/inc/types/*.h
        )

add_library(microbit-dal-2 ${MICROBIT_DAL_SRC})
include_directories(${MICROBIT_DAL_INC})

#microbit
file(GLOB MICROBIT_INC
        yotta_modules/microbit
        yotta_modules/microbit/inc
        yotta_modules/microbit/source
        )
file(GLOB_RECURSE MICROBIT_SRC
        yotta_modules/microbit/source/*.cpp
        yotta_modules/microbit/inc/*.h
        )
add_library(microbit-2 ${MICROBIT_SRC})
include_directories(${MICROBIT_INC})

#mbed-classic
file(GLOB MBED_INC
        yotta_modules/mbed-classic/api
        yotta_modules/mbed-classic/hal
        yotta_modules/mbed-classic/targets/cmsis
        yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC
        yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822
        yotta_modules/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822
        yotta_modules/mbed-classic/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_NRF51_CALLIOPE
        )
file(GLOB_RECURSE MBED_SRC
        yotta_modules/mbed-classic/common/*.c
        yotta_modules/mbed-classic/api/*.h
        yotta_modules/mbed-classic/common/*.cpp
        yotta_modules/mbed-classic/hal/*.h
        yotta_modules/mbed-classic/targets/cmsis/TARGET_NORDIC/*.c
        )
add_library(mbed ${MBED_SRC})
include_directories(${MBED_INC})

#ble
file(GLOB CALLIOPE_BLE_INC
        yotta_modules/ble
        yotta_modules/ble/ble
        yotta_modules/ble/source
        )
file(GLOB_RECURSE CALLIOPE_BLE_SRC
        yotta_modules/ble/*.c*
        yotta_modules/ble/ble/*.h
        )
add_library(calliope-ble ${CALLIOPE_BLE_SRC})
include_directories(${CALLIOPE_BLE_INC})

#ble-nrf51822
file(GLOB BLE_NRF51822_INC
        yotta_modules/ble-nrf51822
        yotta_modules/ble-nrf51822/source
        yotta_modules/ble-nrf51822/source/btle
        yotta_modules/ble-nrf51822/source/common
        )
file(GLOB_RECURSE BLE_NRF51822_SRC
        yotta_modules/ble-nrf51822/*.c*
        yotta_modules/ble-nrf51822/source/common/*.h
        yotta_modules/ble-nrf51822/source/*.h
        )
add_library(ble-nrf51822-2 ${BLE_NRF51822_SRC})
include_directories(${BLE_NRF51822_INC})

#nrf51-sdk
file(GLOB NRF51_SDK_INC
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble/ble_radio_notification
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble/common
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble/device_manager
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble/device_manager/config
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble/ble_services/ble_dfu
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/ble/peer_manager
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/device
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/drivers_nrf
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/ble_flash
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/pstorage
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/drivers_nrf/pstorage/config
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/bootloader_dfu
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/bootloader_dfu/hci_transport
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/fds
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/fstorage
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/experimental_section_vars
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/crc16
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/libraries/util
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/softdevice/common/softdevice_handler
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/softdevice/s130/headers
        )
file(GLOB_RECURSE NRF51_SDK_SRC
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/*.c*
        yotta_modules/nrf51-sdk/source/nordic_sdk/components/*.h*
        )

add_library(nrf51sdk ${NRF51_SDK_SRC})
include_directories(${NRF51_SDK_INC})

#if(EXISTS source/BluetoothServiceDebug.cpp)
add_executable(calliope-demo
        source/BluetoothServiceDebug.cpp
        source/BluetoothServiceDebug.h
        source/BluetoothServiceNotify.cpp
        source/BluetoothServiceNotify.h
        source/BluetoothServiceProgram.cpp
        source/BluetoothServiceProgram.h
        source/Bytes.h
        source/Images.cpp
        source/Images.h
        source/Instruction.cpp
        source/Instruction.h
        source/Interpreter.cpp
        source/Interpreter.h
        source/Localization.h
        source/Main.cpp
        source/Menu.cpp
        source/Menu.h
        source/RunDemo.cpp
        source/RunDemo.h
        source/RunLoveMeter.cpp
        source/RunLoveMeter.h
        source/RunMultiplication.cpp
        source/RunMultiplication.h
        source/RunOracle.cpp
        source/RunOracle.h
        source/RunRockPaperScissors.cpp
        source/RunRockPaperScissors.h
        source/RunSnake.cpp
        source/RunSnake.h
        source/RunTests.cpp
        source/RunTests.h
        source/RunVolumeMeter.cpp
        source/RunVolumeMeter.h
        source/Slice.cpp
        source/Slice.h
        source/Storage.h
        source/Utils.cpp
        source/CalliopeServiceRGB.cpp
        source/CalliopeServiceLightSensor.cpp
        source/CalliopeServiceMicrophone.cpp
        source/PlaygroundFree.cpp
        source/CalliopeServiceMaster.cpp
        source/CalliopeServiceSpeaker.cpp
        source/CalliopeServiceInterpreter.cpp)
#else()
#   add_executable(calliope-demo
#          source/CalliopeTestBoard.cpp
#         source/CalliopeDemo.cpp
#        source/CalliopeDemo.h
#       source/Snake.cpp
#      )
#endif()

LINK_DIRECTORIES(build)

target_link_libraries(calliope-demo microbit-dal-2 microbit-2 nrf51sdk calliope-ble ble-nrf51822-2)
#  )


#add_executable(compass-test compass-test/source/CompassTest.cpp)
#target_link_libraries(compass-test microbit microbit-dal microbit nrf51sdk)
#
#add_executable(accelerometer-test accelerometer-test/source/AccelerometerTest.cpp)
#target_link_libraries(accelerometer-test microbit microbit-dal nrf51sdk)
#
#add_executable(encryption-test encryption-test/source/EncryptionTest.cpp)
#target_link_libraries(encryption-test microbit microbit-dal nrf51sdk)
#
#add_executable(calliope-logger calliope-logger/source/CalliopeDataLogger.cpp)
#target_link_libraries(calliope-logger microbit microbit-dal nrf51sdk)
#
#add_executable(calliope-motor-pwm-test calliope-motor-pwm-test/source/MotorPWMTest.cpp)
#target_link_libraries(calliope-motor-pwm-test microbit microbit-dal nrf51sdk)
#
#
#add_executable(calliope-noise noise/source/CalliopeTestBoard.cpp)
#target_link_libraries(calliope-noise microbit microbit-dal nrf51sdk)


#Linking CXX executable existing/calliope-demo
#FAILED: : && /usr/bin/arm-none-eabi-g++  -fno-exceptions -fno-unwind-tables -Wl,--gc-sections -Wl,--sort-common -Wl,--sort-section=alignment -Wl,-wrap,main -mcpu=cortex-m0 -mthumb -T"/media/wowa/5e81bf61-49cb-4835-81f4-f6f58647d624/mbed/calliope-demo/yotta_targets/calliope-mini-classic-gcc/CMake/../ld/NRF51822.ld" -Wl,-Map,existing/calliope-demo.map -Wl,--start-group existing/CMakeFiles/calliope-demo.dir/source/BluetoothServiceDebug.cpp.o existing/CMakeFiles/calliope-demo.dir/source/BluetoothServiceNotify.cpp.o existing/CMakeFiles/calliope-demo.dir/source/BluetoothServiceProgram.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Images.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Instruction.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Interpreter.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Main.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Menu.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunDemo.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunLoveMeter.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunMultiplication.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunOracle.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunRockPaperScissors.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunSnake.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunTests.cpp.o existing/CMakeFiles/calliope-demo.dir/source/RunVolumeMeter.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Slice.cpp.o existing/CMakeFiles/calliope-demo.dir/source/Utils.cpp.o  existing/microbit-dal-2.a existing/microbit-2.a existing/nrf51sdk.a existing/calliope-ble.a existing/ble-nrf51822-2.a  -lstdc++ -lsupc++ -lm -lc -lgcc -lstdc++ -lsupc++ -lm -lc -lgcc -Wl,--end-group  --specs=nano.specs -o existing/calliope-demo && :
#/usr/bin/../lib/gcc/arm-none-eabi/7.2.1/../../../../arm-none-eabi/bin/ld: warning: cannot find entry symbol Reset_Handler; defaulting to 0000000000018000
