/*
 * File:   ProjetS5P1.c
 * Author: Thomas Roy
 *
 * Created on 20 mars 2018, 20:22
 */

#include "ProjetS5P1.h"

void putch(char byte)
{
    while(!TX1IF) continue;
    TXREG1 = byte; 
}

void uartConfig(){
    
    RPINR0_1 = 0x6; // Mapping de la pin RP27 � U1RX 
    RPOR26_27 = 0x1; // Mapping de la pin RP26 � U1TX
 
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
}
