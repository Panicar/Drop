# Setting source files
set(CORE_SOURCES
	"App/App.cpp"
	"Log/Log.cpp"
	"Platform/Platform.cpp"
	
	# TODO: change this
	"Platform/Windows/PlatformWindows.cpp"
)

# Setting header files
set(CORE_HEADERS 
	"Core.hpp"
	"App/App.hpp"
	"App/AppConfig.hpp"
	"EntryPoint/EntryPoint.hpp"
	"Log/Log.hpp"
	"Platform/Platform.hpp"

	# TODO: change this
	"Platform/Windows/PlatformWindows.hpp"
)

# Define the Core Library
add_library(Core STATIC 
	${CORE_SOURCES}
	${CORE_HEADERS}
)

target_link_libraries(Core PRIVATE fmt spdlog)

target_include_directories(Core PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
)
