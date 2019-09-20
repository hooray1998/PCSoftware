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

    void login(MyThread *machine); //绑定 设备a/b的线程
    void logout(MyThread *machine); //解除绑定 设备a/b的线程
    int getOnlineStatus(); //获得在线的状态

    void setMachineA(MyThread* a);
    void setMachineB(MyThread* b);
    QString getMachineA_id();
    QString getMachineB_id();

    void tie_byID(QString gname,QString a, QString b); //通过id绑定设备组
    void tie(QString gname, MyThread* a, MyThread* b); //通过线程绑定设备组
    void untie(); //解绑该设备组

    void setWorker(QString w); //设置工作人员
    bool stop(); //中止当前调试进程

    //data processing
    void request_b(); //寻求初值
    void request_a(); //寻求实际值
    void request_r(); //寻求末值
    void analyzeData_a(QByteArray data); //存储处理实际值
    void analyzeData_b(QByteArray data); //存储处理初值
    void analyzeData_r(QByteArray data); //存储处理末值

    void returnFinalResult(int mode); //vs调试返回最终值给设备
	void returnThreeResult(int mode); //精度调试返回调整后的三个值
    void analyzeData_BuChong();

    void tellToJD(); //通知设备A转化为精度调试


    void saveData(FILE *fp);
    void readData(FILE *fp);

    bool meishuile;
private:
    MyThread *machineA;
    MyThread *machineB;

};

#endif // GROUP_H
