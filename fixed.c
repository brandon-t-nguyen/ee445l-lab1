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
         // extract each decimal place and co
        ST7735_OutChar(n/1000 + '0');
        nh %= 1000;
        ST7735_OutChar('.');
        ST7735_OutChar(n/100 + '0');
        n %= 100;
        ST7735_OutChar(n/10 + '0');
        n %= 10;
        ST7735_OutChar(n + '0');
    }
}

uint32_t overflow_bin = 256000;
char* overflowOut_bin = "***.**";

void ST7735_uBinOut8(uint32_t n) {
     // check for overflow
    if(n >= overflow_bin) {
        ST7735_OutString(overflowOut_bin);
    } else {
         // print int part of n
        uint16_t nInt = n >> 8;
        if(nInt >= 100) {
            ST7735_OutChar(' ');
        } else {
            ST7735_OutChar(nInt/100 + '0');
            nInt %= 100;
        }
        
        if(nInt >= 10) {
            ST7735_OutChar(' ');
        } else {
            ST7735_OutChar(nInt/10 + '0');
            nInt %= 10;
        }
        
        ST7735_OutChar(nInt + '0');
        ST7735_OutChar('.');
        n &= 0x0ff;
        
         // turn into decimal fixed point and extract the fractional portion
        n = (n * 100) >> 8;
        
        ST7735_OutChar(n/10 + '0');
        n %= 10;
        ST7735_OutChar(n + '0');
    }
}

uint32_t Xmin, Xmax, Ymin, Ymax;
uint32_t XRange, YRange;

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
    Xmin = minX;
    Xmax = maxX;
    XRange = maxX - minX;
    
    Ymin = minY;
    Ymax = maxY;
    YRange = maxY - minY;
    
     // draw title
    ST7735_SetCursor(0,0);
    ST7735_OutString(title);
    
     // clear plot area
    ST7735_FillRect(0, 8, 128, 152, 0xFFFF);
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
    for(int i = 0; i < num; i++) {
        uint32_t x, y;
        if (bufX[i] < Xmax && bufX[i] > Xmin && bufY[i] > Ymin && bufY[i] < Ymax) {
            x = (bufX[i] - Xmin)/XRange * 128;
            y = (bufY[i] - Ymin)/YRange * 152;
            ST7735_DrawPixel(x, y, 0);
        }
    }
}



