//--------------------------------------------------------------
// File     : stm32_ub_lis3dsh.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_LIS3DSH_H
#define __STM32F4_UB_LIS3DSH_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"


// Auflösung
typedef enum {
  SCALE_2G =0,    // Auflösung +/- 2g
  SCALE_4G,       // Auflösung +/- 4g
  SCALE_6G,       // Auflösung +/- 6g
  SCALE_8G,       // Auflösung +/- 8g
  SCALE_16G       // Auflösung +/- 16g
}LIS3DSH_SCALE_t;


// Filter
typedef enum {
  FILTER_800Hz =0,  // Filter (800 Hz)
  FILTER_400Hz,     // Filter (400 Hz)
  FILTER_200Hz,     // Filter (200 Hz)
  FILTER_50Hz       // Filter (50 Hz)
}LIS3DSH_FILTER_t;


// LIS3DSH-Roh-Daten
typedef struct {
  uint8_t scale;     // Auflösung
  uint8_t faktor;    // Auflösungsfaktor
  uint8_t x_achse;   // Rohdaten der X-Achse 
  uint8_t y_achse;   // Rohdaten der Y-Achse
  uint8_t z_achse;   // Rohdaten der Z-Achse
}LIS3DSH_RAW_t;
LIS3DSH_RAW_t LIS3DSH_RAW;


// LIS3DSH-Beschleunigungs-Daten
typedef struct {
  int16_t x_achse; // Beschl. der X-Achse (in +/- mg)
  int16_t y_achse; // Beschl. der Y-Achse (in +/- mg)
  int16_t z_achse; // Beschl. der Z-Achse (in +/- mg)
}LIS3DSH_t;
LIS3DSH_t LIS3DSH;



//--------------------------------------------------------------
// ChipSelect-Pin vom LIS3DSH
//--------------------------------------------------------------
#define LIS3DSH_CS_PIN         GPIO_Pin_3
#define LIS3DSH_CS_GPIO_PORT   GPIOE
#define LIS3DSH_CS_GPIO_CLK    RCC_AHB1Periph_GPIOE


//--------------------------------------------------------------
// Register Adressen
//--------------------------------------------------------------
#define  LIS3DSH_REG_ID       0x0F   // ID Register
#define  LIS3DSH_REG_CTRL4    0x20   // CTRL4
#define  LIS3DSH_REG_CTRL5    0x24   // CTRL5

#define  LIS3DSH_REG_XL       0x28   // X-Achse (Lo-Byte)
#define  LIS3DSH_REG_XH       0x29   // X-Achse (Hi-Byte)

#define  LIS3DSH_REG_YL       0x2A   // Y-Achse (Lo-Byte)
#define  LIS3DSH_REG_YH       0x2B   // Y-Achse (Hi-Byte)

#define  LIS3DSH_REG_ZL       0x2C   // Z-Achse (Lo-Byte)
#define  LIS3DSH_REG_ZH       0x2D   // Z-Achse (Hi-Byte)


//--------------------------------------------------------------
// LIS3DSH Defines
//--------------------------------------------------------------
#define  LIS3DSH_ID           0x3F   // ID vom LIS3DSH IC
#define  LIS3DSH_PWR          0x67   // Power-On (100Hz,X,Y,Z)

#define  LIS3DSH_2G           0x00   // Aufloesung (2G)
#define  LIS3DSH_4G           0x08   // Aufloesung (4G)
#define  LIS3DSH_6G           0x10   // Aufloesung (6G)
#define  LIS3DSH_8G           0x18   // Aufloesung (8G)
#define  LIS3DSH_16G          0x20   // Aufloesung (16G)

#define  LIS3DSH_800HZ        0x00   // Filter (800Hz)
#define  LIS3DSH_400HZ        0x40   // Filter (400Hz)
#define  LIS3DSH_200HZ        0x80   // Filter (200Hz)
#define  LIS3DSH_50HZ         0xC0   // Filter (50Hz)

//--------------------------------------------------------------
// Umrechnungsfaktoren bei 8bit
//--------------------------------------------------------------
#define  LIS3DSH_2G_FAKTOR    15     // 15mg pro Digit (+/-2g)
#define  LIS3DSH_4G_FAKTOR    31     // 31mg pro Digit (+/-4g)
#define  LIS3DSH_6G_FAKTOR    46     // 46mg pro Digit (+/-6g)
#define  LIS3DSH_8G_FAKTOR    61     // 61mg pro Digit (+/-8g)
#define  LIS3DSH_16G_FAKTOR   187    // 187mg pro Digit (+/-16g)



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
ErrorStatus UB_LIS3DSH_Init(LIS3DSH_SCALE_t scale);
void UB_LIS3DSH_Read(void);
void UB_LIS3DSH_SetFilter(LIS3DSH_FILTER_t filter);




//--------------------------------------------------------------
#endif // __STM32F4_UB_LIS3DSH_H
