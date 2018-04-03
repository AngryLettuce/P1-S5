/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.h
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

#ifndef _SPI_MODULE_
#define _SPI_MODULE_

/***************************************************************************
	Include headers :
***************************************************************************/


// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition
#include <csl_mcbsp.h>
#include <csl.h>
#include <dsk6713.h>
#include <csl_irq.h>
#include <csl_gpio.h>
#include <dsk6713_aic23.h>
#include <dsk6713_led.h>


void SPI_init(void);
void SPI_write(Uint8 SPIdata);
Uint8 SPI_read();

#endif // end of #ifndef _SPI_MODULE_

// end of SPI_driver.h
