// Lab1.c
// Runs on TM4C123
// Uses ST7735.c LCD.
// Jonathan Valvano
// January 17, 2017
// Possible main program to test the lab
// Feel free to edit this to match your specifications

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected 
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO)
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground
#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include "ST7735.h"
#include "PLL.h"
#include "fixed.h"
#include "tm4c123gh6pm.h"
void PortF_Init(void);

#define PF2   (*((volatile uint32_t *)0x40025010))
#define PF3   (*((volatile uint32_t *)0x40025020))
#define PF4   (*((volatile uint32_t *)0x40025040))

// version 1: C floating point
// run with compiler options selected for floating-point hardware
static volatile float f_T; // temperature in C
static volatile uint32_t N; // 12-bit ADC value
void Test1(void){
  for(N=0; N<4096; N++){
    f_T = 10.0+ 0.009768*N;
  }
}
// version 2: C fixed-point
static volatile uint32_t i_T; // temperature in 0.01 C
//volatile uint32_t N; // 12-bit ADC value
void Test2(void){
  for(N=0; N<4096; N++){
    i_T = 1000+ (125*N+64)>>7;
  }
}

// Subroutine to wait 10 msec
// Inputs: None
// Outputs: None
// Notes: ...
void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

extern void Test3(void);
extern void Test4(void);
#define OFF 0x00000010
int main(void){uint32_t i;
  PLL_Init(Bus80MHz);
  PortF_Init();
  ST7735_InitR(INITR_REDTAB);
  
  while(1)
  {
    ST7735_SetCursor(0,0);
    ST7735_OutString("Test1 - C float");
    while(PF4 == OFF)
    {
      Test1();
      PF2 ^= 0xFFFFFFFF;
    }
    DelayWait10ms(100);
    
    ST7735_SetCursor(0,0);
    ST7735_OutString("Test2 - C fixed");
    while(PF4 == OFF)
    {
      Test2();
      PF2 ^= 0xFFFFFFFF;
    }
    DelayWait10ms(100);    
    
    ST7735_SetCursor(0,0);
    ST7735_OutString("Test3 - ASM float");
    while(PF4 == OFF)
    { 
      Test3();
      PF2 ^= 0xFFFFFFFF;
    }
    DelayWait10ms(100);
    
    ST7735_SetCursor(0,0);
    ST7735_OutString("Test4 - ASM fixed");
    while(PF4 == OFF)
    { 
      Test4();
      PF2 ^= 0xFFFFFFFF;
    }
    DelayWait10ms(100);
  }
} 

// PF4 is input
// Make PF2 an output, enable digital I/O, ensure alt. functions off
void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= 0x20;        // 1) activate clock for Port F
  while((SYSCTL_PRGPIO_R&0x20)==0){}; // allow time for clock to start
                                    // 2) no need to unlock PF2, PF4
  GPIO_PORTF_PCTL_R &= ~0x000F0F00; // 3) regular GPIO
  GPIO_PORTF_AMSEL_R &= ~0x14;      // 4) disable analog function on PF2, PF4
  GPIO_PORTF_PUR_R |= 0x10;         // 5) pullup for PF4
  GPIO_PORTF_DIR_R |= 0x04;         // 5) set direction to output
  GPIO_PORTF_AFSEL_R &= ~0x14;      // 6) regular port function
  GPIO_PORTF_DEN_R |= 0x14;         // 7) enable digital port
}
