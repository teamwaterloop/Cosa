if (COSA_IS_TOOLCHAIN_PROCESSED)
    return()
endif ()
set(COSA_IS_TOOLCHAIN_PROCESSED True)
set(CMAKE_SYSTEM_NAME Cosa)

# Cosa requires C++11 standard
set(CMAKE_CXX_STANDARD 11)

# Set compilers
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# Include Cosa.cmake in module path
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/Platform/Cosa.cmake)
    message(STATUS "Setting module path with Cosa.cmake")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})
endif ()

# Include system paths
if (UNIX)
    message(STATUS "Including Unix paths")
    include(Platform/UnixPaths)
    if (APPLE)
        message(STATUS "Including Darwin paths")
        list(APPEND CMAKE_SYSTEM_PREFIX_PATH ~/Applications
                /Applications
                /Developer/Applications
                /sw         # Fink
                /opt/local) # MacPorts
    endif ()
elseif (WIN32)
    message(STATUS "Including Windows paths")
    include(Platform/WindowsPaths)
endif ()

# Set platform path to Cosa and arduino-cmake
set()

if (NOT ${ARDUINO_CMAKE_PATH})
    message(FATAL_ERROR "ARDUINO_CMAKE_PATH is not defined")
endif()

set(COSA_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../core")
message(STATUS "`cosa`          location: ${COSA_SDK_PATH}")
message(STATUS "`arduino-cmake` location: ${ARDUINO_CMAKE_PATH}")
