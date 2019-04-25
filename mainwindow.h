#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QStringList>


#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
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
    void showLog(QString msg);
    void SetIP_Port();
    void listenButtonClickSlot();

    //
    //设备组
    void getGroupList();//show
    void addGroup();//show a small widget
    void removeGroup();//show a small widget



    //数据传输
    void analyzeData(int id, QByteArray array);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();





	//数据处理

private:
    Ui::MainWindow *ui;

	//通信
    QTcpServer *pTcpServer;
    QTcpSocket *pTcpSocket;
    QVector<MyThread*> allMachine;

	//设备组
    Group *curGroup;
    //int machineSize;
    //Group *allGroup[200];
    QVector<Group*> allGroup;
    //int groupsize;

	//数据处理
    Expression allExpression;

    QWidget *wip;
    QGridLayout *wip_layout;
    QLabel *wip_labelip;
    QLabel *wip_labelport;
    QLineEdit *wip_ip;
    QSpinBox *wip_port;
    QPushButton *wip_button;
    QStringListModel *m_model;

};

#endif // MAINWINDOW_H
