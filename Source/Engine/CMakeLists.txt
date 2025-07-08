# Setting Core source files
set(CORE_SOURCES
	"Core/App/App.cpp"
	"Core/Log/Log.cpp"
	"Core/Platform/Base/Platform.cpp"
	"Core/Platform/Base/Window.cpp"
)

# Setting Core header files
set(CORE_HEADERS 
	"Core/Core.hpp"
	"Core/App/App.hpp"
	"Core/EntryPoint/EntryPoint.hpp"
	"Core/Log/Log.hpp"
	"Core/Platform/Base/Platform.hpp"
	"Core/Platform/Base/Window.hpp"
)

if(WIN32)
	message(STATUS "Compiling for Windows")
	list(APPEND CORE_SOURCES
		"Core/Platform/Windows/WindowsPlatform.cpp"
		"Core/Platform/Windows/WindowsWindow.cpp"
	)

	list(APPEND CORE_HEADERS
		"Core/Platform/Windows/WindowsPlatform.hpp"
		"Core/Platform/Windows/WindowsWindow.hpp"
	)

elseif(APPLE)
	message(STATUS "Compiling for macOS")
elseif(UNIX)
	message(STATUS "Compiling for Linux or other Unix")
endif()

add_library(Drop STATIC 
	${CORE_SOURCES}
	${CORE_HEADERS}
)

target_link_libraries(Drop PRIVATE fmt spdlog)

target_include_directories(Drop PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/Core/
	#${CMAKE_CURRENT_SOURCE_DIR}/Core/Platform/
)
