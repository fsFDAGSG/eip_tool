# Copyright (C) 2022 dobot All rights reserved.
# Author: libobo@dobot.cc. Created in 202204

set(library_name "EipTool")
add_executable(${library_name}
    main.cpp
    ExplicitMessage.cpp
    ImplicitMessage.cpp
)

target_link_libraries(${library_name} PUBLIC
    EIPScannerS
)

target_include_directories(${library_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#set_target_properties((${library_name}
    #PROPERTIES
    #PUBLIC_HEADER $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
#)

install(TARGETS ${library_name}
)

