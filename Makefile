################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := del -f

CC = "d:/MyProgram/WinAvr/avr/bin/avr-gcc.exe"
LD = "d:/MyProgram/WinAvr/avr/bin/avr-ld"
OBJCOPY = "d:/MyProgram/WinAvr/avr/bin/avr-objcopy"
OBJDUMP = "d:/MyProgram/WinAvr/avr/bin/avr-objdump"
SIZE = "d:/MyProgram/WinAvr/avr/bin/avr-size"
BUILDDIR = build/

MCU = attiny13

CFLAGS = \
-v -x c -D__ATtiny13A__ -DDEBUG  -Os -g2 -Wall -std=c11 -mmcu=attiny13
#-funsigned-char -funsigned-bitfields  -mext=cci
#-ffunction-sections -fdata-sections -fpack-struct -fshort-enums 


LFLAGS = -v
#-funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums 

TARGET = main


# Add inputs and outputs from these tool invocations to the build variables 
SOURCES += main.c \
millis.c \
AnalogIO_1_2Mhz.c \
ModifiedMovingAverage.c \
soundsignals.c \
tone.c

OBJECTS =  main.o \
millis.o \
AnalogIO_1_2Mhz.o \
ModifiedMovingAverage.o \
soundsignals.o \
tone.o
#$(SOURCES:.c=.o)

# All Target

#all: $(SOURCES) $(TARGET).hex
#main.o millis.o AnalogIO_1_2Mhz.o ModifiedMovingAverage.o soundsignals.o tone.o

main.hex: main.o millis.o AnalogIO_1_2Mhz.o ModifiedMovingAverage.o soundsignals.o tone.o
	@echo Building target: $@
	@echo Invoking:  avr-gcc C Linker
	$(LD) -o $(TARGET).elf $(OBJECTS) $(LFLAGS)
# -nostdlib  
	@echo Finished building target: $@
	${OBJCOPY} -O ihex ${TARGET}.elf ${TARGET}.hex
#avr-objcopy -j .text -j .data -O ihex main.elf main.hex
#$(OBJCOPY) -v -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -v -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex $(TARGET).elf $(TARGET).eep || exit 0
	$(OBJDUMP) -h -S $(TARGET).elf > $(TARGET).lss
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf



# AVR32/GNU C Compiler
main.o: main.c
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

millis.o: millis.c
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

AnalogIO_1_2Mhz.o: AnalogIO_1_2Mhz.c
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

ModifiedMovingAverage.o: ModifiedMovingAverage.c
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

soundsignals.o: soundsignals.c
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

tone.o: tone.c
	@echo Building file: $<
	@echo Invoking: avr-gcc C Compiler 
	${CC} -mmcu=${MCU} -c $(CFLAGS) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	
	

# Other Targets
clean:
	$(RM) $(OBJECTS) $(EXECUTABLES) *.d 
	$(RM) $(C_DEPS_AS_ARGS)   
	del -rf "main.elf" "main.hex" "main.eep" "main.map" "main.usersignatures" "main.a"  "main.lss"
