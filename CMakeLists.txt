cmake_minimum_required(VERSION 3.16)

project(Borderless VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

set(PROJ_DIR "${CMAKE_SOURCE_DIR}/source")

#include_directories(${PROJ_DIR})

file(GLOB_RECURSE SOURCES
	"${PROJ_DIR}/*.ui"
	"${PROJ_DIR}/*.qrc"
	"${PROJ_DIR}/*.h"
	"${PROJ_DIR}/*.cpp"
)

#set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
#	$<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
#)

#set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${PROJ_DIR})

add_executable(borderless ${SOURCES})

#target_include_directories(borderless PRIVATE ${PROJ_DIR})

target_link_libraries(borderless Qt6::Widgets)

set_target_properties(borderless PROPERTIES
	WIN32_EXECUTABLE ON
	MACOS_BUNDLE ON
)
