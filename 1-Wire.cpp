    // https://forum.sources.ru/index.php?showtopic=381077
    #include <inttypes.h>
    #include <string.h>
    #include <avr/io.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
     
    // --- Configuration for OneWire
    #define ONEWIRE_PORTReg     PORTB
    #define ONEWIRE_DDRReg      DDRB
    #define ONEWIRE_PINReg      PINB
    #define ONEWIRE_PIN         PB0
     
    // --- Configure for PWM (fan)
    #define PWM_PORTReg         PORTB
    #define PWM_DDRReg          DDRB
    #define PWM_PIN             PB1
     
    // - Minimal temperature to activate fan
    #define FAN_MIN_TEMPERATURE 40
     
    // - Temperature for maximum fan power
    #define FAN_MAX_TEMPERATURE 70
     
    // - Power, used to start fan. Must be enough to start!
    #define FAN_START_PWM_VALUE 200
    #define FAN_START_TIME      2000
     
    // - Minimal fan power
    #define FAN_MIN_POWER       60
     
    // - Maximal fan power
    #define FAN_MAX_POWER       255
     
    // - Configure for alarm
    #define ALARM_PORTReg               PORTB
    #define ALARM_DDRReg                DDRB
    #define ALARM_PIN                   PB2
     
    #define ALARM_ON_MAX_ERRORS         16
    #define ALARM_ON_MAX_TEMPERATURE    80
     
     
     
    // - Set OneWire pin in Output mode
    #define OneWire_setPinAsOutput  ONEWIRE_DDRReg |= (1<<ONEWIRE_PIN)
     
    // - Set OneWire pin in Input mode
    #define OneWire_setPinAsInput   ONEWIRE_DDRReg &= ~(1<<ONEWIRE_PIN)
     
    // - Set LOW level on OneWire pin
    #define OneWire_writePinLOW     ONEWIRE_PORTReg &=  ~(1<<ONEWIRE_PIN)
     
    // - Set HIGH level on OneWire pin
    #define OneWire_writePinHIGH    ONEWIRE_PORTReg |= (1<<ONEWIRE_PIN)
     
    // - Read level from OneWire pin
    #define OneWire_readPin         ( ( ONEWIRE_PINReg & (1<<ONEWIRE_PIN) ) ? 1 : 0 )
     
     
    // - Turn on alarm
    #define Alarm_On    ALARM_PORTReg |= (1<<ALARM_PIN);
     
    // - Turn off alarm
    #define Alarm_Off   ALARM_PORTReg &= ~(1<<ALARM_PIN);
     
    // - PWM Value ( 0 - off, 255 - max )
    uint8_t PWMValue    = 0;
     
    // - Oveflow counter for PWM
    uint8_t PWMCounter  = 0;
    uint8_t PWM         = 0;
     
     
    //! Calculate CRC-8
    uint8_t crc8(const uint8_t * addr, uint8_t len){
        uint8_t crc = 0;
        while (len--) {
            uint8_t inbyte = *addr++;
            for (uint8_t i = 8; i; i--) {
                uint8_t mix = (crc ^ inbyte) & 0x01;
                crc >>= 1;
                if (mix) crc ^= 0x8C;
                inbyte >>= 1;
            }
        }
        return crc;
    }
     
    //! Reset function
    uint8_t OneWire_reset(){
        
        // - Wait for line
        uint8_t Retries = 125;
        OneWire_setPinAsInput;
        do{
            if( --Retries == 0 ) return 0;
            _delay_us( 2 );
        }while( !OneWire_readPin );
        
        // - Drop line
        OneWire_writePinLOW;
        OneWire_setPinAsOutput;
        _delay_us( 480 );
        
        // - Listen for reply pulse
        OneWire_setPinAsInput;
        _delay_us( 70 );
        
        // - Read line state
        uint8_t State = !OneWire_readPin;
        _delay_us( 410 );
        return State;
    }
     
    //! Write single bit
    void OneWire_writeBit( uint8_t Bit ){
        if( Bit & 1 ){
            // - Drop line
            OneWire_writePinLOW;
            OneWire_setPinAsOutput;
            // - Write Bit-1
            _delay_us( 10 );
            OneWire_writePinHIGH;
            _delay_us( 55 );
        }else{
            // - Drop line
            OneWire_writePinLOW;
            OneWire_setPinAsOutput;
            // - Write Bit-0
            _delay_us( 65 );
            OneWire_writePinHIGH;
            _delay_us( 5 );
        }
    }
     
    //! Read single bit
    uint8_t OneWire_readBit(){
        // - Drop line
        OneWire_setPinAsOutput;
        OneWire_writePinLOW;
        _delay_us( 3 );
        
        // - Wait for data
        OneWire_setPinAsInput;
        _delay_us( 10 );
        
        // - Read bit into byte
        uint8_t Bit = OneWire_readPin;
        _delay_us( 53 );
        return Bit;
    }
     
    //! Write byte
    inline void OneWire_writeByte( const uint8_t Byte, uint8_t Power = 0 ){
        
        // - Write each bit
        for( uint8_t BitMask = 0x01; BitMask; BitMask <<= 1 ) OneWire_writeBit( (BitMask & Byte) ? 1 : 0 );
        
        // - Disable power
        if( !Power ){
            OneWire_setPinAsInput;
            OneWire_writePinLOW;
        }
    }
    //! Read byte
    inline uint8_t OneWire_readByte(){
        uint8_t Byte = 0;
        
        // - Read all bits
        for( uint8_t BitMask = 0x01; BitMask; BitMask <<= 1 ){
            // - Read & store bit into byte
            if( OneWire_readBit() ) Byte |= BitMask;
        }
        return Byte;
    }
     
     
    //! Read buffer
    inline void OneWire_read( uint8_t * Buffer, uint8_t Size ){
        for( uint8_t i = 0; i < Size; i++ ) Buffer[ i ] = OneWire_readByte();
    }
     
    //! Write buffer
    inline void OneWire_write(const uint8_t * Buffer, uint8_t Size, uint8_t Power = 0 ){
        for( uint8_t i = 0; i < Size; i++ )  OneWire_writeByte(  Buffer[ i ] );
        if( !Power ){
            // - Disable power
            OneWire_setPinAsInput;
            OneWire_writePinLOW;
        }
    }
     
    //! Map one range to another
    uint16_t map( uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max){
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
     
     
    //! Main function
    int main() {
        
        // - Configure alarm pin
        ALARM_DDRReg |= ( 1 << PB2 );
        
        // - Configure PWM pin
        PWM_DDRReg |= ( 1 << PB1 );
        
        // - Configure timer for PWM
        TIMSK0      = 0b00000010;
        TCCR0B      = 0b00000001;
        
        // - Allow interrupts
        sei();
        
        // - Buffer for ROM
        uint8_t ROM[ 9 ];
        
        // - Fan state
        uint8_t FanState    = 0;
        
        // - Error count
        uint8_t ErrorCount  = 0;
        
        // - Main loop
        while( 1 ){
            // - Check error count
            if( ErrorCount >= ALARM_ON_MAX_ERRORS ){
                // - Force start fan at 100% power
                PWMValue    = 255;
                FanState    = 1;
                ErrorCount  = 0;
                // - Turn on alarm pin
                Alarm_On;
            }
            
            // - Prepare for new cycle
            _delay_ms( 500 );
            memset( ROM, 0, sizeof( ROM ) );
            
            // - Start conversion
            if( !OneWire_reset() ){
                // - Report error
                ErrorCount++;
                continue;
            }
            OneWire_writeByte( 0xCC, 1 );
            OneWire_writeByte( 0x44, 1 );
            
            // - Wait until conversion finished
            _delay_ms( 1000 );
            
            // - Read ROM
            if( !OneWire_reset() ){
                // - Report error
                ErrorCount++;
                continue;
            }
            OneWire_writeByte( 0xCC, 1 );
            OneWire_writeByte( 0xBE, 1 );
            OneWire_read( ROM, sizeof( ROM ) );
            
            // - Check ROM CRC
            if( crc8( ROM, 8 ) != ROM[ 8 ] ){
                // - Report error
                ErrorCount++;
                continue;
            }
            // ------------------------------------------------------------------
            // - Reset error count & turn alarm off
            ErrorCount = 0;
            Alarm_Off;
            // ------------------------------------------------------------------
            
            // --- Get 8-bit temperature
            // - Construct 16-bit register value from 0 and 1 bytes of ROM.
            // - Remove float part (4 right bits) to get interger value
            uint8_t     Temperature     = ((( ROM[ 1 ] << 8 ) | ROM[ 0 ]) >> 4 );
            
            // - Disable fan when temperature lower than 40* celsius
            if( Temperature < FAN_MIN_TEMPERATURE ){
                // - Stop fan
                PWMValue = 0;
                FanState = 0;
                continue;
            }
            
            // - Start fan if stopped
            if( !FanState ){
                PWMValue = FAN_START_PWM_VALUE;
                _delay_ms( FAN_START_TIME );
                FanState = 1;
            }
            
     
            // - Alarm if overheated
            if( Temperature > ALARM_ON_MAX_TEMPERATURE ){
                
                // - Turn alarm on
                Alarm_On;
                _delay_ms( 100 );
                
                // - Turn alarm off
                Alarm_Off;
            }
     
            // - Set fan power
            uint16_t Power = map( Temperature,
                FAN_MIN_TEMPERATURE,    FAN_MAX_TEMPERATURE,
                FAN_MIN_POWER,          FAN_MAX_POWER
            );
            
            // - Limit power to max PWM
            if( Power > 255 ) Power = 255;
            
            // - Set fan power
            PWMValue = Power;
            
        }
     
    }
     
    //! PWM
    ISR( TIM0_OVF_vect ){
        
        // - Set PWM pin HIGH on each PWM counter overflow
        if( ++PWMCounter == 0 ){
            PWM = PWMValue;
            PWM_PORTReg |= (1<<PWM_PIN);
        }
        
        // - Set PWM pin LOW when PWM counter == PWMValue
        if( PWMCounter == PWM ) PWM_PORTReg &= ~(1<<PWM_PIN);
    }

    uint8_t one_wire::exchange(uint8_t data)
    {
        PORTC &= ~BIT_MASK;
        uint8_t Counter = 8;
        do
        {
            bool Result = exchange_bit(data & (1<<0));
            data  = Result ? (data >> 1) | (1 << 7) : data >> 1;
        }
        while (--Counter);
        PORTC |= BIT_MASK;
        return data;
    }
     
    inline bool one_wire::exchange_bit(bool data)
    {
        DDRC |= BIT_MASK;
        _delay_us(7);
        if(data)
            DDRC &= ~BIT_MASK;
        _delay_us(8);
        data = PINC & BIT_MASK;
        _delay_us(45);
        DDRC &= ~BIT_MASK;
        _delay_us(1);
        return data;
    }

/*
    PWMValue = Temperature; // * 2; // - 40;
 
    buffer2[0]=PWMValue % 10;
    PWMValue/=10;
    buffer2[1]=PWMValue % 10;
    PWMValue/=10;
    buffer2[2]=PWMValue % 10;
    PWMValue/=10;
    buffer2[3]=PWMValue % 10;
        nlcd_GotoXY(4,4);
        nlcd_Putc(0x30+buffer2[3]);
        nlcd_GotoXY(5,4);
        nlcd_Putc(0x30+buffer2[2]);
        nlcd_GotoXY(6,4);
        nlcd_Putc(0x30+buffer2[1]);
        nlcd_GotoXY(7,4);
        nlcd_Putc(0x30+buffer2[0]);
*/