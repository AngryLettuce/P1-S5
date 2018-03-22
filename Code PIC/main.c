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
#define _XTAL_FREQ 8e6
void interrupt low_priority myIsr(void);

char data[4]= {105,110,100,48};;
void interrupt low_priority myIsr(void)
{
    if(PIE1bits.RC1IE && PIR1bits.RC1IF){
        TXREG1 = RCREG1;
    }
//BONJOUR
}

void main(void){
    uartConfig();
    interruptConfig();
    char i = 0;     
    while(1){
        

            for (i = 48; i <= 57; i++){
                TXREG1 = i;
                __delay_ms(1500);  
               
            }


        
        
       // printf("Hello\n");
    }
}
