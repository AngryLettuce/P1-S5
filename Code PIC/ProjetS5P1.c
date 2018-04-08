/*
 * File:   ProjetS5P1.c
 * Author: Thomas Roy
 *
 * Created on 20 mars 2018, 20:22
 */

#include <pic18f67j94.h>

#include "ProjetS5P1.h"


void putch(char byte)
{
    while(!TX1IF) continue;
    TXREG1 = byte; 
}

void uartConfig(){
    
    RPINR0_1 = 0x6; // Mapping de la pin RP27 à U1RX 
    RPOR26_27 = 0x1; // Mapping de la pin RP26 à U1TX
 
    //Transmit status and control registor (EUSART) initialization
    TXSTA1 = 0x20;       //8-bits transmission, transmiter enabled, asynchronous mode, Lowspeed mode
    
    
    //Receive status and control registor (EUSART)
    RCSTA1bits.SPEN = 1;     //Serial port enable bit
    RCSTA1bits.RX9 = 0;      //8-bits reception 
    RCSTA1bits.CREN = 1;     //Enable receiver 
    
    BAUDCON1bits.BRG16 = 1;  //Baud Rate control register
    SPBRG1 = 25;             //19200 bps
    SPBRGH1 = 0;
    
}

void interruptConfig(){
    INTCONbits.GIEH = 1;    //Enables all High priority peripherical interrupts
    INTCONbits.GIEL = 1;    //Enables all low priority peripherical interrupts
    PIE1bits.RC1IE = 1;      //Enables the EUSART receive interrupt bit
    PIE2bits.SSP2IE = 1;
    PIE1bits.SSP1IE = 1;
    
    
}

void sendIndex(APP_DATA *data){
    static char data2send = (1 << 7);
    data2send |= data->speaker;
    data2send |= (data->currentState << 6);
    TXREG1 = data2send;
    return;
}

void sendState(APP_DATA *data){
    static char data2send = (1 << 7);
    data2send |= data->state;
    data2send |= (data->currentState << 6);
    TXREG1 = data2send;
    return;
    
}
       
void spiConfig(){
     RPINR8_9 = 0x31; //SCK1 -> RP7 , SDI1 -> RP12
     RPINR10_11 = 0x03; //not_SS1 -> RP14
     RPOR12_13 = 0x40; //SDO1 ->RP13
     
     TRISBbits.TRISB5 = 0;
     TRISBbits.TRISB3 = 1;
     TRISBbits.TRISB2 = 1;
     
     SSP1STATbits.SMP = 0;
     SSP1STATbits.CKE = 1;
     
     SSP1CON1bits.CKP = 0;
     SSP1CON1bits.SSPEN = 1;
     SSP1CON1bits.SSPM = 0b0100;
     
    
}
