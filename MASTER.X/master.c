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
uint8_t cont = 0;       //varibale para lectura del esclavo contador
uint8_t ADC = 0;        //varibale para lectura del esclavo ADC
uint8_t ADC2 = 0;       //varibale para lectura del esclavo Temperatura
float V1,TEMP;
char BUFFER[20];

//******************************************************************************
//  Prototipos de funciones
//******************************************************************************
void setup(void);
void contador (void);
void ADCread (void);
void temperatura (void);

//******************************************************************************
//  Ciclo principal
//******************************************************************************
void main(void) {
    setup();
    _baudios();         //configuración para comunicación USART
    config_txsta();
    config_rcsta();
    LCD_init();         //configuración para la pantalla LCD
    clear_LCD();
    while(1){
        //llama a las diferentes funciones
        contador();     
        ADCread();
        temperatura();
        
        clear_LCD();
        set_cursor(1,1);
        write_string(" S1   S2   S3");
        
        V1 = ADC*0.0196;        //convierte los valores a voltajes
        TEMP = ADC2*1.9547;     //convierte los valores a temperaturas
        sprintf(BUFFER,"%2.1f  %d   %2.1f",V1,cont,TEMP);
        
        set_cursor(2,1);
        write_string(BUFFER);       //se escribe a la LCD los valores
        
        Write_USART_String("S1   S2   S3");
        Write_USART(13);
        Write_USART(10);
        
        Write_USART_String(BUFFER);  //se escribe a USART los valores
        Write_USART(13);
        Write_USART(10);
        
        __delay_ms(500);
    }
}

void contador (void){   // Función para recoger los datos del esclavo contador
    PORTCbits.RC2 = 0;  //slave select activado para slave contador  
    __delay_ms(1);
   
    spiWrite(1);
    cont = spiRead();   //Guarda el valor del contador a una variable
    
    __delay_ms(1);
    PORTCbits.RC2 = 1;   //slave select desactivado para slave contador 
    __delay_ms(1);
}

void ADCread (void){    // Función para recoger los datos del esclavo ADC
    PORTCbits.RC1 = 0;  //slave select activado para slave ADC
    __delay_ms(1);
       
    spiWrite(1);
    ADC = spiRead();    //Guarda el valor del ADC a una variable
       
    __delay_ms(1);
    PORTCbits.RC1 = 1;  //slave select desactivado para slave ADC 
    __delay_ms(1);
}

void temperatura (void){    // Función para recoger los datos del esclavo temperatura
    PORTCbits.RC0 = 0;      //slave select activado para slave temperatura          
    __delay_ms(1);
       
    spiWrite(1);
    ADC2 = spiRead();       //Guarda el valor de la temperatura a una variable
       
    __delay_ms(1);
    PORTCbits.RC0 = 1;       //slave select desactivado para slave temperatura  
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

    TRISC1 = 0;         //Slave select de contador
    TRISC2 = 0;         //Slave select de ADC
    TRISC0 = 0;         //Slave select de temperatura
    PORTCbits.RC2 = 1;  
    PORTCbits.RC1 = 1;  
    PORTCbits.RC0 = 1;  
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
