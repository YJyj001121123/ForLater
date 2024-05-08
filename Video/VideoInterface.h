#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <unordered_set>
#include <queue>

#ifndef FORLATER_VIDEOINTERFACE_H
#define FORLATER_VIDEOINTERFACE_H

#endif //FORLATER_VIDEOINTERFACE_H
/***
 * 视频管理
 */
class IVideoDeviceManager{
    IVideoDeviceManager(){};
    virtual int setDevice() = 0;
    virtual int getDevice() = 0;
};