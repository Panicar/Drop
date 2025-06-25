# Setting source files for the test executable
set( APP_SOURCES
	"Sandbox.cpp"
)

# Setting header files
set ( APP_HEADERS
	"Sandbox.hpp"
)

# Creating the executables
add_executable(Sandbox
	${APP_SOURCES}
	${APP_HEADERS}
)

# Link the framework's Core module
target_link_libraries(Sandbox PRIVATE	
	fmt
	spdlog
	Drop
)

target_include_directories(
	Sandbox PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/
	${DROP_ENGINE_DIR}/Source/Drop/
)