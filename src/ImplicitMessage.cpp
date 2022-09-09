// Copyright (C) 2022 dobot All rights reserved.
// Author: libobo@dobot.cc. Created in 202204

#include <sstream>
#include "cip/connectionManager/NetworkConnectionParams.h"
#include "SessionInfo.h"
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

struct ImplicitParam {
    uint8_t config_id;
    uint8_t output_id;
    uint8_t input_id;
    int output_size;
    int input_size;
};

int ImplicitMessage(const std::string &host, const ImplicitParam &param) {
  Logger::setLogLevel(LogLevel::DEBUG);
#if WIN32
  WSADATA wsaData;
  int winsockStart = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (winsockStart != 0) {
    Logger(LogLevel::ERROR) << "Failed to start WinSock - error code: " << winsockStart;
    return EXIT_FAILURE;
  }
#endif

  auto si = std::make_shared<SessionInfo>(host, 0xAF12);

  // Implicit messaging
  ConnectionManager connectionManager;

  ConnectionParameters parameters;
  parameters.connectionPath =
      {0x20, 0x04, 0x24, param.config_id, 0x2C, param.output_id, 0x2C, param.input_id}; // config Assm151, output Assm150, intput Assm100
  parameters.o2tRealTimeFormat = true;
  parameters.originatorVendorId = 342;
  parameters.originatorSerialNumber = 32423;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.t2oNetworkConnectionParams |= param.input_size; //size of Assm100 =32
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.o2tNetworkConnectionParams |= param.output_size; //size of Assm150 = 32

  parameters.originatorSerialNumber = 0x12345;
  parameters.o2tRPI = 1000000;
  parameters.t2oRPI = 1000000;
  parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;
  parameters.connectionTimeoutMultiplier = 3;

  auto io = connectionManager.forwardOpen(si, parameters);
  if (auto ptr = io.lock()) {
    ptr->setDataToSend(std::vector<uint8_t>(param.output_size, 1));

    ptr->setReceiveDataListener([](auto realTimeHeader, auto sequence, auto data) {
      std::ostringstream ss;
      ss << "secNum=" << sequence << " data=";
      for (auto &byte : data) {
        ss << "[" << std::hex << (int) byte << "]";
      }

      Logger(LogLevel::INFO) << "Received: " << ss.str();
    });

    ptr->setCloseListener([]() {
      Logger(LogLevel::INFO) << "Closed";
    });
    std::vector<uint8_t> send_data(param.output_size);
    for(; connectionManager.hasOpenConnections();) {
        char buff[64];
        printf("Enter implicit cmd:");
        scanf("%63s", buff);
        if (!strcmp(buff, "q")) {
            break;
        } else if (!strcmp(buff, "get") || !strcmp(buff, "g")) {
        } else if (!strcmp(buff, "set") || !strcmp(buff, "s")) {
            printf("Enter addr:");
            scanf("%63s", buff);
            int addr = atoi(buff);
            if (addr >= 0 && addr < send_data.size()) {
                printf("Enter data:");
                scanf("%63s", buff);
                send_data[addr] = buff[0];
                ptr->setDataToSend(send_data);
                printf("Data size is %u, Data is:", send_data.size());
                for (auto &i: send_data) {
                    printf(" 0x%x", i);
                }
                printf("\n");
            }
        } else {
            printf("\n");
        }
        connectionManager.handleConnections(std::chrono::milliseconds(100));
    }
  }

  connectionManager.forwardClose(si, io);

#if WIN32
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
