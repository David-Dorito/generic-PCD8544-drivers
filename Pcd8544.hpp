#ifndef PCD8544_HPP_
#define PCD8544_HPP_

extern "C" {
#include <cstring>
#include "pcd8544_driver.h"
}

class Pcd8544 {
  public:
	struct Screen {
		static constexpr uint8_t  Width{PCD8544_SCREEN_WIDTH};
		static constexpr uint8_t  Height{PCD8544_SCREEN_HEIGHT};
		static constexpr uint16_t FrameBufferSize{PCD8544_SCREEN_SIZE};

		struct Contrast {
			static constexpr uint8_t Default{PCD8544_CONTRAST_DEFAULT};
			static constexpr uint8_t Min{PCD8544_CONTRAST_MIN};
			static constexpr uint8_t Max{PCD8544_CONTRAST_MAX};
		};

		enum class Mode {
			Blank = PCD8544_DISPLAYMODE_BLANK,
			Normal = PCD8544_DISPLAYMODE_NORMAL,
			AllSegOn = PCD8544_DISPLAYMODE_ALLSEGON,
			Inverse = PCD8544_DISPLAYMODE_INVERSE
		};
	};

	enum class TempCoeff {
		mV01 = PCD8544_TEMPCOEFF_01MV,
		mv09 = PCD8544_TEMPCOEFF_09MV,
		mV17 = PCD8544_TEMPCOEFF_17MV,
		mV24 = PCD8544_TEMPCOEFF_24MV
	};

	enum class Color {
		White,
		Black
	};

	using PlatformDrivers = Pcd8544_PlatformDrivers;
	using Handle = Pcd8544_Handle;

	struct Config {
		void*			 Spi;
		void*			 DcPin;
		void*			 ResPin;
		void*			 CsPin;
		void*			 LedPin;
		void*			 VccPin;
		PlatformDrivers* Drivers;
		uint32_t		 SpiTransmitTimeout;
	};

	Pcd8544(Config config);

	void  Init();
	void  Deinit();
	void  SetBacklight(bool isEnabled);
	void  SetSleepMode(bool isEnabled);
	void  SetDisplayMode(Screen::Mode displayMode);
	void  SetTempCoeff(TempCoeff coefficient);
	void  SetContrast(uint8_t contrast);
	void  TogglePixelColor(uint8_t posX, uint8_t posY);
	void  SetPixelColor(uint8_t posX, uint8_t posY, Color color);
	Color GetPixelColor(uint8_t posX, uint8_t posY);
	void  FillScreenColor(Color color);
	void  UpdateScreen();

	static void	 Init(void* self);
	static void	 Deinit(void* self);
	static void	 SetBacklight(void* self, bool isEnabled);
	static void	 SetSleepMode(void* self, bool isEnabled);
	static void	 SetDisplayMode(void* self, Screen::Mode displayMode);
	static void	 SetTempCoeff(void* self, TempCoeff coefficient);
	static void	 SetContrast(void* self, uint8_t contrast);
	static void	 TogglePixelColor(void* self, uint8_t posX, uint8_t posY);
	static void	 SetPixelColor(void* self, uint8_t posX, uint8_t posY, Color color);
	static Color GetPixelColor(void* self, uint8_t posX, uint8_t posY);
	static void	 FillScreenColor(void* self, Color color);
	static void	 UpdateScreen(void* self);

	uint8_t* FrameBuffer() {
		return this->handle.FrameBuffer;
	}
	void FrameBuffer(uint8_t* FrameBuffer) {
		memcpy(this->handle.FrameBuffer, FrameBuffer, Screen::FrameBufferSize);
	}

	static uint8_t* FrameBuffer(void* self) {
		return static_cast<Pcd8544*>(self)->handle.FrameBuffer;
	}
	static void FrameBuffer(void* self, uint8_t* FrameBuffer) {
		memcpy(static_cast<Pcd8544*>(self)->handle.FrameBuffer, FrameBuffer, Screen::FrameBufferSize);
	}

  private:
	Handle handle{};
};

#endif
