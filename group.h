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
    GroupInfo groupInfo;

    void analyzeDataA(QByteArray data);
    void analyzeDataB(QByteArray data);


private:
    MyThread *machineA;
    MyThread *machineB;
    AllData allData;
};

#endif // GROUP_H
