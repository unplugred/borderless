cmake_minimum_required(VERSION 3.16)

project(Borderless VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo" CACHE STRING "" FORCE)
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS
	$<$<CONFIG:RELWITHDEBINFO>:QT_MESSAGELOGCONTEXT>
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

set(app_icon_resource_windows "dist/icon.rc")
set(app_icon_macos "dist/icon.icns")
qt_add_executable(borderless
	"source/main.cpp"
	"source/mainwindow.cpp"
	"source/mainwindow.h"
	"source/mainwindow.ui"
	${app_icon_resource_windows}
	${app_icon_macos}
)
qt_add_resources(borderless imageresources
	PREFIX "/"
	FILES "dist/icon.png"
	FILES "source/init.png"
)

target_link_libraries(borderless Qt6::Widgets)

set_target_properties(borderless PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
	MACOSX_BUNDLE_INFO_PLIST "dist/Info.plist"
	MACOSX_BUNDLE_GUI_IDENTIFIER com.unplugred.borderless
	MACOSX_BUNDLE_LONG_VERSION_STRING ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
	MAXOSX_BUNDLE_ICON_FILE icon.icns
)

qt_finalize_target(borderless)
