# Setting source files
set(CORE_SOURCES
	"Application/Application.cpp"
	"Log/Log.cpp"
)

# Setting header files
set(CORE_HEADERS 
	"Application/Application.hpp"
	"Platform/Platform.hpp"
	"EntryPoint/EntryPoint.hpp"
	"Log/Log.hpp"
)

# Define the Core Library
add_library(Core STATIC 
	${CORE_SOURCES}
	${CORE_HEADERS}
)

target_link_libraries(Core PRIVATE fmt spdlog)

target_include_directories(Core PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/Source
)
