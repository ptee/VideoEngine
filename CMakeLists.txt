##############################################################################
# Project: Video & image sequence player using multithreading to
#          separate the main worker/processor thread and the GUI. 
# Author: Pattreeya Tanisaro
# Date: 20.03.2014
# 
##############################################################################

cmake_minimum_required(VERSION 2.8)

project ( viden )
set (TARGET_NAME "viden")

## General CMake setttings
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -std=c++0x") # -fPIC or -fPIE
set(CMAKE_POSITION_INDEPENDENT_CODE ON )
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build-cmake )
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")


## 3rd Party Libraries
find_package( OpenCV REQUIRED )
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
include_directories(${Qt5Core_INCLUDES})
include_directories(${Qt5Gui_INCLUDES})
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${OpenCV_INCLUDES})

## General Include Files
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/inc  )
include_directories( ${PROJECT_SOURCE_DIR} )

set( SRC 
  src/GeneralDefs.cpp
  src/ImageDefs.cpp
  src/ImagePlayer.cpp
  src/VideoDefs.cpp
  src/VideoPlayer.cpp
  src/VideoUtils.cpp
  )


add_library( ${TARGET_NAME} STATIC ${SRC} )

target_link_libraries( ${TARGET_NAME}  ${OpenCV_LIBS} Qt5::Core Qt5::Gui Qt5::Widgets )


##################################END OF FILE##############################

