// Copyright (C) 2022 dobot All rights reserved.
// Author: libobo@dobot.cc. Created in 202204

#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <cstdlib>
#include <sstream>
#include <cip/connectionManager/NetworkConnectionParams.h>
#include "SessionInfo.h"
#include "MessageRouter.h"
#include "ConnectionManager.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::ConnectionManager;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

int ExplicitMessage(const std::string &host) {
    Logger::setLogLevel(LogLevel::DEBUG);
#if WIN32
  WSADATA wsaData;
  int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (winsockStart != 0) {
    Logger(LogLevel::ERROR) << "Failed to start WinSock - error code: " << winsockStart;
    return EXIT_FAILURE;
  }
#endif

    auto si = std::make_shared<SessionInfo>(host, 0xAF12, std::chrono::seconds(10));
    auto messageRouter = std::make_shared<MessageRouter>();

    // Read attribute
    auto response = messageRouter->sendRequest(si, ServiceCodes::GET_ATTRIBUTE_SINGLE,
            EPath(0x01, 1, 1),
            {});

    if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
        Buffer buffer(response.getData());
        CipUint vendorId;
        buffer >> vendorId;

        Logger(LogLevel::INFO) << "Vendor ID is " << vendorId;
    } else {
        Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << response.getGeneralStatusCode();
    }

    //Write attribute
    // See OpenEr EDS 160 line
    Buffer assembly151;
    assembly151 << CipUsint(1)
        << CipUsint(2)
        << CipUsint(3)
        << CipUsint(4)
        << CipUsint(5)
        << CipUsint(6)
        << CipUsint(7)
        << CipUsint(8)
        << CipUsint(9)
        << CipUsint(10);


    response = messageRouter->sendRequest(si, ServiceCodes::SET_ATTRIBUTE_SINGLE,
            EPath(0x04, 151, 3),
            assembly151.data());

    if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
        Logger(LogLevel::INFO) << "Writing is successful";
    } else {
        Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << response.getGeneralStatusCode();
    }
    for(;;) {
        char buff[64];
        printf("Enter explicit cmd:");
        scanf("%63s", buff);

        auto get_epath = []() {
            printf("Enter epath[classId objectId attributeId]:");
            uint32_t e[3];
            scanf("%u %u %u", &e[0], &e[1], &e[2]);
            return EPath(e[0], e[1], e[2]);
        };
        if (!strcmp(buff, "q")) {
            break;
        } else if (!strcmp(buff, "get") || !strcmp(buff, "g")) {
            auto epath = get_epath();
            response = messageRouter->sendRequest(si,
                    ServiceCodes::GET_ATTRIBUTE_SINGLE,
                    epath, {});

            if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
                auto vec = Buffer(response.getData()).data();
                Logger log(LogLevel::INFO);
                log << "Data size: " << vec.size() << " Data is:";
                log << std::hex;
                for (auto &i: vec) {
                    log << " 0x" << (size_t)i;
                }

            } else {
                Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << (size_t)response.getGeneralStatusCode();
            }

        } else if (!strcmp(buff, "set") || !strcmp(buff, "s")) {
            auto epath = get_epath();
            printf("Enter data:");
            scanf("%63s", buff);

            std::vector<uint8_t> assembly;
            assembly.assign(buff, buff + strlen(buff));
            response = messageRouter->sendRequest(si,
                    ServiceCodes::SET_ATTRIBUTE_SINGLE,
                    epath, assembly);

            if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
                Logger(LogLevel::INFO) << "Writing is successful";
            } else {
                Logger(LogLevel::ERROR) << "We got error=0x" << std::hex << (size_t)response.getGeneralStatusCode();
            }
        } else {
            printf("\n");
        }
    }

#if WIN32
  WSACleanup();
#endif

    return EXIT_SUCCESS;
}
