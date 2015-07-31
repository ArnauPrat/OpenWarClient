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
SET(SOURCE_FILES  main.cpp 
                  MYPFile.cpp 
  )

ADD_EXECUTABLE(mypexptr ${SOURCE_FILES} )  
TARGET_LINK_LIBRARIES(mypexptr z)

set_target_properties( mypexptr 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tools"
    )



