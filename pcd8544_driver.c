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

  param1 Pcd8544_Handle*: the handle of the display

  return:

  desc: turns the display on using the sequence described in the datasheet and sets some basic
configurations

  note: sets the contrast to PCD8544_CONTRAST_DEFAULT found in the header file

\**************************************/
Pcd8544_Status PCD8544_Init(Pcd8544_Handle* pcd8544Handle) {
	Pcd8544_Status status = PCD8544_OK;

	// Control VCC if user provides the pin, otherwise assume external 3.3V
	if (pcd8544Handle->VccPin != NULL) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, HIGH);
		pcd8544Handle->Drivers->Delay(10); // Wait for VCC to stabilize
	}

	// Reset sequence
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->ResPin, LOW);
	pcd8544Handle->Drivers->Delay(1); // Min 100ns, 1ms is safe
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->ResPin, HIGH);
	pcd8544Handle->Drivers->Delay(50); // Wait for internal reset

	// Send configuration commands
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, LOW);
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);

	uint8_t command = SET_EXTENDINST;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		if (pcd8544Handle->VccPin != NULL)
			pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
		return status;
	}

	command = (SET_VOP | PCD8544_CONTRAST_DEFAULT);
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		if (pcd8544Handle->VccPin != NULL)
			pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
		return status;
	}

	command = SET_TC;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		if (pcd8544Handle->VccPin != NULL)
			pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
		return status;
	}

	command = SET_BIAS_1_48;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		if (pcd8544Handle->VccPin != NULL)
			pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
		return status;
	}

	command = SET_BASICINST;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		if (pcd8544Handle->VccPin != NULL)
			pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
		return status;
	}

	command = PCD8544_DISPLAYMODE_NORMAL;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		if (pcd8544Handle->VccPin != NULL)
			pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_Deinit

  param1 Pcd8544_Handle*: the handle of the display

  return:

  desc: turns the display off

  note: also pulls chip select to HIGH and Data/Command to LOW

\**************************************/
void PCD8544_Deinit(Pcd8544_Handle* pcd8544Handle) {
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);
	if (pcd8544Handle->LedPin != NULL)
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->LedPin, LOW);
	if (pcd8544Handle->VccPin != NULL)
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->VccPin, LOW);
}

/*************************************\
  fn: @PCD8544_SetBacklight

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: enable or disable sleep mode of the display

  return:

  desc: enables or disables the background light of the display

  note: function is very basic, just a wrapper for GpioWritePin() with the LED pin as the arg

\**************************************/
void PCD8544_SetBacklight(Pcd8544_Handle* pcd8544Handle, uint8_t isEnabled) {
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->LedPin, !isEnabled);
}

/*************************************\
  fn: @PCD8544_SetSleepMode

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: enable or disable sleep mode of the display

  return:

  desc: enables or disables the sleep mode of the display

  note: display will stop displaying an image if turned on but RAM will NOT get cleared,
		so once you turn off sleep mode it will start displaying the image again

\**************************************/
Pcd8544_Status PCD8544_SetSleepMode(Pcd8544_Handle* pcd8544Handle, uint8_t isEnabled) {
	Pcd8544_Status status = PCD8544_OK;

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, LOW);

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);
	if (isEnabled)
		isEnabled = SET_POWERDOWN;
	else
		isEnabled = SET_POWERUP;

	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &isEnabled, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_SetDisplayMode

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: new mode of the display

  return:

  desc: sets the mode of the display

  note: use the PCD8544_DISPLAYMODE_XXX macros as the mode arg, view the datasheet for a description of
what they do

\**************************************/
Pcd8544_Status PCD8544_SetDisplayMode(Pcd8544_Handle* pcd8544Handle, uint8_t mode) {
	Pcd8544_Status status = PCD8544_OK;

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, LOW);

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);
	mode &= (SET_DISPLAYMODE | 0b00000101);
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &mode, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_SetTempCoeff

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: the temperature coefficient

  return:

  desc: sets the temperature coefficient of the display

  note: use the PCD8544_TEMPCOEFF_XXX macros as the coefficient arg, view the datasheet for a description
of what they do

\**************************************/
Pcd8544_Status PCD8544_SetTempCoeff(Pcd8544_Handle* pcd8544Handle, uint8_t coefficient) {
	Pcd8544_Status status = PCD8544_OK;

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, LOW);

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);
	coefficient &= (SET_TC | 00000011);
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &coefficient, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_SetContrast

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: new contrast of the display

  return:

  desc: sets the contrast of the display

  note:

\**************************************/
Pcd8544_Status PCD8544_SetContrast(Pcd8544_Handle* pcd8544Handle, uint8_t contrast) {
	Pcd8544_Status status = PCD8544_OK;

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, LOW);

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);

	uint8_t command = SET_EXTENDINST;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	command = (SET_VOP | contrast);
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	command = SET_BASICINST;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);

	return status;
}

/*************************************\
  fn: @PCD8544_TogglePixelColor

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: x position of the pixel
  param3 u8: y position of the pixel

  return:

  desc: toggles the colour of the pixel at the specified x and y position

  note: early returns nothing to avoid memory corruption if posX >= width or posY >= height

\**************************************/
void PCD8544_TogglePixelColor(Pcd8544_Handle* pcd8544Handle, uint8_t posX, uint8_t posY) {
	if (posX >= PCD8544_SCREEN_WIDTH || posY >= PCD8544_SCREEN_HEIGHT)
		return;

	uint16_t regIndex = posX + (posY / 8) * PCD8544_SCREEN_WIDTH;

	pcd8544Handle->FrameBuffer[regIndex] ^= (1U << (posY % 8));
}

/*************************************\
  fn: @PCD8544_SetPixelColor

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: the colour of the pixel
  param3 u8: x position of the pixel
  param4 u8: y position of the pixel

  return:

  desc: sets the colour of the pixel at the specified x and y position

  note: early returns nothing to avoid memory corruption if posX >= width or posY >= height

\**************************************/
void PCD8544_SetPixelColor(Pcd8544_Handle* pcd8544Handle, uint8_t posX, uint8_t posY, uint8_t isBlack) {
	if (posX >= PCD8544_SCREEN_WIDTH || posY >= PCD8544_SCREEN_HEIGHT)
		return;

	uint16_t regIndex = posX + (posY / 8) * PCD8544_SCREEN_WIDTH;

	if (isBlack)
		pcd8544Handle->FrameBuffer[regIndex] |= (1U << (posY % 8));
	else
		pcd8544Handle->FrameBuffer[regIndex] &= ~(1U << (posY % 8));
}

/*************************************\
  fn: @PCD8544_GetPixelColor

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: x position of the pixel
  param3 u8: y position of the pixel

  return u8: color of the pixel as a bool

  desc: returns the colour of the pixel at the specified x and y position

  note: returns 0 if posX >= width or posY >= height to avoid memory corruption

\**************************************/
uint8_t PCD8544_GetPixelColor(Pcd8544_Handle* pcd8544Handle, uint8_t posX, uint8_t posY) {
	if (posX >= PCD8544_SCREEN_WIDTH || posY >= PCD8544_SCREEN_HEIGHT)
		return 0;

	uint16_t regIndex = posX + (posY / 8) * PCD8544_SCREEN_WIDTH;

	return ((pcd8544Handle->FrameBuffer[regIndex] & (1U << (posY % 8))) >> (posY % 8));
}

/*************************************\
  fn: @PCD8544_FillScreenColor

  param1 Pcd8544_Handle*: the handle of the display
  param2 u8: the colour to set every pixel to

  return:

  desc: sets the colour of every pixel on the screen

  note:

\**************************************/
void PCD8544_FillScreenColor(Pcd8544_Handle* pcd8544Handle, uint8_t isBlack) {
	isBlack = isBlack ? 0xFF : 0x00;
	for (uint16_t i = 0; i < PCD8544_SCREEN_SIZE; i++)
		pcd8544Handle->FrameBuffer[i] = isBlack;
}

/*************************************\
  fn: @PCD8544_UpdateScreen

  param1 Pcd8544_Handle*: the handle of the display

  return:

  desc: update the screen with the new changes

  note: the reason why you have to update the screen is because the other display screen drawing funcs
dont actually write to the display RAM, it just writes to the buffer in the handle and this func sets the
whole buffer to the displays RAM

\**************************************/
Pcd8544_Status PCD8544_UpdateScreen(Pcd8544_Handle* pcd8544Handle) {
	Pcd8544_Status status = PCD8544_OK;

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, LOW);

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);
	uint8_t command = SET_XADDR_0;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	command = SET_YADDR_0;
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, &command, 1,
												 pcd8544Handle->SpiTransmitTimeout);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, HIGH);
	status = pcd8544Handle->Drivers->SpiTransmit(pcd8544Handle->Spi, pcd8544Handle->FrameBuffer,
												 PCD8544_SCREEN_SIZE, pcd8544Handle->SpiTransmitTimeout);
	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->DcPin, LOW);
	if (status != PCD8544_OK) {
		pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);
		return status;
	}

	pcd8544Handle->Drivers->GpioWritePin(pcd8544Handle->CsPin, HIGH);

	return status;
}
