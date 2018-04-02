
#define SPI_DRIVER_MODULE_IMPORT
#include "SPI_driver.h"



extern MCBSP_Handle DSK6713_AIC23_CONTROLHANDLE;

MCBSP_Config MCBSP0_SPI_Cfg = {
   MCBSP_FMKS(SPCR, FREE, NO)              | // Arrete la comm quand le cpu est n'emule pas
   MCBSP_FMKS(SPCR, SOFT, NO)              | // Arrete la comm quand le cpu est n'emule pas
   MCBSP_FMKS(SPCR, FRST, YES)             | // ?
   MCBSP_FMKS(SPCR, GRST, YES)             | // ?
   MCBSP_FMKS(SPCR, XINTM, XRDY)           | // Xint gen by end of word
   MCBSP_FMKS(SPCR, XSYNCERR, NO)          | // ?
   MCBSP_FMKS(SPCR, XRST, YES)              | //  Serial port transmitter is enabled
   MCBSP_FMKS(SPCR, DLB, OFF)              | // LoopBack, procedure de test
   MCBSP_FMKS(SPCR, RJUST, RZF)            | // Justifier a droite avec extension Extension
   MCBSP_FMKS(SPCR, CLKSTP, DELAY)         | // Rising edge, idle HIGH (MAX3111E)
   MCBSP_FMKS(SPCR, DXENA, OFF)            | // MultiCHannel delay
   MCBSP_FMKS(SPCR, RINTM, RRDY)           | // Rint gen by end of word
   MCBSP_FMKS(SPCR, RSYNCERR, NO)          | // ?
   MCBSP_FMKS(SPCR, RRST, YES),               // Serial port receiver is enabled

   MCBSP_FMKS(RCR, RPHASE, SINGLE)         | // RPHASE = 0
   MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
   MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
   MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
   MCBSP_FMKS(RCR, RFIG, NO)               |
   MCBSP_FMKS(RCR, RDATDLY, 1BIT)          |
   MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
   MCBSP_FMKS(RCR, RWDLEN1, 8BIT)         | // This changes to 16 bits per frame
   MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

   MCBSP_FMKS(XCR, XPHASE, SINGLE)         | // RPHASE = 0
   MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
   MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
   MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
   MCBSP_FMKS(XCR, XFIG, NO)               |
   MCBSP_FMKS(XCR, XDATDLY, 1BIT)          |
   MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
   MCBSP_FMKS(XCR, XWDLEN1, 8BIT)         | // This changes to 32 bits per frame
   MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

   MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
   MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
   MCBSP_FMKS(SRGR, CLKSM, INTERNAL)       |
   MCBSP_FMKS(SRGR, FSGM, DXR2XSR)         | // = 0
   MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
   MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
   MCBSP_FMKS(SRGR, CLKGDV, OF(113)),

   MCBSP_MCR_DEFAULT,
   MCBSP_RCER_DEFAULT,
   MCBSP_XCER_DEFAULT,

   MCBSP_FMKS(PCR, XIOEN, SP)              |
   MCBSP_FMKS(PCR, RIOEN, SP)              |
   MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
   MCBSP_FMKS(PCR, FSRM, INTERNAL)         |
   MCBSP_FMKS(PCR, CLKXM, OUTPUT)          |
   MCBSP_FMKS(PCR, CLKRM, OUTPUT)          |
   MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
   MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
   MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
   MCBSP_FMKS(PCR, FSRP, ACTIVELOW)        |
   MCBSP_FMKS(PCR, CLKXP, RISING)          |
   MCBSP_FMKS(PCR, CLKRP, RISING)
};



void SPI_init(void)
{

    Uint16 registre = DSK6713_rget(DSK6713_MISC);
    registre |= 0x02;
    DSK6713_rset(DSK6713_MISC,registre);
    MCBSP_close(DSK6713_AIC23_CONTROLHANDLE);
    DSK6713_AIC23_CONTROLHANDLE = MCBSP_open(MCBSP_DEV1,MCBSP_OPEN_RESET);
    MCBSP_config(DSK6713_AIC23_CONTROLHANDLE, &MCBSP0_SPI_Cfg);
    MCBSP_start(DSK6713_AIC23_CONTROLHANDLE, MCBSP_XMIT_START | MCBSP_RCV_START | MCBSP_SRGR_START | MCBSP_SRGR_FRAMESYNC, 0x00003000);

    return;
}

void SPI_write(char SPIdata)
{

    while(!MCBSP_xrdy(DSK6713_AIC23_CONTROLHANDLE))
    {
        //do nothing
    }
    MCBSP_write(DSK6713_AIC23_CONTROLHANDLE, SPIdata);


}
