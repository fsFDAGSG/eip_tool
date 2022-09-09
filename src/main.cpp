// Copyright (C) 2022 dobot All rights reserved.
// Author: libobo@dobot.cc. Created in 202204

#include <exception>
#include <iostream>
#include <system_error>

struct ImplicitParam {
    uint8_t config_id;
    uint8_t output_id;
    uint8_t input_id;
    int output_size;
    int input_size;
};

int ImplicitMessage(const std::string &, const ImplicitParam &);
int ExplicitMessage(const std::string &);

// exmample:
//   EipTool 127.0.0.1
//   EipTool 127.0.0.1 [config_id] [output_id] [input_id] [output_size] [input_size]
//   EipTool 127.0.0.1 151 150 100 224 472
//   EipTool 127.0.0.1 151 150 100 32 32
int main(int args, char **argv) {
    try {
        if (args > 2) {
            ImplicitMessage(argv[1],
                {(uint8_t)atoi(argv[2]), (uint8_t)atoi(argv[3]),
                 (uint8_t)atoi(argv[4]), atoi(argv[5]), atoi(argv[6])});
        } else {
            ExplicitMessage(argv[1]);
        }
    } catch (std::system_error &e) {
        printf("Run main error %s : %s \n", e.code().category().name(), e.what());
    } catch (std::exception &e) {
        printf("Run main error %s \n", e.what());
    }
    return 0;
}
