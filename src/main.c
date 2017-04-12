//--------------------------------------------------------------
// File     : main.c
// Datum    : 17.05.2014
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der LIS3DSH Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "ub_lib/stm32_ub_lis3dsh.h"
#include "ub_lib/stm32_ub_led.h"
void delay(uint32_t ms);
#define STM32_CLOCK_HZ 168000000UL
#define STM32_CYCLES_PER_LOOP 6

int main(void)
{
  ErrorStatus check;

  SystemInit(); // Quarz Einstellungen aktivieren

  UB_Led_Init(); // LEDs initialisieren

  // LIS3DSH initialisieren (mit +/- 2G Aufl�sung)
  check=UB_LIS3DSH_Init(SCALE_2G);

  UB_Led_On(LED_RED);
  UB_Led_On(LED_GREEN);
  UB_Led_On(LED_BLUE);
  UB_Led_On(LED_ORANGE);


  delay(1000);

  while(1)
  {
    // LIS3DSH auslesen
    UB_LIS3DSH_Read();
    // wenn Platine nach links geneigt wird, rote LED einschalten
    if(LIS3DSH.y_achse>250) UB_Led_On(LED_ORANGE); else UB_Led_Off(LED_ORANGE);
    // wenn Platine nach rechts geneigt wird, gruene LED einschalten
    if(LIS3DSH.y_achse<-250) UB_Led_On(LED_BLUE); else UB_Led_Off(LED_BLUE); // green
    // wenn Platine nach hinten geneigt wird, blaue LED einschalten
    if(LIS3DSH.x_achse>250) UB_Led_On(LED_RED); else UB_Led_Off(LED_RED); // blue
    // wenn Platine nach vorne geneigt wird, orange LED einschalten
    if(LIS3DSH.x_achse<-250) UB_Led_On(LED_GREEN); else UB_Led_Off(LED_GREEN);
  }

}

void delay(uint32_t ms) {
        ms *= STM32_CLOCK_HZ / 1000 / STM32_CYCLES_PER_LOOP;

        asm volatile(" mov r0, %[ms] \n\t"
                        "1: subs r0, #1 \n\t"
                        " bhi 1b \n\t"
                        :
                        : [ms] "r" (ms)
                        : "r0");
}

