/* 
 * File:   
 * Author: Thomas Roy
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PROJETS5P1_H
#define	PROJETS5P1_H
#include <xc.h> // include processor files - each processor file is guarded.  
#include "constantes.h"

typedef enum {
    STATE_DSK = 0,
    INDEX = 1  
}APP_STATE;

typedef struct{
    APP_STATE currentState;
    char speaker; // chiffre entre 0 et 63
    char state;    
}APP_DATA;

void putch(char byte);
void uartConfig();
void interruptConfig();
void sendIndex(APP_DATA *data);
void sendState(APP_DATA *data);
void spiConfig();



#endif	
