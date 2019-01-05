# Try to find the FFI librairies
# FFI_FOUND - system has FFI lib
# FFI_INCLUDE_DIR - the FFI include directory
# FFI_LIBRARIES - Libraries needed to use FFI

if (FFI_INCLUDE_DIRS AND FFI_LIBRARIES)
    # Already in cache, be silent
    set(FFI_FIND_QUIETLY TRUE)
endif (FFI_INCLUDE_DIRS AND FFI_LIBRARIES)

find_path(FFI_INCLUDE_DIR NAMES ffi.h
    PATHS
    ${CMAKE_INSTALL_PREFIX}/include
    ${KDE4_INCLUDE_DIR}
    /usr/include)
find_library(FFI_LIBRARIES NAMES ffi libffi )
SET(FFI_INCLUDE_DIRS ${FFI_INCLUDE_DIR})


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FFI DEFAULT_MSG FFI_INCLUDE_DIRS FFI_LIBRARIES)

mark_as_advanced(FFI_INCLUDE_DIRS FFI_LIBRARIES)
