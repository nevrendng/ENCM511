/*
 * File         : d2main.c
 * Assignment   : ENCM 511 Driver Project 2
 * Author       : Nolan Parmar, Siddharth Menon, Atulya Sharma
 * Section      : B01 - Group 20
 */

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"

//// CONFIGURATION BITS - PRE-PROCESSOR DIRECTIVES ////

// Code protection 
#pragma config BSS = OFF // Boot segment code protect disabled
#pragma config BWRP = OFF // Boot sengment flash write protection off
#pragma config GCP = OFF // general segment code protecion off
#pragma config GWRP = OFF

// CLOCK CONTROL 
#pragma config IESO = OFF    // 2 Speed Startup disabled
#pragma config FNOSC = FRC  // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS  //Primary Oscillator/External clk freq betwn 100kHz and 8 MHz. Options: LS, MS, HS
#pragma config OSCIOFNC = ON  //CLKO output disabled on pin 8, use as IO. 
#pragma config POSCMOD = NONE  // Primary oscillator mode is disabled

// WDT
#pragma config FWDTEN = OFF // WDT is off
#pragma config WINDIS = OFF // STANDARD WDT/. Applicable if WDT is on
#pragma config FWPSA = PR32 // WDT is selected uses prescaler of 32
#pragma config WDTPS = PS1 // WDT postscler is 1 if WDT selected

//MCLR/RA5 CONTROL
#pragma config MCLRE = OFF // RA5 pin configured as input, MCLR reset on RA5 diabled

//BOR  - FPOR Register
#pragma config BORV = LPBOR // LPBOR value=2V is BOR enabled
#pragma config BOREN = BOR0 // BOR controlled using SBOREN bit
#pragma config PWRTEN = OFF // Powerup timer disabled
#pragma config I2C1SEL = PRI // Default location for SCL1/SDA1 pin

//JTAG FICD Register
#pragma config BKBUG = OFF // Background Debugger functions disabled
#pragma config ICS = PGx2 // PGC2 (pin2) & PGD2 (pin3) are used to connect PICKIT3 debugger

// Deep Sleep RTCC WDT
#pragma config DSWDTEN = OFF // Deep Sleep WDT is disabled
#pragma config DSBOREN = OFF // Deep Sleep BOR is disabled
#pragma config RTCOSC = LPRC// RTCC uses LPRC 32kHz for clock
#pragma config DSWDTOSC = LPRC // DeepSleep WDT uses Lo Power RC clk
#pragma config DSWDTPS = DSWDTPS7 // DSWDT postscaler set to 32768 

void delay(uint32_t Duration)
{
    uint32_t counter = 0;   
    uint32_t delayCycles = Duration * 60000;
 
    while(counter < delayCycles) 
    {
        if( PORTAbits.RA2 == 0 ||   //if button is still pressed
            PORTAbits.RA4 == 0 ||
            PORTBbits.RB4 == 0 )
        {
            counter++; //increment counter
        }
        else                        //button was released
        {
            break; //reset counter
        }
    }
    
    return;
}

int main(void) {
    AD1PCFG = 0xFFFF; //Turn all analog pins as digital  
    TRISBbits.TRISB8 = 0; //Set pin RB8 as output
    TRISBbits.TRISB4 = 1;   //Set pin RB4 as input
    TRISAbits.TRISA2 = 1;   //Set pin RA2 as input
    TRISAbits.TRISA4 = 1;   //Set pin RA4 as input
    CNPU1bits.CN0PUE = 1;   //enable pull up resistor for RA4
    CNPU1bits.CN1PUE = 1;   //enable pull up resistor for RB4
    CNPU2bits.CN30PUE = 1;  //enable pull up resistor for RA2
    
    while(1) {
        while(PORTAbits.RA2 == 0) // PB1 is pushed
    {   
        if( PORTAbits.RA4 == 0 || PORTBbits.RB4 == 0 ) //any other PB is pushed
        {
            LATBbits.LATB8 = 1; //turns the LED on
        }
        else
        {
            LATBbits.LATB8 = ~LATBbits.LATB8;   //turns LED on (stays on until not pressed)
            delay(4); //delay approx 1s
        }
    }
    while(PORTAbits.RA4 == 0)   //PB2 is pushed
    {   
        if( PORTAbits.RA2 == 0 || PORTBbits.RB4 == 0 )  //any other PB is pushed
        {
            LATBbits.LATB8 = 1; //turn the LED on
        }
        else
        {
            LATBbits.LATB8 = ~LATBbits.LATB8;   //turns LED on
            delay(6); //delay approx 2s
        }
    }
    while(PORTBbits.RB4 == 0)   //PB3 is pushed
    {   
        if( PORTAbits.RA4 == 0 || PORTAbits.RA2 == 0 )  //any other PB is pushed
        {
            LATBbits.LATB8 = 1; //turn LED on
        }
        else
        {
            LATBbits.LATB8 = ~LATBbits.LATB8;   //turns LED on
            delay(8); //delay approx 3s
        }
    }
    
    LATBbits.LATB8 = 0;
    }
    return 0;
}