/* 
 * File:   TimeDelay.c
 * Author:  Nolan Parmar, Siddharth Menon,Atulya Sharma 
 *
 * 
 */


#include "xc.h"
#include <stdlib.h>
#include <p24F16KA101.h>
#include "TimeDelay.h"
#include "ChangeClk.h"
uint32_t TMR2flag;

void Delay_ms(uint16_t time_ms) 
{
    
    T2CONbits.TSIDL = 0; //timer when idle
    T2CONbits.T32 = 0; //16bit timer
    T2CONbits.TCS = 0; //internal clock
    
    TMR2 = 0; 
    
    IPC1bits.T2IP = 3; // priority level
    PR2 = time_ms*16; 

    T2CONbits.TON = 1;  
    IEC0bits.T2IE = 1; //enables interrupt
    IFS0bits.T2IF = 0; // interrupt flag
	
    Idle(); 
    return;
}

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    IFS0bits.T2IF=0; 
    T2CONbits.TON=0;
	TMR2flag = 1;
}
