PORT = -P /dev/ttyUSB0

UPLOAD_PROTOCOL = arduino
# UPLOAD_PROTOCOL = usbtiny
BUILD_MCU 	= atmega644p
UPLOAD_SPEED 	= 115200
BUILD_F_CPU 	= 20000000L

# The compiler and build tools for ATMEGA-AVR must be installed on your system,
# or you must download it to a separate directory and write all the paths manually.
CXX 	= avr-gcc
AR 	= avr-ar
SIZE 	= avr-size
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

DUDEFLAGS = -V -p $(BUILD_MCU) $(PORT) -c $(UPLOAD_PROTOCOL) -b $(UPLOAD_SPEED)
FUSES  = -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m -U efuse:w:0xFD:m

SIZEFLAGS =

PRJCXXFLAGS = -Os -mmcu=$(BUILD_MCU) -DF_CPU=$(BUILD_F_CPU)
PRJCXXFLAGS += -W -Wall -pedantic -fshort-enums -fpack-struct
PRJCXXFLAGS += -ffunction-sections -fdata-sections
PRJCXXFLAGS += -funsigned-bitfields
PRJCXXFLAGS += -fno-exceptions
# PRJCXXFLAGS += -fmerge-all-constants
# PRJCXXFLAGS += -funsigned-char
# PRJCXXFLAGS += -fno-inline-small-functions
# PRJCXXFLAGS += -D__AVR_ATmega644P__

# gives information for debugging
# PRJCXXFLAGS += -g

# for floating-point printf
LDFLAGS = -Wl,-u,vfprintf -lprintf_flt -lm

# for smaller printf
# LDFLAGS = -Wl,-u,vfprintf -lprintf_min

# Relax shrinks code even more, but makes disassembly messy
LDFLAGS += -Wl,--relax
# Optional, but often ends up with smaller code
LDFLAGS += -Wl,--gc-sections

FORMAT = ihex
# or binary
