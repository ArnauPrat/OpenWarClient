CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT( openwarclient )

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CPP_FLAGS ${CMAKE_CPP_FLAGS} "-Wall")
set(CMAKE_CPP_FLAGS_DEBUG "${CMAKE_CPP_FLAGS_DEBUG} -O0 -g -pg")
set(CMAKE_CPP_FLAGS_RELEASE "${CMAKE_CPP_FLAGS_RELEASE} -O2 -DNDEBUG ")


INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${OPENWARCLIENT_LIB_INCLUDE_DIRS})
SET( OPENWARCLIENT_SOURCE_FILES main.cpp
                                OWClient.cpp
                                ./common/marshalling.cpp
                                ./io/Properties.cpp

  
  )


ADD_EXECUTABLE(openwarclient ${OPENWARCLIENT_SOURCE_FILES})  
TARGET_LINK_LIBRARIES(openwarclient ssl crypto)


SET( OPENWARCLIENT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/client/")

set_target_properties( openwarclient 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OPENWARCLIENT_OUTPUT_DIR}
   )
