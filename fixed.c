// ******** fixed.c ************** 
// Ryan Syed, Brandon Nguyen
// 1/24/2017
// writes decimal and binary fixed point numbers
// and plots data onto the ST77735
// lab 1
// TA: ???
// last revised: 1/28/2017 

#include <stdint.h>
#include "fixed.h"
#include "ST7735.h"

const uint16_t OVERFLOW_DEC = 9999;
const char* OVERFLOWOUT_DEC = " *.***";

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345  " *.***"
  2345  " 2.345"  
 -8100  "-8.100"
  -102  "-0.102" 
  31  " 0.031" 
-12345  " *.***"
 */ 
void ST7735_sDecOut3(int32_t n) 
   // check for overflow
  if(n > OVERFLOW_DEC || n  < (-1 * OVERFLOW_DEC)) {
    ST7735_OutString(OVERFLOWOUT_DEC);
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

const uint32_t OVERFLOW_BIN = 256000;
const char* OVRFLOWOUT_BIN = "***.**";

/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
   0	  "  0.00"
   2	  "  0.01"
  64	  "  0.25"
   100	  "  0.39"
   500	  "  1.95"
   512	  "  2.00"
  5000	  " 19.53"
 30000	  "117.19"
255997	  "999.99"
256000	  "***.**"
*/
void ST7735_uBinOut8(uint32_t n) {
   // check for overflow
  if(n >= OVERFLOW_BIN) {
    ST7735_OutString(OVRFLOWOUT_BIN);
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


int32_t Xmin, Xmax, Yminimum, Ymaximum;
int32_t XRange, YRange;

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
      minX   smallest X data value allowed, resolution= 0.001
      maxX   largest X data value allowed, resolution= 0.001
      minY   smallest Y data value allowed, resolution= 0.001
      maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
  Xmin = minX;
  Xmax = maxX;
  XRange = maxX - minX;
  
  Yminimum = minY;
  Ymaximum = maxY;
  YRange = maxY - minY;
  
   // draw title
  ST7735_SetCursor(0,0);
  ST7735_OutString(title);
  
   // clear plot area
  ST7735_FillRect(0, 8, 128, 152, 0xFFFF);
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num  number of data points in the two arrays
      bufX   array of 32-bit fixed-point data, resolution= 0.001
      bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {
  for(int i = 0; i < num; i++) {
    int32_t x, y;
    if (bufX[i] < Xmax && bufX[i] > Xmin && bufY[i] > Yminimum && bufY[i] < Ymaximum) {
      x = (bufX[i] - Xmin) * 128 / XRange;
      y = (bufY[i] - Yminimum) * -128 / YRange + 136;
      ST7735_DrawPixel(x, y, 0);
    }
  }
}



