#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QTcpServer>  //监听套接字
#include <QNetworkInterface>
#include <QTcpSocket>  //通信套接字
#include <QKeyEvent>
#include <QDebug>
#include <QSet>
#include <QFile>
#include <QFileInfoList>
#include <QDir>

#include "group.h"
#include "mythread.h"
#include "expression.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void SetIP_Port();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



	//数据传输
    void analyzeData(int id, QByteArray array);

	//设备组
    void getGroupList();//show
    void addGroup();//show a small widget
    void removeGroup();//show a small widget

	//数据处理

private:
    Ui::MainWindow *ui;

	//通信
    QTcpServer *pTcpServer;
    QTcpSocket *pTcpSocket;
    MyThread *allMachine[200];

	//设备组
    Group *curGroup;
    int machineSize;
    Group *allGroup[200];
    int groupsize;

	//数据处理
    Expression allExpression;

    int cur;
    QWidget *wip;
    QGridLayout *wip_layout;
    QLineEdit *wip_ip;
    QLineEdit *wip_port;
    QPushButton *wip_button;

};

#endif // MAINWINDOW_H
