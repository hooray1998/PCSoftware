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
    AllData allData;

    void setMachineA(MyThread* a);
    void setMachineB(MyThread* b);
    QString getMachineA_id();
    QString getMachineB_id();
    void tie(QString gname, MyThread* a, MyThread* b);
    void untie();

    //data processing
    void analyzeDataA(QByteArray data);
    void analyzeDataB(QByteArray data);

private:
    MyThread *machineA;
    MyThread *machineB;
};

#endif // GROUP_H
