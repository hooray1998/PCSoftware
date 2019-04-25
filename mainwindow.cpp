#include "mainwindow.h"
#include "ui_mainwindow.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

const qint64 LOADBYTES = 4096; //4K

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DBG<<"init";
    ui->setupUi(this);
    this->setWindowTitle("TcpServer");
    setWindowState(Qt::WindowMaximized);


    initIpWidget();
    initTieGroupWidget();
    initUntieGroupWidget();
    DBG<<"end";


    m_model=new QStringListModel();
    m_model2=new QStringListModel();

    connect(ui->actionSetIP,&QAction::triggered,this,&MainWindow::showIpWidget);
    connect(ui->actionGroupBound,&QAction::triggered,this,&MainWindow::showTieGroupWidget);
    connect(ui->actionGroupUnbound,&QAction::triggered,this,&MainWindow::showUntieGroupWidget);


    DBG<<"init";
    //通信
    pTcpServer = NULL;
    pTcpServer = new QTcpServer(this);
    //只要一建立连接成功，就会自动触发newConnection函数
    connect(pTcpServer, &QTcpServer::newConnection,
            [=]()
            {
                //取出建立好的连接套接字
                pTcpSocket = pTcpServer->nextPendingConnection();

                //获得客户端的IP和端口
                QString ip = pTcpSocket->peerAddress().toString();
                qint16 port = pTcpSocket->peerPort();
                QString msg = QString("[%1:%2]:The client connection is successful.").arg(ip).arg(port);

                ui->logTextEdit->append(msg);

                MyThread *temp = new MyThread(pTcpSocket);
                allMachine.push_back(temp);
                temp->start();

                //-----------------------------------------------------------------------------------------------
                connect(temp, SIGNAL(SendLog(Group*,QByteArray)), this, SLOT(showLog(Group*,QByteArray)));
                connect(temp, SIGNAL(SendLog(QString)), this, SLOT(showLog(QString)));
            }
            );

	//设备组

	//数据处理

}

MainWindow::~MainWindow()
{
    delete ui;
}
//void MainWindow::on_boundPushButton_clicked()
//{
//}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(true)
        event->accept();
}

void MainWindow::showIpWidget()
{
    wip->move((this->width()-wip->width())/2,(this->height()-wip->height())/2);
    wip->show();
}
void MainWindow::showTieGroupWidget(){
    wtie_A->clear();
    wtie_A->addItems(singleStringList);
    wtie_B->clear();
    wtie_B->addItems(singleStringList);
    wtie->move((this->width()-wtie->width())/2,(this->height()-wtie->height())/2);
    wtie->show();
}
void MainWindow::showUntieGroupWidget(){
    //wip->move((this->width()-wip->width())/2,(this->height()-wip->height())/2);
    //wip->show();

}

//设备组
void MainWindow::getGroupList(){

}//show
void MainWindow::addGroup(){

}//show a small widget
void MainWindow::removeGroup(){

}//show a small widget

void MainWindow::listenButtonClickSlot()
{
    QString myAddr = wip_ip->text();     //手动输入IP到edit框
    QString myPort = wip_port->text();       //手动设置端口
    QString msg;
    bool ret = pTcpServer->listen(QHostAddress(myAddr),myPort.toUInt());      //服务器监听绑定
    if(!ret)
    {
        msg = "绑定失败";
    }
    else
    {
        msg = "绑定成功";
        wip_button->setEnabled(false);
        wip->close();
    }

    ui->logTextEdit->append(msg);
}

void MainWindow::showLog(QString msg)
{
    ui->logTextEdit->append(msg);
}


void MainWindow::showLog(Group *group, QByteArray header)
{
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString groupName;
    QString cmd;
    if(!group)
        groupName = "No Group";
    else
        groupName = "Group:"+group->groupInfo.name;

    if(header.right(2)=="01"){
        cmd = "Login in";
    //TODO: find group if it is login in now.
        updateListView();
    }
    else if(header.right(2)=="02"){
        cmd = "Send a";
    }
    else if(header.right(2)=="03"){
        cmd = "Send b&c";
    }
    else if(header.right(2)=="09"){
        cmd = "Login out";
        for(int i=allMachine.size()-1;i>=0;i--)
        {
            if(allMachine.at(i)->die)
                allMachine.remove(i);
        }
    //TODO: delete if it is disconnected.
        updateListView();
    }
    else{
        cmd = "Other msg";
    }

    QString msg = QString("[%1][%2][Machine:%3] => %4").arg(time).arg(groupName).arg(QString(header.left(6))).arg(cmd);
    ui->logTextEdit->append(msg);
}

void MainWindow::updateListView(){
    groupStringList.clear();
    singleStringList.clear();
    for(int i=0;i<allMachine.size();i++)
    {
        if(!allMachine.at(i)->group)
            singleStringList.append(QString("Machine:%1").arg(allMachine.at(i)->getMachineID()));
    }

    m_model2->setStringList(singleStringList);
    ui->listView_2->setModel(m_model2);
}

void MainWindow::initIpWidget(){
    wip = new QWidget;
    wip->resize(300,200);

    wip_layout = new QGridLayout;
    wip_labelip = new QLabel("IP:");
    wip_labelport = new QLabel("Port:");
    wip_ip = new QLineEdit;
    wip_port = new QSpinBox;
    wip_button = new QPushButton("Listen");

    wip_port->setMaximum(99999);
    wip_ip->setText("127.0.0.1");
    wip_port->setValue(8888);

    wip_layout->addWidget(wip_labelip,0,0,1,1);
    wip_layout->addWidget(wip_labelport,1,0,1,1);
    wip_layout->addWidget(wip_ip,0,1,1,5);
    wip_layout->addWidget(wip_port,1,1,1,2);
    wip_layout->addWidget(wip_button,1,4,1,2);
    wip->setLayout(wip_layout);
    connect(wip_button,&QPushButton::clicked,this,&MainWindow::listenButtonClickSlot);

}
void MainWindow::initTieGroupWidget(){
    wtie = new QWidget;
    wtie->resize(300,200);

    wtie_layout = new QGridLayout;
    wtie_labelA = new QLabel("MachineA:");
    wtie_labelB = new QLabel("MachineB:");
    wtie_labelgroupname = new QLabel("GroupName:");
    wtie_groupname = new QLineEdit();
    wtie_A = new QComboBox();
    wtie_B = new QComboBox();
    wtie_button = new QPushButton("BangDing");
    wtie_buttonclose = new QPushButton("GuanBi");


    wtie_layout->addWidget(wtie_labelA,0,0,1,2);
    wtie_layout->addWidget(wtie_labelB,1,0,1,2);
    wtie_layout->addWidget(wtie_labelgroupname,2,0,1,2);
    wtie_layout->addWidget(wtie_A,0,2,1,5);
    wtie_layout->addWidget(wtie_B,1,2,1,5);
    wtie_layout->addWidget(wtie_groupname,2,2,1,5);
    wtie_layout->addWidget(wtie_button,3,0,1,2);
    wtie_layout->addWidget(wtie_buttonclose,3,3,1,2);
    wtie->setLayout(wtie_layout);
    connect(wtie_button,&QPushButton::clicked,this,&MainWindow::tieTwoMachine);

}
void MainWindow::initUntieGroupWidget(){

}

void MainWindow::tieTwoMachine(){

}
void MainWindow::untieTwoMachine(){

}
