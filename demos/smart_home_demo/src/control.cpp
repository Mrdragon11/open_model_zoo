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

    bool isDangerous = IsDangerous("andrew-0");

    switch (m_state) {
    case SCANNING:
        if (isDangerous) {
            //printf(" [DEBUG] andrew-0 dangerous\n");
            m_state = ALARM;
            Alarm(true);
        }
        break;

    case ALARM:
        //Alarm();
        if (!isDangerous) {
            m_state = SCANNING;
            Alarm(false);
        }
        
        break;

    case ASSISTING:

        break;

    case IDLE:
    default:
        break;
    }


    return ret;
}

int Controller::UpdatePoses(std::vector<HumanPose>& poses)
{
    m_poses = poses;
    return 0;
}

int Controller::UpdateOneFace(cv::Rect rect, int personInd, const std::string& personName, int actionInd)
{
    //match face rect and keypoint face
    for (auto pose : m_poses) {
        // check if keypoints are inside face rect
        if (pose.keypoints[FACE].inside(rect) &&
            pose.keypoints[R_EYE].inside(rect) &&
            pose.keypoints[L_EYE].inside(rect) ) {
            //match, update or create person name-info
            m_persons[personName].index = personInd;
            m_persons[personName].pose = pose;
            m_persons[personName].face = rect;
            m_persons[personName].actionId = actionInd;

            ////debug print
            //printf(" Updated personName %s, personInd %d, actionInd %d, face (%f,%f) \n",
            //    personName, personInd, actionInd,
            //    pose.keypoints[FACE].x, pose.keypoints[FACE].y);
        }
    }


    return 0;
}

bool Controller::IsDangerous(const char *name)
{
    string strname(name);
    for (auto& person : m_persons) {
        if (person.first != strname) {
            continue;
        }
        //found, judge if person is in danger
        PersonInfo &info = person.second;
        if (info.pose.keypoints[FACE].y < 500) {
            return true;
        }
        if (info.actionId == raising_hand) {
            return true;
        }
    }
    return false;
}

int Controller::Alarm(bool on)
{
    int ret;
    if (on) {
        ret = arduino->writeSerialPort(ledON, strlen(ledON));
    }
    else {
        ret = arduino->writeSerialPort(ledOFF, strlen(ledOFF));
    }
    cout << "alarm " << (on ? "ON" : "OFF") <<" ,write serial return " << ret << endl;
    return ret;
}



Controller::~Controller()
{
    if (arduino) {
        delete arduino;
        arduino = nullptr;
    }
}


