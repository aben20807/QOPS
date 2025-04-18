cmake_minimum_required(VERSION 3.0.0)
project(Boost-CMake)

option(BOOST_DOWNLOAD_TO_BINARY_DIR "Prefer downloading Boost to the binary directory instead of source directory" OFF)
set(BOOST_URL "https://sourceforge.net/projects/boost/files/boost/1.75.0/boost_1_75_0.tar.bz2" CACHE STRING "Boost download URL")
set(BOOST_URL_SHA256 "953db31e016db7bb207f11432bef7df100516eeb746843fa0486a222e3fd49cb" CACHE STRING "Boost download URL SHA256 checksum")
set(BOOST_ARCHIVE_DIRECTORY "" CACHE DIRECTORY "Use the specified local directory to search for boost archives instead of downloading them" )

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(DownloadBoost)

message(STATUS "Boost found: ${BOOST_VERSION} ${BOOST_SOURCE}")

include(StandaloneBuild)
include(PlatformDetect)
include(AddBoostLib)
include(AddBoostTest)

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

if(USE_ANDROID)
  # CMake 3.7.1 doesn't define the target triple for the ASM language,
  # resulting in all files compiled for the host architecture
  set(CMAKE_ASM_COMPILER_TARGET "${CMAKE_CXX_COMPILER_TARGET}")
endif()

set(BOOST_LIBS_REQUIRED
  # Header only libs
  header
)
set(BOOST_LIBS_OPTIONAL
  graph
  regex
  system
  serialization
)

foreach(lib ${BOOST_LIBS_REQUIRED})
  include("libs/${lib}.cmake")
endforeach()

foreach(lib ${BOOST_LIBS_OPTIONAL})
  # In case only a subset of modules is available (eg. after using bcp)
  if(EXISTS "${BOOST_SOURCE}/libs/${lib}")
    include("libs/${lib}.cmake")
  endif()
endforeach()

# TODO: Move those to option() calls in the right file
if(NOT BOOST_STANDALONE)
  # Compilation options required by all platforms
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS $<$<CONFIG:Release>:BOOST_DISABLE_ASSERT>)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_SYSTEM_NO_DEPRECATED)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_VERSION=4)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_USES_CHRONO)
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_THREAD_PROVIDES_EXECUTORS)
endif()

if(USE_ANDROID)
  # Android doesn't support thread local storage through compiler intrinsics
  set_property(TARGET Boost::boost APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS BOOST_ASIO_DISABLE_THREAD_KEYWORD_EXTENSION)
endif()
