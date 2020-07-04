// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once



#include <memory>

#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <samples/ocv_common.hpp>


using namespace std;

class Controller
{
    struct Person
    {
        cv::Rect face;
        //pose position
        //action
    };
public:
    int Control();

    //int Update();


private:

    int m_state;

    unordered_map<string, Person> m_persons;

};
