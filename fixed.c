// ******** fixed.c **************
// Brandon Nguyen
// Ryan Syed
// 01/31/2017

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
  else
  {
    ST7735_OutChar(' ');
  }

  // to unroll a loop or not unroll a loop, that is the question
  int32_t currentDigit = n/1000;
  ST7735_OutChar('0' + currentDigit);
  ST7735_OutChar('.');
  n = n - currentDigit * 1000;

  // here's an unrolled implementation
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
  if(n >= 256000)
  {
    ST7735_OutString("***.**");
    return;
  }
  // effectively what was specified was a 24_8 fixed point format

  // print the integer part first
  uint32_t integer = n >> 8;
  int isLeadingDigit = 1;
  for(int32_t divider=100; divider > 1; divider/=10)
  {
    int32_t currentDigit = integer/divider;
    if(currentDigit == 0 && isLeadingDigit)
    {
      ST7735_OutChar(' ');
    }
    else
    {
      ST7735_OutChar('0' + currentDigit);
      isLeadingDigit = 0;
    }
    integer -= currentDigit * divider;
  }

  ST7735_OutChar('0' + integer);  // capture the 1's place
  ST7735_OutChar('.');


  // extract the fraction bits
  uint32_t fraction = n & 0xFF;

  // convert the fraction to decimal form by multiplying by 100 then dividing by 256
  uint32_t decimal = (fraction * 100) >> 8;

  // print out the fraction part
  for(int32_t divider=10; divider > 0; divider/=10)
  {
    int32_t currentDigit = decimal/divider;
    ST7735_OutChar('0' + currentDigit);
    decimal -= currentDigit * divider;
  }
}

// Note that we want to fit the title
// Due to this declared setup, we're going to need to use static globals :(
#define LCD_HEIGHT ST7735_TFTHEIGHT
#define LCD_WIDTH ST7735_TFTWIDTH
#define OUT_OF_BOUNDS 0xFFFFFFFF
#define CHAR_HEIGHT 10

#define AXIS_COLOR 0x0000
#define PLOT_COLOR 0x12CE
static uint32_t PlotH = LCD_HEIGHT; // how much pixel space in the Y dimension
static uint32_t PlotW = LCD_WIDTH;  // how much pixel space in the X dimension
static uint32_t OriginX = 0;
static uint32_t OriginY = 0;

static int32_t MinX, MaxX, MinY, MaxY;
static int32_t VirtualW, VirtualH;  // how large our virtual space is
static int32_t AbsoluteDimension;   // used for having a shared x and y scale
static int32_t VirtualDimension;    // used for having a shared x and y scale

// Converts a set of virtual coordinates to pixel location
// vX and vY are virtual coordinates
// paX and paY are output pointers to write to
// This is currently set up for auto-scaling of axes
static void convertCoord( const int32_t vX, const int32_t vY, uint32_t *paX, uint32_t *paY )
{
  // get our virtual offsets
  int32_t vOffsetX = vX - MinX;
  int32_t vOffsetY = vY - MinY;

  // get our absolute pixel offsets
  int32_t aOffsetX = vOffsetX * AbsoluteDimension / VirtualDimension;
  int32_t aOffsetY = vOffsetY * AbsoluteDimension / VirtualDimension;  // share scaling

  // get our final pixel values
  *paX = aOffsetX;
  *paY = LCD_HEIGHT - aOffsetY;

  if( vX < MinX || vX > MaxX )
  {
    *paX = OUT_OF_BOUNDS;
  }
  if( vY < MinY || vY > MaxY )
  {
    *paY = OUT_OF_BOUNDS;
  }
}

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
  MinX = minX;
  MaxX = maxX;
  MinY = minY;
  MaxY = maxY;
  int numLines = 1;           // first line takes up text
  char *c = title;
  while(*c)
  {
    if(*c=='\n')
    {
      ++numLines;  // count number of lines
    }
    ++c;
  }

  OriginY = CHAR_HEIGHT * numLines;
  PlotH = LCD_HEIGHT - OriginY;
  
  VirtualW = MaxX-MinX;
  VirtualH = MaxY-MinY;
  
  // pick the smaller dimension to scale by
  if(PlotH > PlotW)
  {
    AbsoluteDimension = PlotW;
    VirtualDimension  = VirtualW;
  }
  else
  {
    AbsoluteDimension = PlotH;
    VirtualDimension  = VirtualH;
  }

  ST7735_SetCursor(0,0);
  ST7735_OutString(title);
  ST7735_FillRect(OriginX,OriginY,PlotW,PlotH,0xFFFF); // white out the plot area

  // draw our axis lines
  uint32_t xAxis;
  uint32_t yAxis;
  convertCoord(0,0,&xAxis,&yAxis);
  if( xAxis != OUT_OF_BOUNDS )
  {
    ST7735_DrawFastVLine(xAxis,OriginY,PlotH,AXIS_COLOR);
  }
  if( yAxis != OUT_OF_BOUNDS )
  {
    ST7735_DrawFastHLine(OriginX,yAxis,PlotW,AXIS_COLOR);
  }
}

void ST7735_XYplot(uint32_t num, const int32_t bufX[], const int32_t bufY[])
{
  uint32_t x, y;
  for(uint32_t i = 0; i < num; ++i)
  {
    convertCoord(bufX[i],bufY[i],&x,&y);
    if( x != OUT_OF_BOUNDS && y != OUT_OF_BOUNDS )
    {
      ST7735_DrawPixel(x,y,PLOT_COLOR);
    }
  }
}