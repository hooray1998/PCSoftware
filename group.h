#ifndef GROUP_H
#define GROUP_H

#include "groupinfo.h"
#include "mythread.h"
#include "alldata.h"

class MyThread;
class Group
{
public:
    Group();

    void analyzeDataA(QByteArray data);


private:
    GroupInfo groupInfo;
    MyThread *machineA;
    MyThread *machineB;
    AllData allData;
};

#endif // GROUP_H
