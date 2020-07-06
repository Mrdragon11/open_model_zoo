// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <samples/ocv_common.hpp>

#include "serial_port.hpp"

using namespace std;


enum ProductState {
    IDLE,       // not working
    SCANNING,   // normal working and monitoring
    ALARM,      // detected kid's dangerous actions
    ASSISTING,  // detected adult helping kid
    MAX_PRODUCT_STATE,
};



class Controller
{
    enum {
        SUCCESS = 0,
        FAILURE = 1
    };

    struct Person
    {
        cv::Rect face;
        //pose position
        //action
    };

public:
    //Controller() = default;

    virtual ~Controller();

    int Initialize();

    int Start();

    int Execute();

    void SetState(int state) { m_state = state; }

    int GetState() { return m_state; }

    //int Update();


private:
    int Alarm();

    int m_state = MAX_PRODUCT_STATE;

    SerialPort *arduino = nullptr;

    unordered_map<string, Person> m_persons;

};
