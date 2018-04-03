/*
 * main.c
 */
#include <stdio.h>
#include "SPI_driver.h"


Uint8 data;

void main(void) {

    DSK6713_init();
    DSK6713_LED_init();
    SPI_init();
    int i;


    while(1){

        for(i = 192; i<206; i++){
        DSK6713_waitusec(300000);
        SPI_write(i);

        }
        data = SPI_read();


        if (data == 128){
            DSK6713_LED_toggle(0);
        }

    }
}

