# Setting Core source files
set(CORE_SOURCES
	"Core/App/App.cpp"
	"Core/Log/Log.cpp"
	"Core/Platform/Platform.cpp"
	
	# TODO: change this
	"Core/Platform/Windows/PlatformWindows.cpp"
)

# Setting Core header files
set(CORE_HEADERS 
	"Core/Core.hpp"
	"Core/App/App.hpp"
	"Core/EntryPoint/EntryPoint.hpp"
	"Core/Log/Log.hpp"
	"Core/Platform/Platform.hpp"

	# TODO: change this
	"Core/Platform/Windows/PlatformWindows.hpp"
)

add_library(Drop STATIC 
	${CORE_SOURCES}
	${CORE_HEADERS}
)

target_link_libraries(Drop PRIVATE fmt spdlog)

target_include_directories(Drop PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/Core/
	#${CMAKE_CURRENT_SOURCE_DIR}/Core/Platform/
)
