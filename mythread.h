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
    Group *group;
    MyThread(QTcpSocket *t);
    void WriteData(QByteArray);
    QString getIp();
    qint16 getPort();
    QString getMachineID();
    void setName(QString n);
    void analyzeHeader();
    QString getName();
    bool die;
public slots:

signals:
    void ReadData(int, QByteArray);
    void RecvEnd();
    void SendLog(Group*, QByteArray);
    void SendLog(QString);
protected:
    void run();
private:
    QTcpSocket *tcpSocket;
    int index;
    QByteArray machineId;
    QString name;

	QByteArray data;
    QByteArray mode;
};

#endif // MYTHREAD_H
