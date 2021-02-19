#
#   example_6-multiconfig-multidir/Makefile
# 
#   Пример "разнесения" разных версий программы по отдельным директориям
#
#     mkdir   debug
#     make    compile_flags="-O0 -g" \
#           --directory=debug \
#           --makefile=../Makefile 
#

# d:\MyProgram\WinAvr\toolchain\avr8\avr8-gnu-toolchain/bin/avr-objdump -h -S isr_wdt_vect.o > isr_wdt_vect.lss

SHELL := cmd.exe
RM := del /Q

PATH_TOOLCHAINE = d:\MyProgram\WinAvr\toolchain\avr8\avr8-gnu-toolchain

CC = $(PATH_TOOLCHAINE)"/bin/avr-gcc.exe"
AS = $(PATH_TOOLCHAINE)"/bin/avr-as.exe"
LD = $(PATH_TOOLCHAINE)"/bin/avr-ld"
OBJCOPY = $(PATH_TOOLCHAINE)/"bin/avr-objcopy"
OBJDUMP = $(PATH_TOOLCHAINE)"/bin/avr-objdump"
SIZE = $(PATH_TOOLCHAINE)"/bin/avr-size"
BUILDDIR = build

MCU = attiny13

CFLAGS = \
-v -x c -D__ATtiny13A__ -DDEBUG -Os -g2 -Wall -std=gnu11 -std=gnu++17 -mmcu=attiny13 \
-funsigned-char -funsigned-bitfields  \
-ffunction-sections -fdata-sections -fpack-struct -fshort-enums 


LFLAGS = -v
#-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums 

TARGET = $(BUILDDIR)/main


# Add inputs and outputs from these tool invocations to the build variables 
SOURCES = main.cpp \
wddelay.cpp \
AnalogIO_1_2Mhz.cpp \
soundsignals.cpp \
tone.cpp 

OBJECTS =  $(BUILDDIR)/main.o \
$(BUILDDIR)/wddelay.o \
$(BUILDDIR)/AnalogIO_1_2Mhz.o \
$(BUILDDIR)/soundsignals.o \
$(BUILDDIR)/tone.o 


$(BUILDDIR)/main.hex: $(OBJECTS)
	@echo Building target: $@
	@echo Invoking:  avr-gcc C Linker
	$(CC) -o$(TARGET).elf $(OBJECTS) $(LIBS) -Wl,-Map=$(TARGET).map -Wl,--start-group -Wl,-lm  \
-Wl,--end-group -Wl,--gc-sections -mmcu=attiny13

	@echo Finished building target: $@
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  $(TARGET).elf $(TARGET).hex

	$(OBJCOPY) -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings \
-O ihex $(TARGET).elf $(TARGET).eep || exit 0

	$(OBJDUMP) -h -S $(TARGET).elf > $(TARGET).lss

	$(OBJCOPY) -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures $(TARGET).elf $(TARGET).srec

	$(SIZE) $(TARGET).elf


# AVR/GNU C Compiler
$(BUILDDIR)/%.o: %.cpp
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<

# Other Targets
clean:
	${RM} ${BUILDDIR}


#$(SOURCES:.c=.o)

# All Target

#all: $(SOURCES) $(TARGET).hex
#main.o millis.o AnalogIO_1_2Mhz.o ModifiedMovingAverage.o soundsignals.o tone.o

# -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\gcc\dev\attiny13"  

# $(LD) -o $(TARGET).elf $(OBJECTS) $(LFLAGS)
# -nostdlib  

# ${OBJCOPY} -O ihex ${TARGET}.elf ${TARGET}.hex
#avr-objcopy -j .text -j .data -O ihex main.elf main.hex
#$(OBJCOPY) -v -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  $(TARGET).elf $(TARGET).hex
# 	$(OBJCOPY) -v -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0\
#  --no-change-warnings -O ihex $(TARGET).elf $(TARGET).eep || exit 0
# 	$(OBJDUMP) -h -S $(TARGET).elf > $(TARGET).lss
# 	${SIZE} -C --mcu=${MCU} ${TARGET}.elf
