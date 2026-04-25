#include "Pcd8544.hpp"
#include "pcd8544_driver.h"

Pcd8544::Pcd8544(Config config) {
	this->handle.Spi = config.Spi;
	this->handle.DcPin = config.DcPin;
	this->handle.ResPin = config.ResPin;
	this->handle.CsPin = config.CsPin;
	this->handle.LedPin = config.LedPin;
	this->handle.VccPin = config.VccPin;
	this->handle.Drivers = config.Drivers;
	this->handle.SpiTransmitTimeout = config.SpiTransmitTimeout;
}

void Pcd8544::Init() {
	Pcd8544_Init(&this->handle);
}

void Pcd8544::Deinit() {
	Pcd8544_Deinit(&this->handle);
}

void Pcd8544::SetBacklight(bool isEnabled) {
	Pcd8544_SetBacklight(&this->handle, isEnabled);
}

void Pcd8544::SetSleepMode(bool isEnabled) {
	Pcd8544_SetSleepMode(&this->handle, isEnabled);
}

void Pcd8544::SetDisplayMode(Screen::Mode displayMode) {
	Pcd8544_SetDisplayMode(&this->handle, static_cast<uint8_t>(displayMode));
}

void Pcd8544::SetTempCoeff(TempCoeff coefficient) {
	Pcd8544_SetTempCoeff(&this->handle, static_cast<uint8_t>(coefficient));
}

void Pcd8544::SetContrast(uint8_t contrast) {
	Pcd8544_SetContrast(&this->handle, contrast);
}

void Pcd8544::TogglePixelColor(uint8_t posX, uint8_t posY) {
	Pcd8544_TogglePixelColor(&this->handle, posX, posY);
}

void Pcd8544::SetPixelColor(uint8_t posX, uint8_t posY, Color color) {
	Pcd8544_SetPixelColor(&this->handle, posX, posY, static_cast<uint8_t>(color));
}

Pcd8544::Color Pcd8544::GetPixelColor(uint8_t posX, uint8_t posY) {
	return static_cast<Pcd8544::Color>(Pcd8544_GetPixelColor(&this->handle, posX, posY));
}

void Pcd8544::FillScreenColor(Color color) {
	Pcd8544_FillScreenColor(&this->handle, static_cast<uint8_t>(color));
}

void Pcd8544::UpdateScreen() {
	Pcd8544_UpdateScreen(&this->handle);
}

void Pcd8544::Init(void* pSelf) {
	Pcd8544_Init(&static_cast<Pcd8544*>(pSelf)->handle);
}

void Pcd8544::Deinit(void* pSelf) {
	Pcd8544_Deinit(&static_cast<Pcd8544*>(pSelf)->handle);
}

void Pcd8544::SetBacklight(void* pSelf, bool isEnabled) {
	Pcd8544_SetBacklight(&static_cast<Pcd8544*>(pSelf)->handle, isEnabled);
}

void Pcd8544::SetSleepMode(void* pSelf, bool isEnabled) {
	Pcd8544_SetSleepMode(&static_cast<Pcd8544*>(pSelf)->handle, isEnabled);
}

void Pcd8544::SetDisplayMode(void* pSelf, Screen::Mode displayMode) {
	Pcd8544_SetDisplayMode(&static_cast<Pcd8544*>(pSelf)->handle, static_cast<uint8_t>(displayMode));
}

void Pcd8544::SetTempCoeff(void* pSelf, TempCoeff coefficient) {
	Pcd8544_SetTempCoeff(&static_cast<Pcd8544*>(pSelf)->handle, static_cast<uint8_t>(coefficient));
}

void Pcd8544::SetContrast(void* pSelf, uint8_t contrast) {
	Pcd8544_SetContrast(&static_cast<Pcd8544*>(pSelf)->handle, contrast);
}

void Pcd8544::TogglePixelColor(void* pSelf, uint8_t posX, uint8_t posY) {
	Pcd8544_TogglePixelColor(&static_cast<Pcd8544*>(pSelf)->handle, posX, posY);
}

void Pcd8544::SetPixelColor(void* pSelf, uint8_t posX, uint8_t posY, Color color) {
	Pcd8544_SetPixelColor(&static_cast<Pcd8544*>(pSelf)->handle, posX, posY,
						  static_cast<uint8_t>(color));
}

Pcd8544::Color Pcd8544::GetPixelColor(void* pSelf, uint8_t posX, uint8_t posY) {
	return static_cast<Color>(Pcd8544_GetPixelColor(&static_cast<Pcd8544*>(pSelf)->handle, posX, posY));
}

void Pcd8544::FillScreenColor(void* pSelf, Color color) {
	Pcd8544_FillScreenColor(&static_cast<Pcd8544*>(pSelf)->handle, static_cast<uint8_t>(color));
}

void Pcd8544::UpdateScreen(void* pSelf) {
	Pcd8544_UpdateScreen(&static_cast<Pcd8544*>(pSelf)->handle);
}
