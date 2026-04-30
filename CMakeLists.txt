cmake_minimum_required(VERSION 3.15)

project(PCD8544_Drivers C CXX)

add_library(${PROJECT_NAME}
	pcd8544_driver.c
	Pcd8544.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/
)
