/*
 * main.c
 */
#include <stdio.h>
#include "SPI_driver.h"




void main(void) {

    //DSK6713_init();
    SPI_init();

    while(1){
        SPI_write(0xFF);
    }
}

