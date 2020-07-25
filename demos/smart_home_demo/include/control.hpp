// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <samples/ocv_common.hpp>

#include "serial_port.hpp"
#include "human_pose.hpp"

using namespace std;
using namespace human_pose_estimation;


enum ControlState {
    IDLE,       // not working
    SCANNING,   // normal working and monitoring
    ALARM,      // detected kid's dangerous actions
    ASSISTING,  // detected adult helping kid
    MAX_PRODUCT_STATE,
};


enum HumanKeypoints {
    FACE = 0,
    CENTER = 1,
    L_SHOULDER = 2,
    L_ARM = 3,
    L_HAND = 4,
    R_SHOULDER = 5,
    R_ARM = 6,
    R_HAND = 7,
    L_CROTCH = 8,
    L_KNEE = 9,
    L_FOOT = 10,
    R_CROTCH = 11,
    R_KNEE = 12,
    R_FOOT = 13,
    L_EYE = 14,
    L_EAR = 16,
    R_EYE = 15,
    R_EAR = 17
};


enum Actions {
    sitting = 0,
    writing,
    raising_hand,
    standing,
    turned_around,
    lie_on_the_desk
};


struct PersonInfo
{
    int index;
    cv::Rect face;
    int actionId;
    HumanPose pose;
};

enum {
    SUCCESS = 0,
    FAILURE = 1
};

class Controller
{
public:
    Controller() = default;

    virtual ~Controller();

    int Initialize();

    int Start();

    int Execute();

    void SetState(int state) { m_state = state; }

    int GetState() { return m_state; }
    
    int UpdatePoses(std::vector<HumanPose> &poses);

    int UpdateOneFace(cv::Rect rect, int personInd, const std::string& personName, int actionInd);

    bool IsDangerous(const char* name);


private:
    int Alarm(bool on);

    int m_state = MAX_PRODUCT_STATE;

    SerialPort *arduino = nullptr;

    unordered_map<string, PersonInfo> m_persons; //detected person name - person info (pose,face,action)

    std::vector<HumanPose> m_poses; //poses of all persons

};
