#include <stddef.h>
#include "pcd8544_driver.h"

#define LOW	 0
#define HIGH 1

#define SET_XADDR_0		0b10000000
#define SET_YADDR_0		0b01000000
#define SET_POWERDOWN	0b00100100
#define SET_POWERUP		0b00100000
#define SET_EXTENDINST	0b00100001
#define SET_BASICINST	0b00100000
#define SET_VOP			0b10000000
#define SET_TC			0b00000100
#define SET_DISPLAYMODE 0b00001000
#define SET_BIAS_1_48	0b00010011

/*************************************\
  fn: @PCD8544_Init

  param1 PCD8544_Handle*: the handle of the display

  return:

  desc: turns the display on using the sequence described in the datasheet and sets some basic
configurations

  note: sets the contrast to PCD8544_CONTRAST_DEFAULT found in the header file

\**************************************/
PCD8544_Status PCD8544_Init(PCD8544_Handle* pPcd8544Handle) {
	PCD8544_Status status = PCD8544_OK;

	// Control VCC if user provides the pin, otherwise assume external 3.3V
	if (pPcd8544Handle->pVccPin != NULL) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, HIGH);
		pPcd8544Handle->pTransport->Delay(10); // Wait for VCC to stabilize
	}

	// Reset sequence
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pResPin, LOW);
	pPcd8544Handle->pTransport->Delay(1); // Min 100ns, 1ms is safe
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pResPin, HIGH);
	pPcd8544Handle->pTransport->Delay(50); // Wait for internal reset

	// Send configuration commands
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, LOW);
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);

	uint8_t command = SET_EXTENDINST;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		if (pPcd8544Handle->pVccPin != NULL)
			pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
		return status;
	}

	command = (SET_VOP | PCD8544_CONTRAST_DEFAULT);
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		if (pPcd8544Handle->pVccPin != NULL)
			pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
		return status;
	}

	command = SET_TC;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		if (pPcd8544Handle->pVccPin != NULL)
			pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
		return status;
	}

	command = SET_BIAS_1_48;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		if (pPcd8544Handle->pVccPin != NULL)
			pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
		return status;
	}

	command = SET_BASICINST;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		if (pPcd8544Handle->pVccPin != NULL)
			pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
		return status;
	}

	command = PCD8544_DISPLAYMODE_NORMAL;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		if (pPcd8544Handle->pVccPin != NULL)
			pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_Deinit

  param1 PCD8544_Handle*: the handle of the display

  return:

  desc: turns the display off

  note: also pulls chip select to HIGH and Data/Command to LOW

\**************************************/
void PCD8544_Deinit(PCD8544_Handle* pPcd8544Handle) {
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);
	if (pPcd8544Handle->pLedPin != NULL)
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pLedPin, LOW);
	if (pPcd8544Handle->pVccPin != NULL)
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pVccPin, LOW);
}

/*************************************\
  fn: @PCD8544_SetBacklight

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: enable or disable sleep mode of the display

  return:

  desc: enables or disables the background light of the display

  note: function is very basic, just a wrapper for GpioWritePin() with the LED pin as the arg

\**************************************/
void PCD8544_SetBacklight(PCD8544_Handle* pPcd8544Handle, uint8_t isEnabled) {
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pLedPin, !isEnabled);
}

/*************************************\
  fn: @PCD8544_SetSleepMode

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: enable or disable sleep mode of the display

  return:

  desc: enables or disables the sleep mode of the display

  note: display will stop displaying an image if turned on but RAM will NOT get cleared,
		so once you turn off sleep mode it will start displaying the image again

\**************************************/
PCD8544_Status PCD8544_SetSleepMode(PCD8544_Handle* pPcd8544Handle, uint8_t isEnabled) {
	PCD8544_Status status = PCD8544_OK;

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, LOW);

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);
	if (isEnabled)
		isEnabled = SET_POWERDOWN;
	else
		isEnabled = SET_POWERUP;

	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &isEnabled, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_SetDisplayMode

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: new mode of the display

  return:

  desc: sets the mode of the display

  note: use the PCD8544_DISPLAYMODE_XXX macros as the mode arg, view the datasheet for a description of
what they do

\**************************************/
PCD8544_Status PCD8544_SetDisplayMode(PCD8544_Handle* pPcd8544Handle, uint8_t mode) {
	PCD8544_Status status = PCD8544_OK;

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, LOW);

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);
	mode &= (SET_DISPLAYMODE | 0b00000101);
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &mode, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_SetTempCoeff

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: the temperature coefficient

  return:

  desc: sets the temperature coefficient of the display

  note: use the PCD8544_TEMPCOEFF_XXX macros as the coefficient arg, view the datasheet for a description
of what they do

\**************************************/
PCD8544_Status PCD8544_SetTempCoeff(PCD8544_Handle* pPcd8544Handle, uint8_t coefficient) {
	PCD8544_Status status = PCD8544_OK;

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, LOW);

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);
	coefficient &= (SET_TC | 00000011);
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &coefficient, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_SetContrast

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: new contrast of the display

  return:

  desc: sets the contrast of the display

  note:

\**************************************/
PCD8544_Status PCD8544_SetContrast(PCD8544_Handle* pPcd8544Handle, uint8_t contrast) {
	PCD8544_Status status = PCD8544_OK;

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, LOW);

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);

	uint8_t command = SET_EXTENDINST;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	command = (SET_VOP | contrast);
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	command = SET_BASICINST;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_TogglePixelColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: x position of the pixel
  param3 u8: y position of the pixel

  return:

  desc: toggles the colour of the pixel at the specified x and y position

  note: early returns nothing to avoid memory corruption if posX >= width or posY >= height

\**************************************/
void PCD8544_TogglePixelColor(PCD8544_Handle* pPcd8544Handle, uint8_t posX, uint8_t posY) {
	if (posX >= PCD8544_SCREEN_WIDTH || posY >= PCD8544_SCREEN_HEIGHT)
		return;

	uint16_t regIndex = posX + (posY / 8) * PCD8544_SCREEN_WIDTH;

	pPcd8544Handle->pFrameBuffer[regIndex] ^= (1U << (posY % 8));
}

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
void PCD8544_SetPixelColor(PCD8544_Handle* pPcd8544Handle, uint8_t posX, uint8_t posY, uint8_t isBlack) {
	if (posX >= PCD8544_SCREEN_WIDTH || posY >= PCD8544_SCREEN_HEIGHT)
		return;

	uint16_t regIndex = posX + (posY / 8) * PCD8544_SCREEN_WIDTH;

	if (isBlack)
		pPcd8544Handle->pFrameBuffer[regIndex] |= (1U << (posY % 8));
	else
		pPcd8544Handle->pFrameBuffer[regIndex] &= ~(1U << (posY % 8));
}

/*************************************\
  fn: @PCD8544_GetPixelColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: x position of the pixel
  param3 u8: y position of the pixel

  return u8: color of the pixel as a bool

  desc: returns the colour of the pixel at the specified x and y position

  note: returns 0 if posX >= width or posY >= height to avoid memory corruption

\**************************************/
uint8_t PCD8544_GetPixelColor(PCD8544_Handle* pPcd8544Handle, uint8_t posX, uint8_t posY) {
	if (posX >= PCD8544_SCREEN_WIDTH || posY >= PCD8544_SCREEN_HEIGHT)
		return 0;

	uint16_t regIndex = posX + (posY / 8) * PCD8544_SCREEN_WIDTH;

	return ((pPcd8544Handle->pFrameBuffer[regIndex] & (1U << (posY % 8))) >> (posY % 8));
}

/*************************************\
  fn: @PCD8544_FillScreenColor

  param1 PCD8544_Handle*: the handle of the display
  param2 u8: the colour to set every pixel to

  return:

  desc: sets the colour of every pixel on the screen

  note:

\**************************************/
void PCD8544_FillScreenColor(PCD8544_Handle* pPcd8544Handle, uint8_t isBlack) {
	isBlack = isBlack ? 0xFF : 0x00;
	for (uint16_t i = 0; i < PCD8544_SCREEN_SIZE; i++)
		pPcd8544Handle->pFrameBuffer[i] = isBlack;
}

/*************************************\
  fn: @PCD8544_UpdateScreen

  param1 PCD8544_Handle*: the handle of the display

  return:

  desc: update the screen with the new changes

  note: the reason why you have to update the screen is because the other display screen drawing funcs
dont actually write to the display RAM, it just writes to the buffer in the handle and this func sets the
whole buffer to the displays RAM

\**************************************/
PCD8544_Status PCD8544_UpdateScreen(PCD8544_Handle* pPcd8544Handle) {
	PCD8544_Status status = PCD8544_OK;

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, LOW);

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);
	uint8_t command = SET_XADDR_0;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	command = SET_YADDR_0;
	status = pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, &command, 1,
													 pPcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, HIGH);
	status =
		pPcd8544Handle->pTransport->SpiTransmit(pPcd8544Handle->pSpi, pPcd8544Handle->pFrameBuffer,
												PCD8544_SCREEN_SIZE, pPcd8544Handle->SpiTransmitTimeout);
	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pDcPin, LOW);
	if (status != PCD8544_OK) {
		pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);
		return status;
	}

	pPcd8544Handle->pTransport->GpioWritePin(pPcd8544Handle->pCsPin, HIGH);

	return status;
}
