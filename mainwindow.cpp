#include "mainwindow.h"
#include "ui_mainwindow.h"

const qint64 LOADBYTES = 4096; //4K

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TcpServer");
    setWindowTitle("iTT");
    setWindowState(Qt::WindowMaximized);
    cur = 0;

    wip = new QWidget;
    wip->resize(300,200);

    wip_layout = new QGridLayout;
    wip_ip = new QLineEdit("ip");
    wip_port = new QLineEdit("port");
    wip_button = new QPushButton("Listen");

    wip_layout->addWidget(wip_ip,0,0,1,4);
    wip_layout->addWidget(wip_port,1,0,1,2);
    wip_layout->addWidget(wip_button,1,2,1,2);
    wip->setLayout(wip_layout);



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
                QString temp = QString("[%1:%2]:The client connection is successful.").arg(ip).arg(port);

                //ui->textEditRead->append(temp);

                allMachine[cur] = new MyThread(cur, pTcpSocket);
                allMachine[cur]->start();

                //-----------------------------------------------------------------------------------------------
                //接收到了内容之后，直接在显示区域显示消息
                 connect(allMachine[cur], SIGNAL(ReadData(int, QByteArray)),this,SLOT(analyzeData(int, QByteArray)));
				// 根据ID分析是否是意外掉线，是否存在当前已有的设备组中。
                //-----------------------------------------------------------------------------------------------
                cur++;
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
    QString myAddr = ui->ipLineEdit->text();     //手动输入IP到edit框
    QString myPort = ui->portLineEdit->text();       //手动设置端口
    QString msg;
    bool ret = pTcpServer->listen(QHostAddress(myAddr),myPort.toUInt());      //服务器监听绑定
    if(!ret)
    {
        msg = "绑定失败";
    }
    else
    {
        msg = "绑定成功";
        //ui->boundPushButton->setEnabled(false);
    }

    ui->textEditRead->append(msg);
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
