#include "mythread.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__
#define ONE_SIZE 8000

MyThread::MyThread(QTcpSocket *t)
{
    group = NULL;
    machineId = "-1";
    tcpSocket = t;
    die = false;
    msgTimer = new QTimer(this);
    msgSendTime = 0;
    heartSendTime = 0;
    connect(msgTimer, SIGNAL(timeout()), this, SLOT(sendMsg()));

    heartTimer = new QTimer(this);
}

void MyThread::run()
{
    connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(slot_readyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnect()));
    connect(heartTimer, SIGNAL(timeout()), this, SLOT(sendHeart()));
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
            this->tcpSocket->write(QByteArray("ok"));
            heartTimer->start(10000);
            emit SendLog(this, header);
        }
        else if(machineId==header.left(6))
        {
            emit SendLog(QString("%1不需要再次登录。").arg(QString(machineId)));
        }
        else//Normally bu hui chu xian zhe zhong qingkuang.
            emit SendLog("Error in this.");

    }
    else if(mode=="66"){
        heartSendTime = 0;
    }
    else{
        //analyze if the group have this machine. => ofcourse have it.
        msgTimer->stop();
        msgSendTime = 0;

        if(mode=="02")
        {
            if(!group){
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
        else if(mode=="55")
        {
            if(!group)
            {
                emit SendLog(QString("%1 还未绑定设备，数据会丢失。").arg(QString(machineId)));
            }
            else{
                if(this->group->getMachineA_id()==machineId)
                {
                    group->analyzeData_answer();
                }
                else
                    emit SendLog(QString("所属的设备组没有这个设备A【%1】。").arg(QString(machineId)));
            }
        }
        else if(mode=="80")
        {
            if(!group)
            {
                emit SendLog(QString("%1 还未绑定设备，数据会丢失。").arg(QString(machineId)));
            }
            else{
                if(this->group->getMachineA_id()==machineId)
                {
                    group->receive_buchong();
                }
                else
                    emit SendLog(QString("所属的设备组没有这个设备A【%1】。").arg(QString(machineId)));
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
    //if(heartTimer->isActive()){
        //heartTimer->stop();
    //}

    msgSendTime = 0;
    lastMsg = array;
    sendMsg();
    msgTimer->start(10000);

}
void MyThread::sendMsg(){
    int value = msgSendTime;
    while(value&&value%3==0){
        value = value/3;
    }
    if(msgSendTime>=3&& (1==value)){
        if(this->group->getMachineA_id()==machineId)
        {
            this->group->sendLog(QString("设备A【%1】失去连接，消息重发%2次仍无回复。继续重播").arg(QString(machineId)).arg(msgSendTime));
            //emit SendLog(QString("设备A【%1】失去连接，消息重发%2次仍无回复。继续重播").arg(QString(machineId)).arg(msgSendTime));
        }
        else
            this->group->sendLog(QString("设备B【%1】失去连接，消息重发%2次仍无回复。继续重播").arg(QString(machineId)).arg(msgSendTime));
            //emit SendLog(QString("设备B【%1】失去连接，消息重发%2次仍无回复。继续重播").arg(QString(machineId)).arg(msgSendTime));
    }
    msgSendTime++;
    tcpSocket->write(lastMsg);
}

void MyThread::sendHeart(){
    int value = heartSendTime;
    while(value&&value%3==0){
        value = value/3;
    }
    if(heartSendTime>=3&& (1==!value)){
        if(this->group->getMachineA_id()==machineId)
        {
            this->group->sendLog(QString("设备A【%1】失去连接，心跳%2次仍无回复。继续心跳").arg(QString(machineId)).arg(heartSendTime));
        }
        else
            this->group->sendLog(QString("设备B【%1】失去连接，心跳%2次仍无回复。继续心跳").arg(QString(machineId)).arg(heartSendTime));
    }
    heartSendTime++;
    tcpSocket->write(QByteArray("66"));
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

void MyThread::slot_readyRead(){
    data = tcpSocket->readAll();
    analyzeHeader();
}

void MyThread::slot_disconnect(){
    die = true;
    if(group){
        group->logout(this);
    }
    emit SendLog(this, QByteArray(machineId+"09"));
    tcpSocket->disconnectFromHost();
}

void MyThread::disconnectAll(){
    tcpSocket->disconnectFromHost();
}

void MyThread::end(){
    msgTimer->stop();
    //heartTimer->start(10000);
}
