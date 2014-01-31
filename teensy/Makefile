#example makefile to build teensy code


# Target file name (without extension).
ifndef TARGET
TARGET = blinky
endif


# List C source files here. (C dependencies are automatically generated.)
SRC =   $(TARGET).c \
        usb_debug_only.c
        
MCU = at90usb1286

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
        @echo
        @echo $(MSG_FLASH) $@
        $(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@
        teensy_loader_cli -mmcu=$(MCU) -w -v $@