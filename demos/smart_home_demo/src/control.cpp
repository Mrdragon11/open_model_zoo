// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "control.hpp"
//#include "serial_port.hpp"

const char* portName = "\\\\.\\COM2";
const char ledON[] = "ON\n";
const char ledOFF[] = "OFF\n";

int Controller::Initialize()
{
    SetState(IDLE);

    if (!arduino) {
        arduino = new SerialPort(portName);
    }

    std::cout << "Start to connect serial" << endl;
    while (!arduino->isConnected()) {
        std::cout << "Connecting serial again.." << endl;
        delete arduino;
        Sleep(100);

        arduino = new SerialPort(portName);
    }

    //Checking if arduino is connected or not
    if (arduino->isConnected()) {
        std::cout << std::endl << "Connection established at port " << portName << std::endl;
    }

    return SUCCESS;
}

int Controller::Start()
{
    SetState(SCANNING);
    return SUCCESS;
}

int Controller::Execute()
{
    int ret = SUCCESS;
    switch (m_state) {
    case SCANNING:
        if (1) {
            m_state = ALARM;
            Alarm();
        }
        break;

    case ALARM:
        //Alarm();
        break;

    case ASSISTING:

        break;

    case IDLE:
    default:
        break;
    }


    return ret;
}

int Controller::Alarm()
{
    int ret;
    ret = arduino->writeSerialPort(ledON, strlen(ledON));
    cout << "write serial ret " << ret << endl;
    return ret;
}

Controller::~Controller()
{
    if (arduino) {
        delete arduino;
        arduino = nullptr;
    }
}


