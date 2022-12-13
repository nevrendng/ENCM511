/* 
 * File:   IOs.c
 * Author:  Nolan Parmar, Siddharth Menon,Atulya Sharma 
 *
 * 
 */

#include "xc.h"
#include "TimeDelay.h"
#include "UART2.h"
#include "ChangeClk.h"
#include "IOs.h"





void IOinit(void) 
{
    AD1PCFG = 0xFFFF; // all analog pins are digital
    
    TRISBbits.TRISB8 = 0; // Make  RB8 as digital output
    
    TRISAbits.TRISA2 = 1; // Make  RA2 as digital input
    TRISBbits.TRISB4 = 1; // Make  RB4 as digital input
    TRISAbits.TRISA4 = 1; // Make  RA4 as digital input
    
    CNEN1bits.CN0IE = 1; // CN inputs
    CNEN1bits.CN1IE = 1;
    CNEN2bits.CN30IE = 1;
    
    CNPU1bits.CN0PUE = 1; // setting pull ups
    CNPU1bits.CN1PUE = 1;
    CNPU2bits.CN30PUE = 1;
    
    IPC4bits.CNIP = 3; //level 2 priority higher than the timer priority
    IFS1bits.CNIF = 0; // Set the CN interrupt flag to 0
    IEC1bits.CNIE = 1;
    
    
    return;
}
uint16_t NEWCNFLAG;
uint16_t ON=0;
uint16_t COUNTDOWN_ON=0;
uint8_t MIN=0;
uint8_t SEC=0;

void Display()
{
    NewClk(8);
    
    XmitUART2('\r',1);
    Disp2Dec(MIN);
    Disp2String("m: ");
    Disp2Dec(SEC);
    XmitUART2('s',1);
    
    if(ON==1)   //if alarm is on display else dont display                      
        Disp2String("  --ALARM!");
    else
        Disp2String("          ");
    
    NewClk(32);
        
}

void CountDown() //countdown 
{                          
    if(SEC>0)
    {
        SEC--;
    }
    else if(SEC==0 && MIN>0)
    {
        SEC=59;
        MIN--;
    }else    // point is reached min=0 and seconds=0 and alarm is on
    {                                  
        ON=1;
        COUNTDOWN_ON=0;
    }      
}

void IOcheck(void)
{
    IEC1bits.CNIE = 0; //disable CN interrupts to avoid debounces
    Delay_ms(400);   // 400 msec delay to filter out debounces 
    IEC1bits.CNIE = 1; //Enable CN interrupts to detect pb release
    
    Display();
    
    if(NEWCNFLAG==0)//when nothing is pressed
    { 
        if(COUNTDOWN_ON==0)//check if countdown is on 
        {  
            if(ON==0)
            {
                LATBbits.LATB8 = 0;
            }
            else
            {
                LATBbits.LATB8 = 1;
            }
            Idle(); //stay idle until button is pressed
        }
        else
        {                              //start countdown and flash led
            CountDown();
            Delay_ms(600); 
            if(LATBbits.LATB8==1)           //flashing led
            {
                LATBbits.LATB8 = 0;
            }
            else if(LATBbits.LATB8==0)
            {
                LATBbits.LATB8 = 1;
            }
        }
    }
    
    while(NEWCNFLAG==1)
    {
        MIN++;                  //increment minutes while button is pressed             
        if(ON==1)               //turn off alarm if button is pressed      
        {
            ON=0;
        }
        if(COUNTDOWN_ON==1)                  //stop countdown if button is pressed
        {
            COUNTDOWN_ON=0;
        }
        if(MIN==60)                          //if minutes go past 59
        {
            MIN=0;
        }
        Display();           //display alarm minutes              
        Delay_ms(500);       //delay for precise minute increments         
    }
    while(NEWCNFLAG==2)
    {
        SEC++;          //increment seconds while button is pressed                    
        if(ON==1)       //if button is pressed when alarm is on turn off alarm              
        {
            ON=0;
        }
        if(COUNTDOWN_ON==1)    //stop countdown is button is pressed
        {
            COUNTDOWN_ON=0;
        }
        if(SEC==60)
        {
            SEC=0;
        }
        Display();      //display alarm seconds 
        Delay_ms(500);                  
    }
    
    if(NEWCNFLAG==3)
    {
        
        Delay_ms(3000);
        if(NEWCNFLAG==3)//if button is pressed after the reset
        {                   
            MIN=0;
            SEC=0;
            ON=0;
            COUNTDOWN_ON=0;
        }
        else //short pressed
        {                              
            if(COUNTDOWN_ON==0)
            {            //check if there is anything to countdown
                if(MIN>0 || SEC>0)
                {
                    COUNTDOWN_ON=1;
                }
            }
            else
            {                          //Turn off if countdown is on
                COUNTDOWN_ON=0;
            }
        }
    }
    
  
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    
     if (IFS1bits.CNIF == 1)  //setting states
    {
        if (PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1)        //nothing pressed
        {
            NEWCNFLAG = 0; 
        } else if (PORTAbits.RA2 == 0 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 1) //PB1 pressed
        {
            NEWCNFLAG = 1; 
        } else if (PORTAbits.RA2 == 1 && PORTBbits.RB4 == 0 && PORTAbits.RA4 == 1) //PB2 pressed
        {
            NEWCNFLAG = 2;
        } else if (PORTAbits.RA2 == 1 && PORTBbits.RB4 == 1 && PORTAbits.RA4 == 0) //PB3 pressed
        {
            NEWCNFLAG = 3; 
        } else{                                                                    //other combinations being pressed same as nothing pressed  
            NEWCNFLAG = 0; 
        }
    }

IFS1bits.CNIF = 0; // clear IF flag
Nop();
}