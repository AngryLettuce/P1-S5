/*
 * main.c
 */
#include <stdio.h>
#include "SPI_driver.h"


Uint8 data;
short flag = 0;
short index = 0;

void main(void) {

    DSK6713_init();
    DSK6713_LED_init();
    SPI_init();
    configAndStartTimer0(2812500/5);
    init_ext_intr();
    int i;


    while(1){

        /*for(i = 0; i<13; i++){
        DSK6713_waitusec(300000);
        SPI_write((i<<4) | 0x07);
        //data = SPI_read();

        }*/
        if (data == 255){
            DSK6713_LED_toggle(0);
            data = 0;
        }

    }
}

