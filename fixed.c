// ******** fixed.c ************** 
// possible header file for Lab 1 Spring 2017
// feel free to change the specific syntax of your system
// put your name here
// put the date here

#include <stdint.h>
#include "fixed.h"
#include "ST7735.h"

uint16_t overflow_dec = 9999;
char* overflowOut_dec = " *.***";

void ST7735_sDecOut3(int32_t n) {
     // check for overflow
    if(n > overflow_dec || n  < (-1 * overflow_dec)) {
        ST7735_OutString(overflowOut_dec);
    } else {
         // test sign bit
        if (n < 0) {
            ST7735_OutChar('-');
        } else {
            ST7735_OutChar(' ');
        }
        
         // print number
         // extract each decimal place and add 48 to convert to ascii
        ST7735_OutChar(n/1000 + 48);
        ST7735_OutChar((n-n/1000)/100 + 48);
        ST7735_OutChar((n-n/100)/10 + 48);
        ST7735_OutChar(n - n/10 + 48);
    }
}

uint32_t overflow_bin = 256000;
char* overflowOut_bin = "***.**";

void ST7735_uBinOut8(uint32_t n) {
    if(n >= overflow_bin) {
        ST7735_OutString(overflowOut_bin);
    }
}

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
  // TODO
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[])
{
  // TODO
}



