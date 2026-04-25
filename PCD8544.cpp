#include "PCD8544.hpp"
#include "pcd8544_driver.h"

PCD8544::PCD8544(Config config) {
	this->handle.pSpiHandle = config.pSpiHandle;
	this->handle.pDcPin = config.pDcPin;
	this->handle.pResPin = config.pResPin;
	this->handle.pCsPin = config.pCsPin;
	this->handle.pLedPin = config.pLedPin;
	this->handle.pVccPin = config.pVccPin;
	this->handle.pTransport = config.pTransport;
}

void PCD8544::Init() {
	PCD8544_Init(&this->handle);
}

void PCD8544::Deinit() {
	PCD8544_Deinit(&this->handle);
}

void PCD8544::SetBacklight(bool isEnabled) {
	PCD8544_SetBacklight(&this->handle, isEnabled);
}

void PCD8544::SetSleepMode(bool isEnabled) {
	PCD8544_SetSleepMode(&this->handle, isEnabled);
}

void PCD8544::SetDisplayMode(Screen::Mode displayMode) {
	PCD8544_SetDisplayMode(&this->handle, static_cast<uint8_t>(displayMode));
}

void PCD8544::SetTempCoeff(TempCoeff coefficient) {
	PCD8544_SetTempCoeff(&this->handle, static_cast<uint8_t>(coefficient));
}

void PCD8544::SetContrast(uint8_t contrast) {
	PCD8544_SetContrast(&this->handle, contrast);
}

void PCD8544::TogglePixelColor(uint8_t posX, uint8_t posY) {
	PCD8544_TogglePixelColor(&this->handle, posX, posY);
}

void PCD8544::SetPixelColor(Color color, uint8_t posX, uint8_t posY) {
	PCD8544_SetPixelColor(&this->handle, static_cast<uint8_t>(color), posX, posY);
}

PCD8544::Color PCD8544::GetPixelColor(uint8_t posX, uint8_t posY) {
	return static_cast<PCD8544::Color>(PCD8544_GetPixelColor(&this->handle, posX, posY));
}

void PCD8544::FillScreenColor(Color color) {
	PCD8544_FillScreenColor(&this->handle, static_cast<uint8_t>(color));
}

void PCD8544::UpdateScreen() {
	PCD8544_UpdateScreen(&this->handle);
}

void PCD8544::Init(void* pSelf) {
	PCD8544_Init(&static_cast<PCD8544*>(pSelf)->handle);
}

void PCD8544::Deinit(void* pSelf) {
	PCD8544_Deinit(&static_cast<PCD8544*>(pSelf)->handle);
}

void PCD8544::SetBacklight(void* pSelf, bool isEnabled) {
	PCD8544_SetBacklight(&static_cast<PCD8544*>(pSelf)->handle, isEnabled);
}

void PCD8544::SetSleepMode(void* pSelf, bool isEnabled) {
	PCD8544_SetSleepMode(&static_cast<PCD8544*>(pSelf)->handle, isEnabled);
}

void PCD8544::SetDisplayMode(void* pSelf, Screen::Mode displayMode) {
	PCD8544_SetDisplayMode(&static_cast<PCD8544*>(pSelf)->handle, static_cast<uint8_t>(displayMode));
}

void PCD8544::SetTempCoeff(void* pSelf, TempCoeff coefficient) {
	PCD8544_SetTempCoeff(&static_cast<PCD8544*>(pSelf)->handle, static_cast<uint8_t>(coefficient));
}

void PCD8544::SetContrast(void* pSelf, uint8_t contrast) {
	PCD8544_SetContrast(&static_cast<PCD8544*>(pSelf)->handle, contrast);
}

void PCD8544::TogglePixelColor(void* pSelf, uint8_t posX, uint8_t posY) {
	PCD8544_TogglePixelColor(&static_cast<PCD8544*>(pSelf)->handle, posX, posY);
}

void PCD8544::SetPixelColor(void* pSelf, Color color, uint8_t posX, uint8_t posY) {
	PCD8544_SetPixelColor(&static_cast<PCD8544*>(pSelf)->handle, static_cast<uint8_t>(color), posX,
						  posY);
}

PCD8544::Color PCD8544::GetPixelColor(void* pSelf, uint8_t posX, uint8_t posY) {
	return static_cast<Color>(PCD8544_GetPixelColor(&static_cast<PCD8544*>(pSelf)->handle, posX, posY));
}

void PCD8544::FillScreenColor(void* pSelf, Color color) {
	PCD8544_FillScreenColor(&static_cast<PCD8544*>(pSelf)->handle, static_cast<uint8_t>(color));
}

void PCD8544::UpdateScreen(void* pSelf) {
	PCD8544_UpdateScreen(&static_cast<PCD8544*>(pSelf)->handle);
}
