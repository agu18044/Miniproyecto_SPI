#include <xc.h> 
#include <stdint.h>  
#include "ADC.h"

#define _XTAL_FREQ 8000000

uint8_t ADCmed(uint8_t ch) {
    ADCON0bits.ADCS = 1;    // Conversion a FOSC/8
    ADCON1 = 0;
    
    switch (ch) {   //configura los pines en analogicos y seleciona el canal
        case 0:   // pin A0 entrada analoga
            TRISAbits.TRISA0 = 1;
            ANSELbits.ANS0 = 1;
            ADCON0bits.CHS = 0;
            break;
        case 1: // pin A1 entrada analoga 
            TRISAbits.TRISA1=1; 
            ANSELbits.ANS1=1; 
            ADCON0bits.CHS=1; 
            break;
        case 2: // pin A2 entrada analoga
            TRISAbits.TRISA2=1; 
            ANSELbits.ANS2=1; 
            ADCON0bits.CHS=2; 
            break;
        case 3: // pin A3 entrada analoga
            TRISAbits.TRISA3=1; 
            ANSELbits.ANS3=1; 
            ADCON0bits.CHS=3; 
            break;
        case 4: // pin RA5 entrada analoga
            TRISAbits.TRISA5=1; 
            ANSELbits.ANS4=1; 
            ADCON0bits.CHS=4; 
            break;
        case 5: // pin E0 entrada analoga
            TRISEbits.TRISE0=1; 
            ANSELbits.ANS5=1; 
            ADCON0bits.CHS=5; 
            break;
        case 6: // pin E1 entrada analoga
            TRISEbits.TRISE1=1; 
            ANSELbits.ANS6=1; 
            ADCON0bits.CHS=6; 
            break;
        case 7: // pin E2 entrada analoga
            TRISEbits.TRISE2=1; 
            ANSELbits.ANS7=1; 
            ADCON0bits.CHS=7; 
            break;
        case 8: // pin B2 entrada analoga
            TRISBbits.TRISB2=1; 
            ANSELHbits.ANS8=1; 
            ADCON0bits.CHS=8; 
            break;
        case 9: // pin B3 entrada analoga
            TRISBbits.TRISB3=1; 
            ANSELHbits.ANS9=1; 
            ADCON0bits.CHS=9; 
            break;
        case 10: // pin B1 entrada analoga
            TRISBbits.TRISB1=1; 
            ANSELHbits.ANS10=1; 
            ADCON0bits.CHS=10; 
            break;
        case 11: // pin B4 entrada analoga
            TRISBbits.TRISB4=1; 
            ANSELHbits.ANS11=1; 
            ADCON0bits.CHS=11; 
            break;
        case 12: // pin B0 entrada analoga
            TRISBbits.TRISB0=1; 
            ANSELHbits.ANS12=1; 
            ADCON0bits.CHS=12; 
            break;
        case 13: // pin B5 entrada analoga
            TRISBbits.TRISB5=1; 
            ANSELHbits.ANS13=1; 
            ADCON0bits.CHS=13; 
            break;
        default: 
            return 0;
    }
 
    ADCON0bits.ADON = 1;    // ADC esta activiado
    __delay_ms(0.25);
    ADCON0bits.GO = 1; // conversion A/D esta en proceso
    while (ADCON0bits.GO == 1){
        return ADRESH;
    }
}
