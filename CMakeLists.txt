cmake_minimum_required (VERSION 3.8)

project ("JoystickCamProfiler")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
	"jcp/source/main.cpp"
    "libs/mathplot/mathplot.cpp")

find_package(wxWidgets REQUIRED base core propgrid OPTIONAL_COMPONENTS )

set(CMAKE_CXX_FLAGS "-O0 -Wall -Wextra")

if(wxWidgets_FOUND)
	include(${wxWidgets_USE_FILE})
	add_executable (JoystickCamProfiler ${SOURCES})
	#set_target_properties(JoystickCamProfiler PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

	target_include_directories(JoystickCamProfiler
		PRIVATE
		"${PROJECT_SOURCE_DIR}/jcp"
		"${PROJECT_SOURCE_DIR}/libs"
	)

	target_link_libraries(JoystickCamProfiler ${wxWidgets_LIBRARIES})

endif(wxWidgets_FOUND)
