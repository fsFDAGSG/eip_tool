// Copyright (C) 2022 dobot All rights reserved.
// Author: libobo@dobot.cc. Created in 202204

#include <exception>
#include <iostream>

int ImplicitMessage();
int ExplicitMessage();

int main(int args, char **argv) {
    try {
        ImplicitMessage();
        ExplicitMessage();
    } catch (std::exception &e) {
        printf("Run main error %s \n", e.what());
    }
    return 0;
}
