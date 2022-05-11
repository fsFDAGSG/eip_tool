# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202204

cmake_minimum_required(VERSION 3.14)

project("EipTool")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(EIPScanner)

add_subdirectory(src)

option(BUILD_TESTING "Build test" OFF)
include(CTest)
if(BUILD_TESTING)
    include(googletest)

    ADD_SUBDIRECTORY(tests)
endif()

