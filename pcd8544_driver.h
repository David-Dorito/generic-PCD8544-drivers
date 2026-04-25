#ifndef _PCD8544_DRIVER_H_
#define _PCD8544_DRIVER_H_

#include <stdint.h>

#define PCD8544_SCREEN_WIDTH  84
#define PCD8544_SCREEN_HEIGHT 48
#define PCD8544_SCREEN_SIZE	  PCD8544_SCREEN_WIDTH*(PCD8544_SCREEN_HEIGHT / 8)

#define PCD8544_DISPLAYMODE_BLANK	 0b00001000
#define PCD8544_DISPLAYMODE_NORMAL	 0b00001100
#define PCD8544_DISPLAYMODE_ALLSEGON 0b00001001
#define PCD8544_DISPLAYMODE_INVERSE	 0b00001101

#define PCD8544_TEMPCOEFF_01MV 0b00000000
#define PCD8544_TEMPCOEFF_09MV 0b00000001
#define PCD8544_TEMPCOEFF_17MV 0b00000010
#define PCD8544_TEMPCOEFF_24MV 0b00000011

#define PCD8544_CONTRAST_DEFAULT 0x3F
#define PCD8544_CONTRAST_MIN	 0x00
#define PCD8544_CONTRAST_MAX	 0x7F

typedef enum PCD8544_Status {
	PCD8544_OK,
	PCD8544_ERROR,
	PCD8544_BUSY,
	PCD8544_TIMEOUT
} PCD8544_Status;

typedef struct PCD8544_Transport {
	PCD8544_Status (*SpiTransmit)(void* pSpiHandle, uint8_t* pTxBuffer, uint16_t len,
								  uint32_t SpiTransmitTimeout);
	void (*GpioWritePin)(void* pGpioHandle, uint8_t isEnabled);
	void (*Delay)(uint32_t milliseconds);
} PCD8544_Transport;

typedef struct PCD8544_Handle {
	uint8_t			   pFrameBuffer[PCD8544_SCREEN_SIZE];
	void*			   pSpi;
	void*			   pDcPin;
	void*			   pResPin;
	void*			   pCsPin; // all of these are void* to be portable between different architectures
	void*			   pLedPin;
	void*			   pVccPin;
	PCD8544_Transport* pTransport;
	uint32_t		   SpiTransmitTimeout;
} PCD8544_Handle;

/*************************************\
  fn: @PCD8544_Init

  param1 PCD8544_Handle*: the handle of the display

  return:

  desc: turns the display on using the sequence described in the datasheet and sets some basic
		configurations

  note:

\**************************************/
PCD8544_Status PCD8544_Init(PCD8544_Handle* pPcd8544Handle);

/*************************************\
  fn: @PCD8544_Deinit

  param1 PCD8544_Handle*: the handle of the display

  return:

  desc: turns the display off

  note: also pulls chip select to HIGH and Data/Command to LOW

\**************************************/
void PCD8544_Deinit(PCD8544_Handle* pPcd8544Handle);

/*************************************\
  fn: @PCD8544_SetBacklight

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: enable or disable sleep mode of the display

  return:

  desc: enables or disables the background light of the display

  note: function is very basic, just a wrapper for GPIO_WritePin() with the LED pin as the arg

\**************************************/
void PCD8544_SetBacklight(PCD8544_Handle* pPcd8544Handle, uint8_t isEnabled);

/*************************************\
  fn: @PCD8544_SetSleepMode

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: enable or disable sleep mode of the display

  return:

  desc: enables or disables the sleep mode of the display

  note: display will stop displaying an image if turned on but RAM will NOT get cleared,
		so once you turn off sleep mode it will start displaying the image again

\**************************************/
PCD8544_Status PCD8544_SetSleepMode(PCD8544_Handle* pPcd8544Handle, uint8_t isEnabled);

/*************************************\
  fn: @PCD8544_SetDisplayMode

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: new mode of the display

  return:

  desc: sets the mode of the display

  note: use the PCD8544_DISPLAYMODE_XXX macros as the mode arg, view the datasheet for a description of
		what they do

\**************************************/
PCD8544_Status PCD8544_SetDisplayMode(PCD8544_Handle* pPcd8544Handle, uint8_t mode);

/*************************************\
  fn: @PCD8544_SetTempCoeff

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: the temperature coefficient

  return:

  desc: sets the temperature coefficient of the display

  note: use the PCD8544_TEMPCOEFF_XXX macros as the coefficient arg, view the datasheet for a description
of what they do

\**************************************/
PCD8544_Status PCD8544_SetTempCoeff(PCD8544_Handle* pPcd8544Handle, uint8_t coefficient);

/*************************************\
  fn: @PCD8544_SetContrast

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: new contrast of the display

  return:

  desc: sets the contrast of the display

  note:

\**************************************/
PCD8544_Status PCD8544_SetContrast(PCD8544_Handle* pPcd8544Handle, uint8_t contrast);

/*************************************\
  fn: @PCD8544_TogglePixelColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: x position of the pixel
  param3 u8: y position of the pixel

  return:

  desc: toggles the colour of the pixel at the specified x and y position

  note: early returns nothing to avoid memory corruption if posX >= width or posY >= height

\**************************************/
void PCD8544_TogglePixelColor(PCD8544_Handle* pPcd8544Handle, uint8_t posX, uint8_t posY);

/*************************************\
  fn: @PCD8544_SetPixelColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: the colour of the pixel
  param3 u8: x position of the pixel
  param4 u8: y position of the pixel

  return:

  desc: sets the colour of the pixel at the specified x and y position

  note: early returns nothing to avoid memory corruption if posX >= width or posY >= height

\**************************************/
void PCD8544_SetPixelColor(PCD8544_Handle* pPcd8544Handle, uint8_t isBlack, uint8_t posX, uint8_t posY);

/*************************************\
  fn: @PCD8544_GetPixelColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: x position of the pixel
  param3 u8: y position of the pixel

  return u8: color of the pixel as a bool

  desc: returns the colour of the pixel at the specified x and y position

  note: returns 0 if posX >= width or posY >= height to avoid memory corruption

\**************************************/
uint8_t PCD8544_GetPixelColor(PCD8544_Handle* pPcd8544Handle, uint8_t posX, uint8_t posY);

/*************************************\
  fn: @PCD8544_FillScreenColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: the colour to set every pixel to

  return:

  desc: sets the colour of every pixel on the screen

  note:

\**************************************/
void PCD8544_FillScreenColor(PCD8544_Handle* pPcd8544Handle, uint8_t isBlack);

/*************************************\
  fn: @PCD8544_UpdateScreen

  param1 PCD8544_Handle*: the handle of the display

  return:

  desc: update the screen with the new changes

  note: the reason why you have to update the screen is because the other display screen drawing funcs
dont actually write to the display RAM, it just writes to the buffer in the handle and this func sets the
whole buffer to the displays RAM

\**************************************/
PCD8544_Status PCD8544_UpdateScreen(PCD8544_Handle* pPcd8544Handle);

#endif
