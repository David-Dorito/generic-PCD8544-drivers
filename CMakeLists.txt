cmake_minimum_required(VERSION 4.3.0)

project(pcd8544_drivers LANGUAGES C)

add_library(${PROJECT_NAME})

option(PCD8544_DRIVERS_CXX_ENABLED "Enable the C++ wrapper for the PCD8544 drivers" "OFF")

target_compile_features(${PROJECT_NAME}
PRIVATE
	c_std_99
)

target_sources(${PROJECT_NAME}
PRIVATE
	pcd8544_driver.c

PUBLIC
	FILE_SET HEADERS
	FILES
		pcd8544_driver.h
)

if(PCD8544_DRIVERS_CXX_ENABLED)
	enable_language(CXX)

	target_compile_features(${PROJECT_NAME}
	PRIVATE
		cxx_std_17
	)

	target_sources(${PROJECT_NAME}
	PRIVATE
		Pcd8544.cpp

	PUBLIC
		FILE_SET HEADERS
		FILES
			Pcd8544.hpp
	)
endif()
