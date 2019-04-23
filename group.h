#ifndef GROUP_H
#define GROUP_H

#include "groupinfo.h"
#include "mythread.h"
#include "alldata.h"

class Group
{
public:
    Group();

private:
    GroupInfo groupInfo;
    MyThread *machineA;
    MyThread *machineB;
    AllData allData;
};

#endif // GROUP_H
