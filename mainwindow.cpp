#include "mainwindow.h"
#include "ui_mainwindow.h"

const qint64 LOADBYTES = 4096; //4K

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TcpServer");
    setWindowState(Qt::WindowMaximized);

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


    QStringList strList;
    strList<<"1"<<"22"<<"333"<<"4444";
    m_model=new QStringListModel(strList);
    ui->listView->setModel(m_model);
    connect(ui->actionSetIP,&QAction::triggered,this,&MainWindow::SetIP_Port);


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
                //接收到了内容之后，直接在显示区域显示消息
                connect(temp, &MyThread::ReadData, this, &MainWindow::analyzeData);
                connect(temp, &MyThread::SendLog, this, &MainWindow::showLog);
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

void MainWindow::SetIP_Port()
{

    wip->show();


    /*
    */
}

void MainWindow::analyzeData(int id, QByteArray array){

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
    }

    ui->logTextEdit->append(msg);
}

void MainWindow::showLog(QString msg)
{
    ui->logTextEdit->append(msg);
    //TODO: delete if it is disconnected.
    //TODO: find group if it is login in now.
}

