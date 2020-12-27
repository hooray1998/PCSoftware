#include "mythread.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__
#define ONE_SIZE 8000

MyThread::MyThread(QTcpSocket *t)
{
    group = NULL;
    machineId = "-1";
    lastMsg = "empty";
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
        if(machineId=="-1")//首次与线程绑定
        {
            SendMsgOnce("01");
            machineId = header.left(6);
            heartTimer->start(10000);
            emit SendLog(this, header);
        }
        else if(machineId==header.left(6))//已经绑定了,但是仍然发送登录消息
        {
            SendMsgOnce("01");
            emit SendLog(QString("%1不需要再次登录。").arg(QString(machineId)));
        }
        else//收到的消息不是来自绑定的客户端(要么就是那边发错了)
            emit SendLog("Error in this.");

    }
    else if(mode=="66"){
        if(heartSendTime>3)
        this->group->sendLog(QString("设备%1【%2】重新连接")\
                .arg((this->group->getMachineA_id()==machineId)?'A':'B')\
                .arg(QString(machineId)));
        heartSendTime = 0;
    }
    else{
        //除了收到登录信息时可能没有绑定设备组
        //其他情况消息发来的时候一定绑定到了某个设备组
        char ID;
        if(!group){ emit SendLog(QString("设备%1还未绑定设备组，数据%2会丢失。").arg(QString(machineId)).arg(QString(data.right(data.size()-8)))); return; }
        else if(machineId==this->group->getMachineA_id()) ID = 'A';
        else if(machineId==this->group->getMachineB_id()) ID = 'B';
        else{ emit SendLog(QString("所属的设备组没有这个设备【%1】, 数据%2").arg(QString(machineId)).arg(QString(data.right(data.size()-8)))); return ; }


        switch (QString(mode).toInt()) {
            case 2:
                if(ID=='A'){
                    SendMsgOnce("02");//回应A
                    group->analyzeData_a(data.right(data.size()-8));
                }
                if(ID=='B'){
                    respondMsg("07");
                    group->analyzeData_r(data.right(data.size()-8));
                }
                break;
            case 3:
                if(ID=='B'){
                    respondMsg("06");
                    group->analyzeData_b(data.right(data.size()-8));
                }
                break;
            case 8:
            case 16:
                if(ID=='A'){
                    respondMsg(mode);
                    group->receive_buchong();
                }
                break;
            case 80:
                if(ID=='A'){
                    SendMsgOnce("80");//应答:补充完成
                    group->receive_buchong();
                }
                break;
            case 15:
            case 25:
                if(ID=='A'){
                    respondMsg(mode);
                    group->analyzeData_answer();
                }
                break;
            case 55:
                if(ID=='A'){
                    SendMsgOnce("55");
                    group->analyzeData_answer();
                }
                break;
            default:
                if(ID!='A'){
                    emit SendLog(QString("收到设备%1的无效消息%2").arg(ID).arg(QString(data)));
                }
                switch (QString(mode).toInt()) { //处理来自A的应答消息
                    //respondMsg  这些消息是回应哪些发过的消息,参数填发过的消息
                    case 35:
                    case 45:
                    case 14:
                    case 24:
                    case 65:
                    case 11:
                    case 4: respondMsg(mode);break;
                    //NOTE: 中止/结束
                    case 91: 
                    case 18: respondMsg(mode); this->group->end(); break;
                    default: emit SendLog(QString("收到设备A的无效消息%1").arg(QString(data)));
                }
        }
    }
}
void MyThread::WriteData(QByteArray array)
{
    if(die) return ;
    if(lastMsg != "empty"){
        emit SendLog(QString("上次的消息[%1]还未收到回应,不可继续发送新消息[%2]").arg(QString(lastMsg)).arg(QString(array)));
        return;
    }

    msgSendTime = 0;
    lastMsg = array;
    sendMsg();
    msgTimer->start(8765);

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
        }
        else
            this->group->sendLog(QString("设备B【%1】失去连接，消息重发%2次仍无回复。继续重播").arg(QString(machineId)).arg(msgSendTime));
    }
    msgSendTime++;
    tcpSocket->write(lastMsg);
}

void MyThread::sendHeart(){
    int value = heartSendTime;
    while(value&&value%3==0){
        value = value/3;
    }
    if(heartSendTime>=3&& (1==value)){
    //if(heartSendTime>=3){
    DBG<<"cur HEadr time:"<<heartSendTime;
        this->group->sendLog(QString("设备%1【%2】失去连接，心跳%3次仍无回复。继续心跳")\
                .arg((this->group->getMachineA_id()==machineId)?'A':'B')\
                .arg(QString(machineId))\
                .arg(heartSendTime ));
    }
    heartSendTime = heartSendTime+1;
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
    msgTimer->stop();
    heartTimer->stop();
    emit SendLog(this, QByteArray(machineId+"09"));
    tcpSocket->disconnectFromHost();
}

void MyThread::disconnectAll(){
    tcpSocket->disconnectFromHost();
}

void MyThread::end(){
    msgTimer->stop();
}

void MyThread::respondMsg(QByteArray msg){
    QByteArray mode = lastMsg.left(2);
    if(QString(mode)==msg){
        lastMsg = "empty";
        msgTimer->stop();
        msgSendTime = 0;
    }else{
        emit SendLog(QString("收到对%1冗余的应答, answer of need is %2").arg(QString(msg)).arg(QString(mode)));
    }
}

void MyThread::SendMsgOnce(QByteArray msg){
    tcpSocket->write(msg);
}
