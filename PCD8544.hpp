#ifndef PCD8544_HPP_
#define PCD8544_HPP_

extern "C" {
#include <cstring>
#include "pcd8544_driver.h"
}

class PCD8544 {
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

	using Transport = PCD8544_Transport;
	using Handle = PCD8544_Handle;

	struct Config {
		void*	   pSpi;
		void*	   pDcPin;
		void*	   pResPin;
		void*	   pCsPin;
		void*	   pLedPin;
		void*	   pVccPin;
		Transport* pTransport;
		uint32_t   SpiTransmitTimeout;
	};

	PCD8544(Config config);

	void  Init();
	void  Deinit();
	void  SetBacklight(bool isEnabled);
	void  SetSleepMode(bool isEnabled);
	void  SetDisplayMode(Screen::Mode displayMode);
	void  SetTempCoeff(TempCoeff coefficient);
	void  SetContrast(uint8_t contrast);
	void  TogglePixelColor(uint8_t posX, uint8_t posY);
	void  SetPixelColor(Color color, uint8_t posX, uint8_t posY);
	Color GetPixelColor(uint8_t posX, uint8_t posY);
	void  FillScreenColor(Color color);
	void  UpdateScreen();

	static void	 Init(void* pSelf);
	static void	 Deinit(void* pSelf);
	static void	 SetBacklight(void* pSelf, bool isEnabled);
	static void	 SetSleepMode(void* pSelf, bool isEnabled);
	static void	 SetDisplayMode(void* pSelf, Screen::Mode displayMode);
	static void	 SetTempCoeff(void* pSelf, TempCoeff coefficient);
	static void	 SetContrast(void* pSelf, uint8_t contrast);
	static void	 TogglePixelColor(void* pSelf, uint8_t posX, uint8_t posY);
	static void	 SetPixelColor(void* pSelf, Color color, uint8_t posX, uint8_t posY);
	static Color GetPixelColor(void* pSelf, uint8_t posX, uint8_t posY);
	static void	 FillScreenColor(void* pSelf, Color color);
	static void	 UpdateScreen(void* pSelf);

	uint8_t* FrameBuffer() {
		return this->handle.pFrameBuffer;
	}
	void FrameBuffer(uint8_t* pFrameBuffer) {
		memcpy(this->handle.pFrameBuffer, pFrameBuffer, Screen::FrameBufferSize);
	}

	static uint8_t* FrameBuffer(void* pSelf) {
		return static_cast<PCD8544*>(pSelf)->handle.pFrameBuffer;
	}
	static void FrameBuffer(void* pSelf, uint8_t* pFrameBuffer) {
		memcpy(static_cast<PCD8544*>(pSelf)->handle.pFrameBuffer, pFrameBuffer, Screen::FrameBufferSize);
	}

  private:
	Handle handle{};
};

#endif
