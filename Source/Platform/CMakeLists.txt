# Setting source files
set(PLATFORM_SOURCES
	
)

# Setting header files
set(PLATFORM_HEADERS 
	"Platform.hpp"
)

# Define the Core Library
add_library(Platform STATIC 
	${PLATFORM_SOURCES}
	${PLATFORM_HEADERS}
)

target_link_libraries(Platform PRIVATE Core)

target_include_directories(Platform PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
)
