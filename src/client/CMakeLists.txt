CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT( openwarclient )

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fstrict-aliasing -pipe -D__NO_INLINE__ -fno-rtti -D_IRR_STATIC_LIB_ \
-DASIO_STANDALONE ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -O0 -g -pg ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG ")



INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(${OPENWARCLIENT_LIB_INCLUDE_DIRS})

SET( OPENWARCLIENT_SOURCE_FILES ./main.cpp
                                ./OWClient.cpp
                                ./login_sequence.cpp
                                ./core/marshalling.cpp
                                ./network/NetworkSocket.cpp
                                ./world/CZone.cpp
  )


ADD_EXECUTABLE(openwarclient ${OPENWARCLIENT_SOURCE_FILES})  
ADD_DEPENDENCIES(openwarclient irrlicht)
#TARGET_LINK_LIBRARIES(openwarclient irrlicht ssl crypto pthread z)
TARGET_LINK_LIBRARIES(openwarclient irrlicht)


SET( OPENWARCLIENT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/client/")

set_target_properties( openwarclient 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OPENWARCLIENT_OUTPUT_DIR}
   )
