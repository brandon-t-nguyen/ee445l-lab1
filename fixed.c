// ******** fixed.c ************** 
// Brandon Nguyen
// Ryan Syed
// 01/25/2017

#include <stdint.h>
#include "fixed.h"
#include "ST7735.h"

void ST7735_sDecOut3(int32_t n)
{
    // boundary value checks
    if(n < -9999 || 9999 < n)
    {
        ST7735_OutString(" *.****");
        return;
    }

    if(n < 0)
    {
        // if negative, abs() it and print a dash
        n = -n;
        ST7735_OutChar('-');
    }

    // to unroll a loop or not unroll a loop, that is the question
    int32_t currentDigit = n/1000;
    ST7735_OutChar('0' + currentDigit);
    ST7735_OutChar('.');
    n = n - currentDigit * 1000;

    currentDigit = n/100;
    ST7735_OutChar('0' + currentDigit);
    n = n - currentDigit * 100;

    currentDigit = n/10;
    ST7735_OutChar('0' + currentDigit);
    n = n - currentDigit * 10;

    // n is already < 10
    ST7735_OutChar('0' + n);
}

void ST7735_uBinOut8(uint32_t n)
{
  // TODO
}

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
  // TODO
}

void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[])
{
  // TODO
}



