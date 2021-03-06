#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QTcpSocket>  //通信套接字
#include <QTimer>  //通信套接字
#include <QHostAddress>

#include <QSetIterator>
#include <QObject>
#include <QThread>
#include <QtDebug>
#include <QWidget>
#include <QByteArray>
#include <QUdpSocket>
#include <QFile>

#include "group.h"


class Group;

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QTcpSocket *t);

    void setName(QString n); //设备的id
    void setGroup(Group *g); //绑定的group

    Group* getGroup(); //返回group
    QString getIp(); //获得ip
    qint16 getPort(); //获得端口号
    QString getMachineID(); //获得id

    void SendMsgOnce(QByteArray msg);
    void WriteData(QByteArray); //发送消息
    void analyzeHeader(); //根据协议分析数据，获得id，判断在哪个设备组，然后处理

    void disconnectAll(); //断开连接

    void end();
    void respondMsg(QByteArray msg);

    bool die;

    QTimer *msgTimer;
    int msgSendTime;
    QByteArray lastMsg;

    QTimer *heartTimer;
    int heartSendTime;
public slots:

    void sendMsg(); //发送消息
    void sendHeart(); //发送消息
    void slot_readyRead();
    void slot_disconnect();
signals:
    void ReadData(int, QByteArray);
    void RecvEnd();
    void SendLog(QString);
    void SendLog(MyThread*, QByteArray);
protected:
    void run();
private:
    QTcpSocket *tcpSocket;
    int index;
    QByteArray machineId;
    Group *group;

	QByteArray data;
    QByteArray mode;
};

#endif // MYTHREAD_H
