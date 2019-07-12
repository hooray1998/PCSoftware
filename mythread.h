#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QTcpSocket>  //通信套接字
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

    void setName(QString n);
    void setGroup(Group *g);

    Group* getGroup();
    QString getIp();
    qint16 getPort();
    QString getMachineID();

    void WriteData(QByteArray);
    void analyzeHeader();


    bool die;
public slots:

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
