//--------------------------------------------------------------
// File     : stm32_ub_lis3dsh.c
// Datum    : 17.05.2014
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : GPIO, STM32_UB_SPI1
// Funktion : LIS3DSH (3Achs Beschleunigungs Sensor, 16bit)
//            Polling-Mode, keine Interrupts,
//            keine Click- und FreeFall-Auswertung
//            es werden nur die 8MSB-Bits benutzt
//
// Hinweis  : Settings :
//            SPI-Mode = 3, FRQ-Max = 10MHz
//            SPI1 [CLK=PA5, MOSI=PA7, MISO=PA6]
//            Chip-Select an PE3
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_lis3dsh.h"
#include "stm32_ub_spi1.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_LIS3DSH_CalcData(void);
void P_LIS3DSH_initIO(void);
void P_LIS3DSH_CS(BitAction wert); 
uint8_t P_LIS3DSH_Read1Adr(uint8_t adr);
void P_LIS3DSH_Write1Adr(uint8_t adr, uint8_t wert);



//--------------------------------------------------------------
// Initialisierung vom LIS3DSH
// scale : [SCALE_2G,SCALE_4G,SCALE_6G,SCALE_8G,SCALE_16G]
// Return_wert :
//  -> ERROR   , wenn Initialisierung fehlgeschlagen
//  -> SUCCESS , wenn Initialisierung ok war 
//--------------------------------------------------------------
ErrorStatus UB_LIS3DSH_Init(LIS3DSH_SCALE_t scale)
{
  ErrorStatus ret_wert=ERROR;
  uint8_t wert;

  // Init aller Variabeln
  LIS3DSH.x_achse=0;
  LIS3DSH.y_achse=0;
  LIS3DSH.z_achse=0;
  LIS3DSH_RAW.scale=LIS3DSH_2G;
  LIS3DSH_RAW.faktor=LIS3DSH_2G_FAKTOR;
  LIS3DSH_RAW.x_achse=0;
  LIS3DSH_RAW.y_achse=0;
  LIS3DSH_RAW.z_achse=0;


  // Init der Chip-Select-Leitung
  P_LIS3DSH_initIO();
  // ChipSelect auf Hi
  P_LIS3DSH_CS(Bit_SET);
  // init vom SPI-1 im Mode-3
  ret_wert=UB_SPI1_Init(SPI_MODE_3); 
  if(ret_wert==SUCCESS) {
    // auslesen vom Identifier
    wert=P_LIS3DSH_Read1Adr(LIS3DSH_REG_ID);
    if(wert!=LIS3DSH_ID) {
      // empfangene ID ist falsch
      ret_wert=ERROR;
    }
    else {
      // ID ist richtig
      // LIS3DSH einschalten
      if(scale==SCALE_2G) {
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,LIS3DSH_2G);
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL4,LIS3DSH_PWR);
        LIS3DSH_RAW.scale=LIS3DSH_2G;
        LIS3DSH_RAW.faktor=LIS3DSH_2G_FAKTOR;
      }
      else if(scale==SCALE_4G) {
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,LIS3DSH_4G);
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL4,LIS3DSH_PWR);
        LIS3DSH_RAW.scale=LIS3DSH_4G;
        LIS3DSH_RAW.faktor=LIS3DSH_4G_FAKTOR;
      }
      else if(scale==SCALE_6G) {
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,LIS3DSH_6G);
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL4,LIS3DSH_PWR);
        LIS3DSH_RAW.scale=LIS3DSH_6G;
        LIS3DSH_RAW.faktor=LIS3DSH_6G_FAKTOR;
      }
      else if(scale==SCALE_8G) {
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,LIS3DSH_8G);
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL4,LIS3DSH_PWR);
        LIS3DSH_RAW.scale=LIS3DSH_8G;
        LIS3DSH_RAW.faktor=LIS3DSH_8G_FAKTOR;
      }
      else {
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,LIS3DSH_16G);
        P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL4,LIS3DSH_PWR);
        LIS3DSH_RAW.scale=LIS3DSH_16G;
        LIS3DSH_RAW.faktor=LIS3DSH_16G_FAKTOR;
      }
    }
  }   

  return(ret_wert); 
}


//--------------------------------------------------------------
// Auslesen vom LIS3DSH und fuellen der Struktur "LIS3DSH"
// (die Funktion muss zyklisch aufgerufen werden) 
//   LIS3DSH.x_achse => Beschleunigung der X-Achse (in +/- mg)
//   LIS3DSH.y_achse => Beschleunigung der Y-Achse (in +/- mg)
//   LIS3DSH.z_achse => Beschleunigung der Z-Achse (in +/- mg)
//
// Hinweis : nur die MSB 8bit werden ausgewertet
//--------------------------------------------------------------
void UB_LIS3DSH_Read(void)
{
  // X-Achse Rohdaten auslesen
  LIS3DSH_RAW.x_achse=P_LIS3DSH_Read1Adr(LIS3DSH_REG_XH);
  // Y-Achse Rohdaten auslesen
  LIS3DSH_RAW.y_achse=P_LIS3DSH_Read1Adr(LIS3DSH_REG_YH);
  // Z-Achse Rohdaten auslesen
  LIS3DSH_RAW.z_achse=P_LIS3DSH_Read1Adr(LIS3DSH_REG_ZH);

  // Daten in mg-Werte umrechnen
  P_LIS3DSH_CalcData();
}


//--------------------------------------------------------------
// Filter einstellen
// filter : [FILTER_800Hz, FILTER_400Hz]
//          [FILTER_200Hz, FILTER_50Hz]
//--------------------------------------------------------------
void UB_LIS3DSH_SetFilter(LIS3DSH_FILTER_t filter)
{
  uint8_t value;

  if(filter==FILTER_800Hz) {
    value=(LIS3DSH_RAW.scale|LIS3DSH_800HZ);
    P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,value);
  }
  else if(filter==FILTER_400Hz) {
    value=(LIS3DSH_RAW.scale|LIS3DSH_400HZ);
    P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,value);
  }
  else if(filter==FILTER_200Hz) {
    value=(LIS3DSH_RAW.scale|LIS3DSH_200HZ);
    P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,value);
  }
  else {
    value=(LIS3DSH_RAW.scale|LIS3DSH_50HZ);
    P_LIS3DSH_Write1Adr(LIS3DSH_REG_CTRL5,value);
  }
}


//--------------------------------------------------------------
// interne Funktion
// Beschleunigungs Roh-Daten in mg-Werte umrechnen
// (Roh-Daten sind im 2er-Komplement gespeichert)
//--------------------------------------------------------------
void P_LIS3DSH_CalcData(void)
{
  uint8_t wert;
  int16_t beschleunigung; 
 
  // X-Daten umrechnen  
  wert=LIS3DSH_RAW.x_achse;
  if((wert&0x80)!=0) {
    // negativ
    wert=(0xFF^wert);
    wert++;
    beschleunigung=(int16_t)(LIS3DSH_RAW.faktor*wert);
    LIS3DSH.x_achse=0-beschleunigung;
  }
  else {
    // positiv
    beschleunigung=(int16_t)(LIS3DSH_RAW.faktor*wert);
    LIS3DSH.x_achse=beschleunigung;
  }

  // Y-Daten umrechnen  
  wert=LIS3DSH_RAW.y_achse;
  if((wert&0x80)!=0) {
    // negativ
    wert=(0xFF^wert);
    wert++;
    beschleunigung=(int16_t)(LIS3DSH_RAW.faktor*wert);
    LIS3DSH.y_achse=0-beschleunigung;
  }
  else {
    // positiv
    beschleunigung=(int16_t)(LIS3DSH_RAW.faktor*wert);
    LIS3DSH.y_achse=beschleunigung;
  }

  // Z-Daten umrechnen  
  wert=LIS3DSH_RAW.z_achse;
  if((wert&0x80)!=0) {
    // negativ
    wert=(0xFF^wert);
    wert++;
    beschleunigung=(int16_t)(LIS3DSH_RAW.faktor*wert);
    LIS3DSH.z_achse=0-beschleunigung;
  }
  else {
    // positiv
    beschleunigung=(int16_t)(LIS3DSH_RAW.faktor*wert);
    LIS3DSH.z_achse=beschleunigung;
  }
}


//--------------------------------------------------------------
// interne Funktion
// Init der ChipSelect-Leitung
//--------------------------------------------------------------
void P_LIS3DSH_initIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Init der Chip-Select-Leitung
  RCC_AHB1PeriphClockCmd(LIS3DSH_CS_GPIO_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = LIS3DSH_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LIS3DSH_CS_GPIO_PORT, &GPIO_InitStructure);
}


//--------------------------------------------------------------
// interne Funktion
// Pegel von ChipSelect einstellen
//--------------------------------------------------------------
void P_LIS3DSH_CS(BitAction wert) {
  if(wert==Bit_RESET) {
    LIS3DSH_CS_GPIO_PORT->BSRRH = LIS3DSH_CS_PIN;
  }
  else {
    LIS3DSH_CS_GPIO_PORT->BSRRL = LIS3DSH_CS_PIN;
  }
}


//--------------------------------------------------------------
// interne Funktion
// auslesen einer Adresse
// adr    : 0x00 bis 0x3F
// return : 0x00 bis 0xFF
//--------------------------------------------------------------
uint8_t P_LIS3DSH_Read1Adr(uint8_t adr)
{
  uint8_t ret_wert=0;
  uint8_t spi_wert;

  // ChipSelect auf Lo
  P_LIS3DSH_CS(Bit_RESET);

  // eine Adresse auslesen
  spi_wert=(adr&0x3F);
  spi_wert|=0x80;

  // Adresse senden
  UB_SPI1_SendByte(spi_wert);

  // Adresse auslesen
  ret_wert=UB_SPI1_SendByte(0x55);

  // ChipSelect auf Hi
  P_LIS3DSH_CS(Bit_SET);

  return(ret_wert);  
}


//--------------------------------------------------------------
// interne Funktion
// beschreiben einer Adresse
// adr    : 0x00 bis 0x3F
// wert   : 0x00 bis 0xFF
//--------------------------------------------------------------
void P_LIS3DSH_Write1Adr(uint8_t adr, uint8_t wert)
{
  uint8_t spi_wert;

  // ChipSelect auf Lo
  P_LIS3DSH_CS(Bit_RESET);

  // eine Adresse beschreiben
  spi_wert=(adr&0x3F);

  // Adresse senden
  UB_SPI1_SendByte(spi_wert);

  // Adresse beschreiben
  UB_SPI1_SendByte(wert);

  // ChipSelect auf Hi
  P_LIS3DSH_CS(Bit_SET);
}
