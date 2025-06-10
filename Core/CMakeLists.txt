# Setting source files
set(CORE_SOURCES
	"Source/Application/Application.cpp"
	"Source/Log/Log.cpp"
)

# Setting header files
set(CORE_HEADERS 
	"Source/Application/Application.hpp"
	"Source/EntryPoint/EntryPoint.hpp"
	"Source/Log/Log.hpp"
	"Source/Utils/Definitions.hpp"
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

