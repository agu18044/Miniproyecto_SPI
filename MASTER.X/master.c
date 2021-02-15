/*
 * File:   master.c
 * Author: Diego Aguilar
 *
 * Created on 14 de febrero de 2021, 11:44 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "SPI.h"
#include "USART.h"
#include "LCD.h"

//******************************************************************************
//  Palabra de cofiguración
//******************************************************************************
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//  Variables
//******************************************************************************
#define _XTAL_FREQ 8000000
uint8_t cont = 0;
uint8_t ADC = 0;
float V1;
char BUFFER[20];

//******************************************************************************
//  Prototipos de funciones
//******************************************************************************
void setup(void);
void contador (void);
void ADCread (void);

//******************************************************************************
//  Ciclo principal
//******************************************************************************
void main(void) {
    setup();
    _baudios();
    config_txsta();
    config_rcsta();
    LCD_init();
    clear_LCD();
    while(1){
        contador();
        ADCread();
        
        clear_LCD();
        set_cursor(1,1);
        write_string("S1   S2   S3");
        
        V1 = ADC*0.0196; 
        sprintf(BUFFER,"%2.1f   %d", V1,cont);
        
        set_cursor(2,1);
        write_string(BUFFER);
        
        Write_USART_String("S1   S2   S3");
        Write_USART(13);
        Write_USART(10);
        
        Write_USART_String(BUFFER);
        Write_USART(13);
        Write_USART(10);
        
        __delay_ms(500);
    }
}

void contador (void){
    PORTCbits.RC2 = 0;
    __delay_ms(1);
   
    spiWrite(1);
    cont = spiRead();
    
    __delay_ms(1);
    PORTCbits.RC2 = 1;
    __delay_ms(1);
}

void ADCread (void){
    PORTCbits.RC1 = 0;
    __delay_ms(1);
       
    spiWrite(1);
    ADC = spiRead();
       
    __delay_ms(1);
    PORTCbits.RC1 = 1; 
    __delay_ms(1);
}

//******************************************************************************
//  Cofiguración
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0;
    TRISE = 0;
    PORTD = 0;
    PORTE = 0;

    TRISC1 = 0;
    TRISC2 = 0;
    PORTCbits.RC2 = 1;
    PORTCbits.RC1 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
