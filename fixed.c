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
						n = -1 * n;
        } else {
            ST7735_OutChar(' ');
        }
        
         // print number
         // extract each decimal place and co
        ST7735_OutChar(n/1000 + '0');
        n %= 1000;
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
				uint8_t printzero = 0;
         // print int part of n
        uint16_t nInt = n >> 8;
        if(nInt < 100) {
            ST7735_OutChar(' ');
        } else {
            ST7735_OutChar(nInt/100 + '0');
            nInt %= 100;
						printzero = 1;
        }
        
        if(nInt < 10 && !printzero) {
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

 // I used this lame naming for Y because "Ymin" is already defined =(
int32_t Xmin, Xmax, Y0, Y;
int32_t XRange, YRange;

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
    Xmin = minX;
    Xmax = maxX;
    XRange = maxX - minX;
    
    Y0 = minY;
    Y = maxY;
    YRange = maxY - minY;
    
     // draw title
    ST7735_SetCursor(0,0);
    ST7735_OutString(title);
    
     // clear plot area
    ST7735_FillRect(0, 8, 128, 152, 0xFFFF);
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
    for(int i = 0; i < num; i++) {
        int32_t x, y;
        if (bufX[i] < Xmax && bufX[i] > Xmin && bufY[i] > Y0 && bufY[i] < Y) {
            x = (-1 * bufX[i] - Xmin) * 128 / XRange;
            y = (-1 * bufY[i] - Y0) * 152 / YRange;
            ST7735_DrawPixel(x, y, 0);
        }
    }
}



