#include "mythread.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__
#define ONE_SIZE 8000

MyThread::MyThread(int i, QTcpSocket *t)
{
    machineId = "-1";
    index = i;
    tcpSocket = t;

}

void MyThread::run()
{

    //-----------------------------------------------------------------------------------------------
    //接收到了内容之后，直接在显示区域显示消息
    /**/ connect(tcpSocket, &QTcpSocket::readyRead,
    /**/        [=]()
    /**/        {
    /**/           //从通信套接字中间取出内容
    /**/           data = tcpSocket->readAll();
    /**/        }
    /**/        );
    //-----------------------------------------------------------------------------------------------

}

void MyThread::analyzeHeader(){
    QByteArray header = data.left(8);
    mode = header.right(2);
    if(header.left(6)!=machineId){
        if(!machineId.size()&&mode.data()=="01")
        {
            machineId = header.left(6);
        }
        else
            DBG<<"It is a wrong machineId:"<<header.left(6)<<"  oldId is "<<machineId;
    }
    else{
        DBG<<"mode is "<<mode;
        if(mode == "02")
        {
            if(!group)
            {
                DBG<<"Haven't bound group, data will be loss.";
            }
            else{
                group->analyzeDataA(data.right(data.size()-8));
            }
        }
    }
}
void MyThread::WriteData(QByteArray array)
{
    if(die) return ;
    tcpSocket->write(array);
    //DBG<<" "<<ip<<"port:"<<port<<"   context:"<<array;
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

