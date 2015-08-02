CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT( mypexptr )

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CPP_FLAGS ${CMAKE_CPP_FLAGS} "-Wall")
set(CMAKE_CPP_FLAGS_DEBUG "${CMAKE_CPP_FLAGS_DEBUG} -O0 -g -pg")
set(CMAKE_CPP_FLAGS_RELEASE "${CMAKE_CPP_FLAGS_RELEASE} -O2 -DNDEBUG ")


INCLUDE_DIRECTORIES(./src)
SET( MYPEXPTR_SOURCE_FILES  mypexptr_main.cpp 
                  MYPFile.cpp 
  )

SET( MYPHASH_SOURCE_FILES myphash_main.cpp 
  )

SET( MYPNAME_SOURCE_FILES mypname_main.cpp 
                  names.cpp
  )

ADD_EXECUTABLE(myphash ${MYPHASH_SOURCE_FILES} )  
ADD_EXECUTABLE(mypexptr ${MYPEXPTR_SOURCE_FILES} )  
ADD_EXECUTABLE(mypname ${MYPNAME_SOURCE_FILES} )  

TARGET_LINK_LIBRARIES(mypexptr z)

SET( MYPTOOLS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/tools/myptools")

set_target_properties( mypexptr 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MYPTOOLS_OUTPUT_DIR}
    )

set_target_properties( myphash 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MYPTOOLS_OUTPUT_DIR}
    )

set_target_properties( mypname 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MYPTOOLS_OUTPUT_DIR}
    )

