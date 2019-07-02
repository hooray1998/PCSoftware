#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "QTXLSX.h"

#include <QStandardItemModel>
#include <QStringListModel>
#include <QStringList>
#include <QTime>
#include <QPalette>
#include <QColor>
#include <QMessageBox>
#include <QFileDialog>


#include <QTableWidgetItem>
#include <QPushButton>
#include <QComboBox>
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
    void resizeEvent(QResizeEvent *event);
signals:
    void changeStyle(const QString &qssFile);

public slots:
    void startVS1(); //开启当前设备
    void startVS2();
    void startJingdu();
    void startVS1(int index); //指定设备启动
    void startVS2(int index);
    void startJingdu(int index);
    void stopDebug();


    void showLog(QString msg);
    void showLog(MyThread* machine, QByteArray header);
    void showLog(QString group,QString msg);

    void changeStyle();             //更换样式
    void initTcpServer();
    void initUI();


    void initIpWidget();
    void showIpWidget();
    void listenButtonClickSlot();

    void initTieGroupWidget();
    void showTieGroupWidget();
    void tieTwoMachine();

    void initUntieGroupWidget();
    void showUntieGroupWidget();
    void untieTwoMachine();

    void initWorkerWidget();
    void setCurWorker(QString w);

    void initVSFormulaWidget();

    bool findMachineInGroup(QString machine, int &index, bool &AorB);
    bool findGroupInGroup(QString group, int &index);


    void showTable(QModelIndex index);
    void updateTable();
    void scrollCurItem1(QTableWidgetItem *cur);
    void scrollCurItem2(QTableWidgetItem *cur);
    void scrollCurItem3(QTableWidgetItem *cur);


    //Worker
    void manageWorker();
    void readWorkerList();
    void saveWorkerList();

    //VSFormula
    void showVSFormula();
    void readVSFormulaList();
    void saveVSFormulaList();

    //save to excel
    void saveTable2Excel();
    void saveAsTable2Excel();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //样式枚举
    enum Style {
        Style_Silvery = 0,          //银色样式
        Style_Blue = 1,             //蓝色样式
        Style_LightBlue = 2,        //淡蓝色样式
        Style_DarkBlue = 3,         //深蓝色样式
        Style_Gray = 4,             //灰色样式
        Style_LightGray = 5,        //浅灰色样式
        Style_DarkGray = 6,         //深灰色样式
        Style_Black = 7,            //黑色样式
        Style_LightBlack = 8,       //浅黑色样式
        Style_DarkBlack = 9,        //深黑色样式
        Style_PSBlack = 10,         //PS黑色样式
        Style_FlatBlack = 11,       //黑色扁平样式
        Style_FlatWhite = 12        //白色扁平样式
    };


    void readGroupShip();
    void saveGroupShip();
    void updateListView();
    void setStyle(Style style);

    //保存退出时状态
    void saveExitStatus();
    void readExitStatus();

    //数据处理



private:
    Ui::MainWindow *ui;

	//通信
    QTcpServer *pTcpServer;
    QTcpSocket *pTcpSocket;
    QVector<MyThread*> allMachine;

	//设备组
    Group *curGroup;
    QVector<Group*> allGroup;
    QVector<QString> allGroupLog;

	//数据处理
    Expression allExpression;

    //Set Ip
    QWidget *wip;
    QGridLayout *wip_layout;
    QLabel *wip_labelip;
    QLabel *wip_labelport;
    QLineEdit *wip_ip;
    QSpinBox *wip_port;
    QPushButton *wip_button;


    //Tie group
    QWidget *wtie;
    QGridLayout *wtie_layout;
    QLabel *wtie_labelA;
    QLabel *wtie_labelB;
    QLabel *wtie_labelgroupname;
    QLabel *wtie_msg;
    QComboBox *wtie_A;
    QComboBox *wtie_B;
    QLineEdit *wtie_groupname;
    QPushButton *wtie_button;
    QPushButton *wtie_buttonclose;

    //Untie group
    QWidget *wuntie;
    QGridLayout *wuntie_layout;
    QLabel *wuntie_label;
    QLabel *wuntie_msg;
    QComboBox *wuntie_group;
    QPushButton *wuntie_button;
    QPushButton *wuntie_buttonclose;


    QStandardItemModel *model;

    QStringListModel *m_model;
    QStringListModel *m_model2;
    QStringList groupStringList;
    QStringList singleStringList;
    QString curGroupName;


    int foo;
    int bar;
    QStringList groupHeaders;
    QStringList headers;
    QStringList headers2;
    QVector<double> *f_vectorArr[16];//f_vector[0] point to originalVector.
    QVector<QString> *s_vectorArr[3];


    QPalette red;
    QPalette black;
    QColor green;

    //change Theme
    QMenu *themeMenu;

    //worker
    QVector<QString> workerList;
    QWidget *wworker;
    QGridLayout *wworker_layout;
    QLabel *wworker_label;
    QComboBox *wworker_workerList;
    QPushButton *wworker_buttonDel;
    QLabel *wworker_label2;
    QLineEdit *wworker_lineedit;
    QPushButton *wworker_buttonAdd;
    QLabel *wworker_msg;
    QPushButton *wworker_buttonclose;


    //vsFormula
    QVector<QString> vsformulaList;
    QWidget *wvsformula;
    QGridLayout *wvsformula_layout;
    QLabel *wvsformula_rule;
    QLabel *wvsformula_label;
    QComboBox *wvsformula_vsformulaList;
    QPushButton *wvsformula_buttonDel;
    QLabel *wvsformula_label2;
    QLineEdit *wvsformula_lineedit;
    QPushButton *wvsformula_buttonAdd;
    QLabel *wvsformula_msg;
    QPushButton *wvsformula_buttonclose;
};

#endif // MAINWINDOW_H
