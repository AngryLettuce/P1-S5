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
       while(SSP1STATbits.BF && !TXSTA1bits.TRMT){};
           SSP1BUF = RCREG1;
        
        
    }
    if(SSP1STATbits.BF && PIR1bits.SSP1IF){
 //       while(!TX1IF) continue;
        while(!TXSTA1bits.TRMT){};
            TXREG1 = SSP1BUF;

    }

    
}

int main(void){
    uartConfig();
    interruptConfig();
    spiConfig();
    data.currentState = STATE_DSK;
    data.state = 3;
    data.speaker = 4;
    //char i = 0;     
    while(1){
      /*  switch(data.currentState) {
            case STATE_DSK :
                __delay_ms(1000);
                sendState(&data);
                data.currentState = INDEX;
                break;
            
            case INDEX :
                __delay_ms(1000);
                sendIndex(&data);
                data.currentState = STATE_DSK;
                break;            
        }*/

        
    }
    return (EXIT_SUCCESS);
}
    
        
        
        
        
                
                        
                            
                    
        
        
        
        
        

//            for (i = 97; i <= 112; i++){
//               TXREG1 = i;
//               
//            }


        
        
       // printf("Hello\n");
    
    

