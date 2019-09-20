#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QInputDialog>
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
//#include <QDebug>
#include <QSet>
#include <QFile>
#include <QFileInfoList>
#include <QDir>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

#include "group.h"
#include "mythread.h"
#include "expression.h"

#include "QTXLSX.h"
#include "setting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event); //程序退出时触发
    void resizeEvent(QResizeEvent *event); //窗口大小变化时触发
signals:
    void changeStyle(const QString &qssFile);//改变主题

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



    //数据处理


public slots:
    void startVS1(); //开始vs1调试
    void startVS2(); //开始vs2调试
    void startJingdu(); //开始精度调试
    void startVS1(int index);
    void startVS2(int index);
    void startJingdu(int index);
    void stopDebug(); //停止调试


    void showLog(QString msg); //显示log
    void showLog(MyThread* machine, QByteArray header); //显示log
    void showLog(QString group,QString msg); //显示log

    void changeStyle();             //更换样式
    void initTcpServer();   //初始化tcp服务器
    void initUI();  //初始化UI

    void showSetting(); //参数设置界面

    void initIpWidget(); //初始化ip修改界面
    void showIpWidget(); //显示ip修改界面
    void listenButtonClickSlot(); //ip监听按钮点击后触发

    void initTieGroupWidget(); //init绑定界面
    void showTieGroupWidget(); //show绑定界面
    void tieTwoMachine(); //绑定界面绑定按钮点击后触发

    void initUntieGroupWidget(); //初始化解绑界面
    void showUntieGroupWidget(); //show解绑界面
    void untieTwoMachine(); //解绑界面解绑按钮点击

    void initWorkerWidget(); //工作人员界面
    void setCurWorker(QString w); //设置当前工作人员

    void initVSFormulaWidget(); //公式修改界面初始化

    bool findMachineInGroup(QString machine, int &index, bool &AorB); //从所有设备中查找某设备，返回是否找到。结果存到index中。
    bool findGroupInGroup(QString group, int &index); //从所有设备组查找group设备组的索引


    void showTable(QModelIndex index); //显示表格
    void updateTable(); //更新表格
    void scrollCurItem1(QTableWidgetItem *cur); //聚焦某个表格项
    void scrollCurItem2(QTableWidgetItem *cur); //聚焦某个表格项
    void scrollCurItem3(QTableWidgetItem *cur); //聚焦某个表格项

    void changeComment1(QModelIndex index); //修改备注信息
    void changeComment2(QModelIndex index); //修改备注信息
    void changeComment3(QModelIndex index); //修改备注信息

    void readConfig(); //读取配置信息
    void saveConfig(); //保存配置信息

    void updateAllDebugValue();
    void setInitValue(int index, bool all); //使用当前的参数初始化index设备组

    //Worker
    void manageWorker(); //工作人员界面

    //VSFormula
    void showVSFormula(); //公式界面


    void updateListView(); //更新左侧设备组和未绑定设备列表
    void updateDebugLabel(int index);
    void setStyle(Style style); //修改皮肤


    //save to excel
    void saveTable2Excel(); //保存表格到 excel文件
    void saveAsTable2Excel(); //另存为


	void slot_connect(); // 收到tcp连接请求，处理连接
	void slot_addWorkers(); // add工作人员
	void slot_delWorkers(); //delete工作人员
	void slot_addFormula(); //add 公式
	void slot_delFormula(); //delete 公式

private slots:
    void on_pushButtonChangeJD_clicked();  //切换为精度调试按钮点击后触发

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_3_clicked();

private:
    Ui::MainWindow *ui;

	//通信
    QTcpServer *pTcpServer; //tcp服务器
    QTcpSocket *pTcpSocket; //临时的tcp接口
    QVector<MyThread*> allMachine; //存储所有的设备对应的线程

	//设备组
    Group *curGroup;  //当前显示的设备组
    QVector<Group*> allGroup; //存储所有的设备组
    QVector<QString> allGroupLog; //所有设备组最新的消息，左上角表格显示的

    //Set Ip ,  ip设置界面相关
    QWidget *wip;
    QGridLayout *wip_layout;
    QLabel *wip_labelip;
    QLabel *wip_labelport;
    QLineEdit *wip_ip;
    QSpinBox *wip_port;
    QPushButton *wip_button;

    //Tie group ,  设备绑定界面相关
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

    //Untie group  设备解绑界面相关
    QWidget *wuntie;
    QGridLayout *wuntie_layout;
    QLabel *wuntie_label;
    QLabel *wuntie_msg;
    QComboBox *wuntie_group;
    QPushButton *wuntie_button;
    QPushButton *wuntie_buttonclose;


    QStandardItemModel *model; //显示设备组的model

    QStringListModel *m_model2; //未绑定列表的model
    QStringList groupStringList; //所有设备组的名称，显示在左上角
    QStringList singleStringList; //未绑定列表的名称列表
    QString curGroupName; //当前设备组的名称


    int foo; //当前窗口的width的100分之1
    int bar; //当前窗口的height的100分之1

    QStringList groupHeaders; //设备组表格的表头
    QStringList headers; //vs调试的表头
    QStringList headers2; //精度调试的表头
    QVector<double> *f_vectorArr[16];//f_vector[0] point to originalVector. 存储表格中所有的数值
    QVector<QString> *s_vectorArr[3]; //存储表格中所有的文字


    QPalette red; //红色背景
    QPalette black; //黑色背景
    QColor green; //绿色

    //change Theme
    QMenu *themeMenu; //皮肤菜单

    //worker ,  工作人员界面相关
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


    //vsFormula , 公式修改界面相关
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

	Setting debugInitValue; //参数修改界面
};

#endif // MAINWINDOW_H
