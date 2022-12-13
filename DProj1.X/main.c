/*
 * File:   main.c
 * Author: Nolan Parmar
 *
 * Created on September 21, 2022, 9:24 PM
 */


#include "xc.h"
#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>


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


// GLOBAL VARIABLES



// MACROS
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   // set sleep mode
#define Idle() {__asm__ volatile ("pwrsav #1");}
#define dsen() {__asm__ volatile ("BSET DSCON, #15");}

void main(void) 
{
    //ucid: 30132281
    TRISAbits.TRISA0 = 1;   //sets RA0 as input
    TRISAbits.TRISA1 = 1;   //sets RA1 as input
    TRISAbits.TRISA2 = 1;   //sets RA2 as input
    TRISAbits.TRISA5 = 1;   //sets RA5 as input
    TRISBbits.TRISB0 = 0;   //sets RB0 as output
    TRISBbits.TRISB1 = 0;   //sets RB1 as output
    TRISBbits.TRISB2 = 0;   //sets RB2 as output
    TRISBbits.TRISB4 = 0;   //sets RB4 as output
    TRISBbits.TRISB7 = 0;   //sets RB7 as output
    TRISBbits.TRISB8 = 0;   //sets RB8 as output
    TRISBbits.TRISB9 = 0;   //sets RB9 as output
    TRISBbits.TRISB12 = 0;  //sets RB12 as output

    while(1) //infinite loop
    {
          if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0 && PORTAbits.RA2 == 0 && PORTAbits.RA5 == 0) //user input 0000
        {
            //no led turns on
           LATBbits.LATB0 =0; 
           LATBbits.LATB1 =0; 
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
           
   
        }
        else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0 && PORTAbits.RA2 == 0 && PORTAbits.RA5 == 1) //user input 0001 
        {
           LATBbits.LATB0 =1; //led turns on
           LATBbits.LATB1 =1; //led turns on
           LATBbits.LATB2 =1; //led turns on
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
        else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0 && PORTAbits.RA2 == 1 && PORTAbits.RA5 == 0) //user input 0010
        {
            //no led turns on
           LATBbits.LATB0 =0; 
           LATBbits.LATB1 =0; 
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
        else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 0 && PORTAbits.RA2 == 1 && PORTAbits.RA5 == 1) //user input 0011
        {
           LATBbits.LATB0 =1; //led turns on
           LATBbits.LATB1 =0; 
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
         else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1 && PORTAbits.RA2 == 0 && PORTAbits.RA5 == 0) //user input 0100
        {
           LATBbits.LATB0 =1; //led turns on
           LATBbits.LATB1 =1; //led turns on
           LATBbits.LATB2 =1; //led turns on
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
         else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1 && PORTAbits.RA2 == 0 && PORTAbits.RA5 == 1) //user input 0101
        {
           LATBbits.LATB0 =1; //led turns on
           LATBbits.LATB1 =1; //led turns on
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
         else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1 && PORTAbits.RA2 == 1 && PORTAbits.RA5 == 0) //user input 0110
        {
           LATBbits.LATB0 =1; //led turns on
           LATBbits.LATB1 =1; //led turns on
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
         else if(PORTAbits.RA0 == 0 && PORTAbits.RA1 == 1 && PORTAbits.RA2 == 1 && PORTAbits.RA5 == 1)//user input 0111
        {
           //8 leds turn on
           LATBbits.LATB0 =1; 
           LATBbits.LATB1 =1; 
           LATBbits.LATB2 =1; 
           LATBbits.LATB4 =1;
           LATBbits.LATB7 =1; 
           LATBbits.LATB8 =1; 
           LATBbits.LATB9 =1; 
           LATBbits.LATB12=1; 
        }
         else if(PORTAbits.RA0 == 1 && PORTAbits.RA1 == 0 && PORTAbits.RA2 == 0 && PORTAbits.RA5 == 0) //user input 1000
        {
           LATBbits.LATB0 =1; //led turns on
           LATBbits.LATB1 =0; 
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
        }
         else //input 1010-1111
         {
           //no led turns on
           LATBbits.LATB0 =0; 
           LATBbits.LATB1 =0; 
           LATBbits.LATB2 =0;
           LATBbits.LATB4 =0;
           LATBbits.LATB7 =0; 
           LATBbits.LATB8 =0; 
           LATBbits.LATB9 =0; 
           LATBbits.LATB12=0; 
         }
        
        
        
    }
    return;
}
