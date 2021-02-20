/*
 * File:   master.c
 * Author: Diego Aguilar
 *
 * Created on 14 de febrero de 2021, 11:44 PM
 */

#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "ADC.h"

//******************************************************************************
//  Palabra de cofiguraci�n
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
uint8_t ADC = 0;
float TEMP;

//******************************************************************************
//  Prototipos de funciones
//******************************************************************************
void setup(void);
void semaforo (void);

//*****************************************************************************
// C�digo de Interrupci�n 
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
        spiWrite(ADC);      //se manda el valor del contador al bus de datos
        SSPIF = 0;          //se apaga la bandera
    }
}

//******************************************************************************
//  Ciclo principal
//******************************************************************************
void main(void) {
    setup();
    while(1){
        ADC = ADCmed(8);        //lee el valor del canal 8 del ADC
        TEMP = ADC * 1.9547;    //convierte el valor a temperatura
        semaforo();             //llama a la funci�n semaforo 
    }
}

void semaforo (void){
    //si la temperatura es menor de 25 se enciende la LED verde
    if (TEMP < 25){
        PORTD = 1;
    }
    //si la tempertatura esta entre 25 y 36 se enciende la LED amarilla
    else if (TEMP > 25 && TEMP < 36){
        PORTD = 2;
    }
    //si la temperatura es mayor a 36 se enciende la LED roja
    else if (TEMP > 36){
        PORTD = 4;
    }
    
}
//******************************************************************************
//  Cofiguraci�n
//******************************************************************************
void setup(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0;
    TRISD = 0;
  
    PORTD = 0;  //el semaforo
    
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupci�n MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupci�n MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
