#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include "groupinfo.h"
#include "mythread.h"
#include "alldata.h"
#include <string.h>

class MyThread;
class Group : public QObject
{
    Q_OBJECT

signals:
    void SendLog(QString, QString);//group + msg
public:
    Group();
    GroupInfo groupInfo;
    AllData allData;

    void setMachineA(MyThread* a);
    void setMachineB(MyThread* b);
    QString getMachineA_id();
    QString getMachineB_id();

    void tie_byID(QString gname,QString a, QString b);
    void tie(QString gname, MyThread* a, MyThread* b);
    void untie();

    void setWorker(QString w);
    void stop();

    //data processing
    void request_b();
    void request_a();
    void request_r();
    void analyzeData_a(QByteArray data);
    void analyzeData_b(QByteArray data);
    void analyzeData_r(QByteArray data);
    void returnFinalResult(double final);




private:
    MyThread *machineA;
    MyThread *machineB;
};

#endif // GROUP_H
