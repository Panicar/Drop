cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED DROP_ENGINE_DIR)
	set(DROP_ENGINE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif() 

# Setting the version of the project
if(NOT VERSION)
	if(METADATA_VERSION)
		set(VERSION ${METADATA_VERSION})
	else()
		set(VERSION "1.0.0.0")		
	endif()
endif()
set(METADATA_VERSION ${VERSION} CACHE STRING "Version number for executable metadata" FORCE)

# Definition of the project
set(METADATA_PROJECT_NAME "DROP" CACHE STRING "Name of the Project")
project("${MEATADATA_PROJECT_NAME}" VERSION "${METADATA_VERSION}" LANGUAGES C CXX)


if(MSVC)
    add_compile_options(/EHsc)  # Enable C++ exception handling with unwind semantics
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DP_DEV_MODE)
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_compile_definitions(DP_PROFILE_MODE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_definitions(DP_MASTER_MODE)
endif()


# Todo: move these to initial setup cmake
# Setting C++ Standards
set(CMAKE_CXX_STANDARD 20)

# Must be passed from CMakePresets
if(NOT DEFINED CMAKE_OUTPUT_BASE_DIR)
  message(FATAL_ERROR "CMAKE_OUTPUT_BASE_DIR must be defined via preset")
endif()

# Configure binary output locations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_OUTPUT_BASE_DIR}/Bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_OUTPUT_BASE_DIR}/Bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_OUTPUT_BASE_DIR}/Bin")

# Modules
#list(APPEND CMAKE_MODULE_PATH "{CMAKE_SOURCE_DIR}/CMake")
#--------------------------------------------------------------#


# Add ThirdParty Dependencies
add_subdirectory(ThirdParty)

# Add Core Modules
add_subdirectory(Source)

# Add framework tests
add_subdirectory(Sandbox)