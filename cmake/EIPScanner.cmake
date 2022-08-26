# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202204

include(FetchContent)
FetchContent_Declare(
    EIPScanner
    # GIT_REPOSITORY https://github.com/xiaohaidao/EIPScanner.git
    GIT_REPOSITORY http://192.168.0.11:20080/libobo/EIPScanner.git
    # GIT_TAG dev
    GIT_TAG master
)
FetchContent_MakeAvailable(EIPScanner)
include_directories(${eipscanner_SOURCE_DIR}/src/)

