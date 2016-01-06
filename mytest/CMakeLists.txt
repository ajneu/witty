cmake_minimum_required(VERSION 2.8)
project (witty_proj)

set(CMAKE_CXX_STANDARD   11)
set(CMAKE_CXX_EXTENSIONS OFF)



###########
# Debug or Release
###########
if (NOT CMAKE_BUILD_TYPE)
  ## set default to Debug
  set(CMAKE_BUILD_TYPE Debug)  # override with -DCMAKE_BUILD_TYPE=Debug
  message("==> CMAKE_BUILD_TYPE empty. Changing it to Release.")
else()
  message("==> CMAKE_BUILD_TYPE == ${CMAKE_BUILD_TYPE}.")
endif()



###########
# Warnings
###########
if (CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable") # suppress
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")              # suppress
endif ()




find_package(Boost COMPONENTS system thread signals)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  set(libs ${libs}    ${Boost_LIBRARIES})
endif()

if(WITTY_CUSTOM_INSTALL_PATH AND   NOT "${WITTY_CUSTOM_INSTALL_PATH}" STREQUAL "")
  set(ENV{PATH} "${WITTY_CUSTOM_INSTALL_PATH}/include:${WITTY_CUSTOM_INSTALL_PATH}/lib:$ENV{PATH}")
endif()



##  FindWt.cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE( Wt REQUIRED )

INCLUDE_DIRECTORIES( ${Wt_INCLUDE_DIR} )




# target_link_libraries( yourexe
#   ${Wt_DEBUG_LIBRARY}
#   ${Wt_HTTP_DEBUG_LIBRARY}
#   ${Wt_EXT_DEBUG_LIBRARY}
#   )
#
# # or
#
# target_link_libraries( yourexe
#   ${Wt_LIBRARY}
#   ${Wt_HTTP_LIBRARY}
#   ${Wt_EXT_LIBRARY}
#   )

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../wt-3.3.5/build/install/include)
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../wt-3.3.5/build/install/lib)



set(target go)

set(src
  main.cpp
  application.cpp
)

add_executable(${target} ${src})

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_link_libraries(${target}
  ${libs}

  ${Wt_LIBRARY}                  # wt
  ${Wt_HTTP_LIBRARY}             # wthttp
)
else()
  target_link_libraries(${target}
  ${libs}

  ${Wt_DEBUG_LIBRARY}            # wtd           or wt
  ${Wt_HTTP_DEBUG_LIBRARY}       # wthttpd       or wthttp
)
endif()
