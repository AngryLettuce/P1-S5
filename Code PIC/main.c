/*
 * File:   main.c
 * Author: Thomas Roy
 *
 * Created on 20 mars 2018, 10:13
 */


#include <xc.h>
#include <stdlib.h>
#include "config_bits.h"
#include <stdio.h>
#include "ProjetS5P1.h"
#include "LCD_SPI.h"
#define _XTAL_FREQ 8000000
void interrupt low_priority comPC(void);
void interrupt high_priority DSK(void);

APP_DATA data;

//char data[4]= {105,110,100,48};
//void interrupt low_priority comPC(void)
//{
//    if(PIE1bits.RC1IE && PIR1bits.RC1IF){
//        TXREG1 = RCREG1;
//    }
//}
char dataUART = 0;
char dataSPI = 0;

void interrupt high_priority DSK(void)
{
    
    if(PIE1bits.RC1IE && PIR1bits.RC1IF){
       if(!SSP1STATbits.BF){
          dataUART = RCREG1; //envoi DSK
       }
        
        
        
    }
    if(SSP1STATbits.BF && PIR1bits.SSP1IF){
        if(TXSTA1bits.TRMT){
            TXREG1 = SSP1BUF; //envoi au PC
            SSP1BUF = dataUART;
            dataUART = 0;
            
        }
    }

    
}

int main(void){
    uartConfig();
    interruptConfig();
    spiConfig();
    //initialisation_LCD();
    data.currentState = STATE_DSK;
    data.state = 3;
    data.speaker = 4;
    //putStringLCD("Hello World");
    while(1){
    }
    return (EXIT_SUCCESS);
}