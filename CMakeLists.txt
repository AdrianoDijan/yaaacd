cmake_minimum_required(VERSION 3.9)

project(yaaacd CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake" TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES cgal/5.3 gmp/6.2.1 GENERATORS cmake_find_package)
conan_cmake_autodetect(settings)
conan_cmake_install(
  PATH_OR_REFERENCE
  .
  BUILD
  missing
  REMOTE
  conancenter
  SETTINGS
  ${settings})

find_package(CGAL CONFIG REQUIRED)
find_package(gmp REQUIRED)
find_package(mpfr REQUIRED)

add_library(yaaacd src/bounds.cpp src/octree.cpp src/vertex.cpp src/helpers.cpp src/hashmap.cpp
                   include/common.hpp include/octree.hpp include/hashmap.hpp include/objfile.hpp)

set_target_properties(yaaacd PROPERTIES PUBLIC_HEADER
                                        "include/octree.hpp;include/common.hpp;include/hashmap.hpp;include/objfile.hpp")
target_include_directories(yaaacd
  PRIVATE $<TARGET_PROPERTY:CGAL,INTERFACE_INCLUDE_DIRECTORIES>)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.0.1)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

file(GLOB TEST_FILES test/test_*.cpp)

add_executable(tests ${TEST_FILES})

target_include_directories(tests PUBLIC include/)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain yaaacd CGAL)
target_link_libraries(${PROJECT_NAME} ${gmp_LIBS} ${mpfr_LIBS})

enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(tests)

install(
  TARGETS yaaacd
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/yaaacd
)

