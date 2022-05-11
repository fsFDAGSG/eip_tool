// Copyright (C) 2022 dobot inc All rights reserved.
// Author: libobo@dobot.cc. Created in 202204

#include "gtest/gtest.h"
#include "eip_robot/Eip.h"

TEST(eipfunctions, EIP) {
    eip_robot::Eip eip;
    eip.Example();
    ASSERT_TRUE(true);

}
