#include "mythread.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__
#define ONE_SIZE 8000

MyThread::MyThread(QTcpSocket *t)
{
    group = NULL;
    machineId = "-1";
    tcpSocket = t;
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
        emit SendLog(QString("%1 disconnect").arg(QString(machineId)));
        tcpSocket->disconnectFromHost();
    });

    //-----------------------------------------------------------------------------------------------

}

void MyThread::analyzeHeader(){
    QByteArray header = data.left(8);
    mode = header.right(2);
    if(mode=="01")
    {
        if(machineId=="-1")//init login
        {
            machineId = header.left(6);
            emit SendLog(QString("%1 login in").arg(QString(machineId)));
        }
        else if(machineId==header.left(6))
        {
            emit SendLog(QString("don't need to login in again."));
        }
        else//Normally bu hui chu xian zhe zhong qingkuang.
            emit SendLog("Error in this.");

    }
    else if(mode=="02")
    {
            if(!group)
            {
                emit SendLog("Haven't bound group, data will be loss.");
            }
            else{
                emit SendLog("Its group is ready.");
                group->analyzeDataA(data.right(data.size()-8));
            }
    }
    else if(mode=="03")
    {
    }
    else if(mode=="04")
    {
    }
    else if(mode=="05")
    {
    }
    else{
        emit SendLog("receive other mode");
        DBG<<"length is"<<data.size()<<"head"<<header<<"mode"<<mode<<"   all is"<<data;
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

