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
    DBG<<"start run";

    //-----------------------------------------------------------------------------------------------
    //接收到了内容之后，直接在显示区域显示消息
    /**/ connect(tcpSocket, &QTcpSocket::readyRead,
    /**/        [=]()
    /**/        {
    /**/           data = tcpSocket->readAll();
                    analyzeHeader();
    /**/        }
    /**/        );
    connect(tcpSocket, &QTcpSocket::disconnected, [=](){
        DBG<<"dis connect";
        die = true;
        emit SendLog(group, QByteArray(machineId+"09"));
        tcpSocket->disconnectFromHost();
    });

    //-----------------------------------------------------------------------------------------------

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
            emit SendLog(group, header);
        }
        else if(machineId==header.left(6))
        {
            emit SendLog(QString("%1don't need to login in again.").arg(QString(machineId)));
        }
        else//Normally bu hui chu xian zhe zhong qingkuang.
            emit SendLog("Error in this.");

    }
    else if(mode=="02")
    {
            if(!group)
            {
                emit SendLog(QString("%1Haven't bound group, data will be loss.").arg(QString(machineId)));
            }
            else{
                emit SendLog(group, header);
                group->analyzeDataA(data.right(data.size()-8));
            }
    }
    else if(mode=="03")
    {
            if(!group)
            {
                emit SendLog(QString("%1Haven't bound group, data will be loss.").arg(QString(machineId)));
            }
            else{
                emit SendLog(group, header);
                group->analyzeDataA(data.right(data.size()-8));
            }
    }
    else if(mode=="04")
    {
    }
    else if(mode=="05")
    {
    }
    else{
        emit SendLog(QString("%1 receive other mode%2").arg(QString(machineId)).arg(QString(mode)));
    }
}
void MyThread::WriteData(QByteArray array)
{
    if(die) return ;
    tcpSocket->write(array);
}


void MyThread::setName(QString n)
{
    name = n;
}

QString MyThread::getName()
{
    return name;
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
