#
#
#  MIT License
#
#  Copyright (c) 2024-2025 Yafiyogi
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in all
#  copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#
#

# PKG_CONFIG_PATH=${HOME}/code/tp/lib/pkgconfig:${HOME}/code/tp/share/pkgconfig:${PKG_CONFIG_PATH} cmake --fresh -DCMAKE_INSTALL_PREFIX=${HOME}/code/cpp/build -DYY_THIRD_PARTY_LIBRARY=${HOME}/code/tp -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DCMAKE_BUILD_TYPE=Release .
# cmake --fresh -DCMAKE_INSTALL_PREFIX=${HOME}/code/cpp/build -DYY_THIRD_PARTY_LIBRARY=${HOME}/code/tp -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DCMAKE_BUILD_TYPE=Release .

cmake_minimum_required(VERSION 3.25)

project(yy_web LANGUAGES CXX VERSION 0.0.1)

set(YY_ROOT "${PROJECT_SOURCE_DIR}/..")
set(YY_CMAKE "${YY_ROOT}/yy_cmake")

include(${YY_CMAKE}/cmake_common.txt)
include(${YY_CMAKE}/cmake_clang_tidy.txt)

add_library(yy_web STATIC)

target_compile_options(yy_web
  PRIVATE
    "-DSPDLOG_COMPILED_LIB"
    "-DSPDLOG_FMT_EXTERNAL")

target_include_directories(yy_web
  PRIVATE
    "${CMAKE_INSTALL_PREFIX}/include" )

target_include_directories(yy_web
  SYSTEM PRIVATE
    "${YY_THIRD_PARTY_LIBRARY}/include" )

target_sources(yy_web
  PRIVATE
    yy_web_server.cpp
    yy_web_handler.cpp
  PUBLIC FILE_SET HEADERS
    FILES
      yy_web_server.h
      yy_web_handler.h)

install(TARGETS yy_web
  EXPORT yy_webTargets
  FILE_SET HEADERS DESTINATION include/yy_web)

install(EXPORT yy_webTargets
  NAMESPACE yy_web::
  DESTINATION lib/cmake/yy_web)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("yy_webConfigVersion.cmake"
  VERSION ${yy_web_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(FILES "yy_webConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/yy_webConfigVersion.cmake"
    DESTINATION lib/cmake/yy_web)

#add_subdirectory(unit_tests)

add_yy_tidy_all(yy_web)
