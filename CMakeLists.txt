cmake_minimum_required(VERSION 3.15)
# Project name
project("Blink")

# Product filename
set(PRODUCT_NAME Sensored-Motor)

# AVR Chip Configuration
set(F_CPU 16000000UL)
# CPU, you can find the list here:
set(MCU atmega2560)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUD 115200)
# The programmer to use, read avrdude manual for list
set(PROG_TYPE wiring)

set(CMAKE_EXPORT_COMPILE_COMMANDS)

# AVR Fuses, must be in concordance with your hardware and F_CPU
# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p
set(E_FUSE 0xfd)
set(H_FUSE 0xda)
set(L_FUSE 0xfd)
set(LOCK_BIT 0xff)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

# Pass defines to compiler
add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD})
# mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

file(COPY ${CMAKE_SOURCE_DIR}/link DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/link)

add_compile_options(
  -mmcu=${MCU}
  -std=gnu99
  -Os
  -Wall
  -Werror
  -funsigned-char
  -ffunction-sections
  -fdata-sections)

add_link_options(
  -mmcu=${MCU}
  -nostdlib
  -nostartfiles
  -nodefaultlibs
  -Wl,-Map=${PRODUCT_NAME}.map
  -Wl,--gc-sections
  -T${CMAKE_SOURCE_DIR}/link/init.ld)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c") # Load all files in src
                                                   # folder
file(GLOB ASM_FILES "${CMAKE_SOURCE_DIR}/src/*.S")
include_directories(include)

# Create one target
add_executable(${PRODUCT_NAME} ${SRC_FILES} ${ASM_FILES})

# Rename the output to .elf as we will create multiple files
set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME
                                                 ${PRODUCT_NAME}.elf)

# Strip binary for upload add_custom_target( strip ALL avr-strip
# ${PRODUCT_NAME}.elf DEPENDS ${PRODUCT_NAME})

# Transform binary into hex file, we ignore the eeprom segments in the step
add_custom_target(
  hex ALL
  avr-objcopy
  -R
  .eeprom
  -O
  ihex
  ${PRODUCT_NAME}.elf
  ${PRODUCT_NAME}.hex)
# DEPENDS strip) Transform binary into hex file, this is the eeprom part (empty
# if you don't use eeprom static variables) add_custom_target(eeprom avr-objcopy
# -j .eeprom --set-section-flags=.eeprom="alloc,load"  --change-section-lma
# .eeprom=0 -O ihex ${PRODUCT_NAME}.elf ${PRODUCT_NAME}.eep DEPENDS strip)

# Upload the firmware with avrdude
add_custom_target(
  upload
  avrdude
  -c
  ${PROG_TYPE}
  -p
  ${MCU}
  -P
  /dev/ttyACM0
  -b
  ${BAUD}
  -D
  -U
  flash:w:${PRODUCT_NAME}.hex
  DEPENDS hex)
# Upload the eeprom with avrdude add_custom_target(upload_eeprom avrdude -c
# ${PROG_TYPE} -p ${MCU}  -U eeprom:w:${PRODUCT_NAME}.eep DEPENDS eeprom)

# Burn fuses add_custom_target(fuses avrdude -c ${PROG_TYPE} -p ${MCU}  -U
# lfuse:w:${L_FUSE}:m -U hfuse:w:${H_FUSE}:m -U efuse:w:${E_FUSE}:m -U
# lock:w:${LOCK_BIT}:m )

# Clean extra files
set_directory_properties(
  PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
             "${PRODUCT_NAME}.hex;${PRODUCT_NAME}.eeprom;${PRODUCT_NAME}.lst")
