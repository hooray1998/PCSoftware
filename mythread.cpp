#include "mythread.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__
#define ONE_SIZE 8000

MyThread::MyThread(QTcpSocket *t)
{
    group = NULL;
    machineId = "-1";
    tcpSocket = t;
    die = false;
}

void MyThread::run()
{
    connect(tcpSocket, &QTcpSocket::readyRead, [=](){
       data = tcpSocket->readAll();
            analyzeHeader();
    }
    );

    connect(tcpSocket, &QTcpSocket::disconnected, [=](){
        DBG<<"断开连接";
        die = true;
        emit SendLog(this, QByteArray(machineId+"09"));
        tcpSocket->disconnectFromHost();
    });

}

void MyThread::analyzeHeader(){
    QByteArray header = data.left(8);
    mode = header.right(2);
    DBG<<"length is"<<data.size()<<"head"<<header<<"mode"<<mode<<"   all is"<<data;



    if(mode=="01")
    {
        if(machineId=="-1")//init login
        {
            machineId = header.left(6);
            this->WriteData(QByteArray("ok"));
            emit SendLog(this, header);
        }
        else if(machineId==header.left(6))
        {
            emit SendLog(QString("%1不需要再次登录。").arg(QString(machineId)));
        }
        else//Normally bu hui chu xian zhe zhong qingkuang.
            emit SendLog("Error in this.");

    }
    else{
        //analyze if the group have this machine. => ofcourse have it.

        if(mode=="02")
        {
                if(!group)
                {
                    emit SendLog(QString("设备%1还未绑定设备组，数据会丢失。").arg(QString(machineId)));
                }
                else{

                    if(this->group->getMachineA_id()==machineId)
                    {
                        group->analyzeData_a(data.right(data.size()-8));
                    }
                    else if(this->group->getMachineB_id()==machineId)
                    {
                        group->analyzeData_r(data.right(data.size()-8));
                    }
                    else
                        emit SendLog(QString("所属的设备组没有这个设备【%1】。").arg(QString(machineId)));
                }
        }
        else if(mode=="03")
        {
                if(!group)
                {
                    emit SendLog(QString("%1 还未绑定设备，数据会丢失。").arg(QString(machineId)));
                }
                else{
                    if(this->group->getMachineB_id()==machineId)
                    {
                        group->analyzeData_b(data.right(data.size()-8));
                    }
                    else
                        emit SendLog(QString("所属的设备组没有这个设备B【%1】。").arg(QString(machineId)));
                }
        }
        else{
            emit SendLog(QString("%1 receive other mode%2").arg(QString(machineId)).arg(QString(mode)));
        }

    }
}
void MyThread::WriteData(QByteArray array)
{
    if(die) return ;
    tcpSocket->write(array);
}

QString MyThread::getIp()
{
    return tcpSocket->peerAddress().toString();
}

qint16 MyThread::getPort()
{
    return tcpSocket->peerPort();
}


QString MyThread::getMachineID(){
    return machineId;
}

void MyThread::setGroup(Group *g){
    group = g;
}

Group* MyThread::getGroup(){
    return group;
}
