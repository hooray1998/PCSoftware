#include "mainwindow.h"
#include "ui_mainwindow.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

const qint64 LOADBYTES = 4096; //4K

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("上位机软件");
    setStyle(MainWindow::Style_Silvery);
    setWindowState(Qt::WindowMaximized);

    initUI();

    initIpWidget();
    initTieGroupWidget();
    initUntieGroupWidget();
    initWorkerWidget();
    initVSFormulaWidget();
    initTcpServer();

	//设备组

	//数据处理

    listenButtonClickSlot();//auto to connect
    readGroupShip();
    readWorkerList();
    readVSFormulaList();
    updateListView();
    setCurWorker(ui->comboBoxWorker1->currentText());


    ui->doubleSpinBoxVS1Value_vsmode->setValue(777.77);
    ui->doubleSpinBoxVS2Value_vsmode->setValue(999.4);

}

MainWindow::~MainWindow()
{
    delete wip;
    delete wtie;
    delete wuntie;

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveGroupShip();
    saveWorkerList();
    saveVSFormulaList();
    wip->close();
    wtie->close();
    wuntie->close();
    wworker->close();
    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent *event){
    //set layout
    foo = this->width()/100;
    bar = this->height()/100;
    ui->logTextEdit->setFixedWidth(foo*82);
    ui->listView_2->setFixedHeight(bar*33);


    event->accept();

}

void MainWindow::initUI(){

    ui->tableWidgetvs1->setRowCount(200);
    ui->tableWidgetvs1->setColumnCount(10);
    ui->tableWidgetvs2->setRowCount(200);
    ui->tableWidgetvs2->setColumnCount(10);
    ui->tableWidget_2->setRowCount(200);
    ui->tableWidget_2->setColumnCount(20);
    ui->tabWidget->setCurrentIndex(0);

    headers<<"yuanzhi"<<"b"<<"r"<<"r-b"<<"a"<<"diata"<<"finnal";
    ui->tableWidgetvs1->setHorizontalHeaderLabels(headers);
    ui->tableWidgetvs2->setHorizontalHeaderLabels(headers);
    //ui->tableWidgetvs1->setFixedWidth(ui->tabWidget->width());
    ui->tableWidgetvs1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //test1();
    ui->tableWidgetvs1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetvs2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    //ui->listView->setUpdatesEnabled(true);
    ui->listView_2->setUpdatesEnabled(true);

    ui->comboBoxWorker1->setFixedWidth(140);
    ui->lineEditJiNumber_mode1->setFixedWidth(140);


    //m_model=new QStringListModel();
    m_model2=new QStringListModel();

    model = new QStandardItemModel;
    groupHeaders<<"设备组"<<"状态";
    model->setColumnCount(2);

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setShowGrid(false);//主体部分
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    //ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    //ui->tableView->setColumnWidth(0,20);

    red.setColor(QPalette::WindowText, Qt::red);
    black.setColor(QPalette::WindowText, Qt::black);
    green.setRgb(127,0,0,127);

    themeMenu = new QMenu();
    themeMenu->setTitle("改变主题");

    //添加换肤菜单
    QStringList name;
    name << "银色" << "浅蓝色" << "灰色" << "PS黑色";

    foreach (QString str, name) {
        QAction *action = new QAction(str, this);
        themeMenu->addAction(action);
        connect(action, SIGNAL(triggered(bool)), this, SLOT(changeStyle()));
    }
    ui->menu_4->addMenu(themeMenu);

    connect(ui->tableWidgetvs1,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(scrollCurItem(QTableWidgetItem*)));

    connect(ui->actionSetIP,&QAction::triggered,this,&MainWindow::showIpWidget);
    connect(ui->actionGroupBound,&QAction::triggered,this,&MainWindow::showTieGroupWidget);
    connect(ui->pushButtonTie,&QPushButton::clicked,this,&MainWindow::showTieGroupWidget);
    connect(ui->actionGroupUnbound,&QAction::triggered,this,&MainWindow::showUntieGroupWidget);
    connect(ui->pushButtonUntie,&QPushButton::clicked,this,&MainWindow::showUntieGroupWidget);
    connect(ui->actionAdd_Worker,&QAction::triggered,this,&MainWindow::manageWorker);
    connect(ui->actionVSFormula,&QAction::triggered,this,&MainWindow::showVSFormula);
    connect(ui->tableView,&QTableView::clicked,this,&MainWindow::showTable);

    connect(ui->pushButtonStop,&QPushButton::clicked, this, &MainWindow::stopDebug);
    connect(ui->pushButtonStartVS1,&QPushButton::clicked,this,&MainWindow::startVS1);
    connect(ui->pushButtonStartVS2,&QPushButton::clicked,this,&MainWindow::startVS2);

    connect(ui->comboBoxWorker1,&QComboBox::currentTextChanged,this,&MainWindow::setCurWorker);

    ui->doubleSpinBoxVS1Value_vsmode->setMaximum(9999);
    ui->doubleSpinBoxVS2Value_vsmode->setMaximum(9999);
    ui->doubleSpinBoxVS1Value_jdmode->setMaximum(9999);
    ui->doubleSpinBoxVS2Value_jdmode->setMaximum(9999);
    ui->doubleSpinBoxYinliuValue_jdmode->setMaximum(9999);


}

void MainWindow::changeStyle()
{
    QAction *act = (QAction *)sender();
    QString name = act->text();
    QString qssFile = ":/qss/blue.css";

    if (name == "银色") {
        qssFile = ":/qss/silvery.css";
        setStyle(MainWindow::Style_Silvery);
    } else if (name == "蓝色") {
        qssFile = ":/qss/blue.css";
        setStyle(MainWindow::Style_Blue);
    } else if (name == "浅蓝色") {
        qssFile = ":/qss/lightblue.css";
        setStyle(MainWindow::Style_LightBlue);
    } else if (name == "深蓝色") {
        qssFile = ":/qss/darkblue.css";
        setStyle(MainWindow::Style_DarkBlue);
    } else if (name == "灰色") {
        qssFile = ":/qss/gray.css";
        setStyle(MainWindow::Style_Gray);
    } else if (name == "浅灰色") {
        qssFile = ":/qss/lightgray.css";
        setStyle(MainWindow::Style_LightGray);
    } else if (name == "深灰色") {
        qssFile = ":/qss/darkgray.css";
        setStyle(MainWindow::Style_DarkGray);
    } else if (name == "黑色") {
        qssFile = ":/qss/black.css";
        setStyle(MainWindow::Style_Black);
    } else if (name == "浅黑色") {
        qssFile = ":/qss/lightblack.css";
        setStyle(MainWindow::Style_LightBlack);
    } else if (name == "深黑色") {
        qssFile = ":/qss/darkblack.css";
        setStyle(MainWindow::Style_DarkBlack);
    } else if (name == "PS黑色") {
        qssFile = ":/qss/psblack.css";
        setStyle(MainWindow::Style_PSBlack);
    } else if (name == "黑色扁平") {
        qssFile = ":/qss/flatblack.css";
        setStyle(MainWindow::Style_FlatBlack);
    } else if (name == "白色扁平") {
        qssFile = ":/qss/flatwhite.css";
        setStyle(MainWindow::Style_FlatWhite);
    }

    emit changeStyle(qssFile);
}


void MainWindow::setStyle(MainWindow::Style style)
{
    QString qssFile = ":/qss/blue.css";

    if (style == MainWindow::Style_Silvery) {
        qssFile = ":/qss/silvery.css";
    } else if (style == MainWindow::Style_Blue) {
        qssFile = ":/qss/blue.css";
    } else if (style == MainWindow::Style_LightBlue) {
        qssFile = ":/qss/lightblue.css";
    } else if (style == MainWindow::Style_DarkBlue) {
        qssFile = ":/qss/darkblue.css";
    } else if (style == MainWindow::Style_Gray) {
        qssFile = ":/qss/gray.css";
    } else if (style == MainWindow::Style_LightGray) {
        qssFile = ":/qss/lightgray.css";
    } else if (style == MainWindow::Style_DarkGray) {
        qssFile = ":/qss/darkgray.css";
    } else if (style == MainWindow::Style_Black) {
        qssFile = ":/qss/black.css";
    } else if (style == MainWindow::Style_LightBlack) {
        qssFile = ":/qss/lightblack.css";
    } else if (style == MainWindow::Style_DarkBlack) {
        qssFile = ":/qss/darkblack.css";
    } else if (style == MainWindow::Style_PSBlack) {
        qssFile = ":/qss/psblack.css";
    } else if (style == MainWindow::Style_FlatBlack) {
        qssFile = ":/qss/flatblack.css";
    } else if (style == MainWindow::Style_FlatWhite) {
        qssFile = ":/qss/flatwhite.css";
    }

    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
}

void MainWindow::initTcpServer(){
    //通信
    pTcpServer = NULL;
    pTcpServer = new QTcpServer(this);
    //只要一建立连接成功，就会自动触发newConnection函数
    connect(pTcpServer, &QTcpServer::newConnection,
            [=]()
            {
                //取出建立好的连接套接字
                pTcpSocket = pTcpServer->nextPendingConnection();
                MyThread *temp = new MyThread(pTcpSocket);
                allMachine.push_back(temp);
                temp->start();

                connect(temp, SIGNAL(SendLog(QString)), this, SLOT(showLog(QString)));
                connect(temp, SIGNAL(SendLog(MyThread*,QByteArray)), this, SLOT(showLog(MyThread*,QByteArray)));
            }
            );
}
void MainWindow::showIpWidget()
{
    wip->move((this->width()-wip->width())/2,(this->height()-wip->height())/2);
    wip->hide();
    wip->show();
}
void MainWindow::showTieGroupWidget(){
    wtie_msg->clear();
    wtie_groupname->clear();
    wtie->move((this->width()-wtie->width())/2,(this->height()-wtie->height())/2);
    wtie->hide();
    wtie->show();
}
void MainWindow::showUntieGroupWidget(){
    QItemSelectionModel *model_selection = ui->tableView->selectionModel();
    QModelIndexList IndexList= model_selection->selectedIndexes();
    QMap<int,int>rowMap;
    foreach (QModelIndex index, IndexList)
    {
        if(! index.isValid()) return;
        //if(!index.column()==0) return ;
        rowMap.insert(index.row(),0);
        //)qDebug()<<"del row..."<<index.row();
    }

    QMapIterator<int,int> Iterator(rowMap);
    Iterator.toBack();
    while(Iterator.hasPrevious())
    {
        Iterator.previous();
        int rowm=Iterator.key();
        qDebug()<<"delete row..."<<rowm;
        allGroup.remove(rowm);
        //file_model->removeRow(rowm);
    }
    updateListView();

    /*
    wuntie_msg->clear();
    wuntie->move((this->width()-wuntie->width())/2,(this->height()-wuntie->height())/2);
    wuntie->hide();
    wuntie->show();
    */
}


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
        msg = "绑定成功 IP:"+myAddr+"   Port:"+myPort;
        wip_button->setEnabled(false);
        wip->close();
    }

    ui->logTextEdit->append(msg);
}

void MainWindow::showLog(QString msg)
{
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString msg2 = QString("[%1] => %2").arg(time).arg(msg);
    ui->logTextEdit->append(msg2);
}


void MainWindow::showLog(MyThread* machine, QByteArray header)
{

    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString groupName;
    QString cmd;
    if(!machine->getGroup())
        groupName = "No Group";
    else
        groupName = "Group:"+machine->getGroup()->groupInfo.name;

    if(header.right(2)=="01"){
        cmd = "登录";
        //TODO: find group if it is login in now.
        int index;
        bool AorB, ok = findMachineInGroup(header.left(6), index, AorB);
        if(ok){
            allGroup.at(index)->login(machine);
            groupName = "Group:"+allGroup.at(index)->groupInfo.name;
        }
        updateListView();
    }
    else if(header.right(2)=="09"){
        cmd = "下线";
        for(int i=allMachine.size()-1;i>=0;i--)
        {
            if(allMachine.at(i)->die)
                allMachine.remove(i);
        }
        updateListView();
    }
    else{
        cmd = "Other msg";
    }
    QString msg = QString("[%1][%2][Machine:%3] => %4").arg(time).arg(groupName).arg(QString(header.left(6))).arg(cmd);

    ui->logTextEdit->append(msg);
}

void MainWindow::showLog(QString group,QString msg)
{
    DBG<<"group"<<group<<"msg:"<<msg;
    if(group==curGroupName)
        updateTable();

    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString msg2 = QString("[%1][%2] => %4").arg(time).arg(group).arg(msg);
    ui->logTextEdit->append(msg2);
}

void MainWindow::updateListView(){
    groupStringList.clear();
    singleStringList.clear();
    model->clear();
    //model->setHorizontalHeaderLabels(groupHeaders);
    for(int i=0;i<allGroup.size();i++)
    {
        groupStringList.append(allGroup.at(i)->groupInfo.name);
        model->setItem(i,0,new QStandardItem(allGroup.at(i)->groupInfo.name));
        model->setItem(i,1,new QStandardItem("some message in there."));
    }

    for(int i=0;i<allMachine.size();i++)
    {
        if(!allMachine.at(i)->getGroup())
            singleStringList.append(QString("%1").arg(allMachine.at(i)->getMachineID()));
    }

    //m_model->setStringList(groupStringList);

    //ui->listView->setModel(m_model);



    m_model2->setStringList(singleStringList);
    ui->listView_2->setModel(m_model2);

    //update Tie widget and untiewidget
    wtie_A->clear();
    wtie_A->addItems(singleStringList);
    wtie_B->clear();
    wtie_B->addItems(singleStringList);

    wuntie_group->clear();
    wuntie_group->addItems(groupStringList);

    if(singleStringList.size()==0)
        ui->listView_2->hide();
    else
        ui->listView_2->show();
}

void MainWindow::initIpWidget(){
    wip = new QDialog;
    wip->resize(300,200);

    wip_layout = new QGridLayout;
    wip_labelip = new QLabel("IP:");
    wip_labelport = new QLabel("Port:");
    wip_ip = new QLineEdit;
    wip_port = new QSpinBox;
    wip_button = new QPushButton("绑定");

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
    wtie = new QDialog;
    wtie->setWindowTitle("绑定设备");
    wtie->resize(300,200);

    wtie_layout = new QGridLayout;
    wtie_labelA = new QLabel("设备A:");
    wtie_labelB = new QLabel("设备B:");
    wtie_msg = new QLabel();
    wtie_labelgroupname = new QLabel("设备组名称:");
    wtie_groupname = new QLineEdit();
    wtie_A = new QComboBox();
    wtie_B = new QComboBox();
    wtie_button = new QPushButton("绑定");
    wtie_buttonclose = new QPushButton("关闭");


    wtie_layout->addWidget(wtie_labelA,0,0,1,2);
    wtie_layout->addWidget(wtie_labelB,1,0,1,2);
    wtie_layout->addWidget(wtie_labelgroupname,2,0,1,2);
    wtie_layout->addWidget(wtie_A,0,2,1,5);
    wtie_layout->addWidget(wtie_B,1,2,1,5);
    wtie_layout->addWidget(wtie_groupname,2,2,1,5);
    wtie_layout->addWidget(wtie_msg,3,0,1,5);
    wtie_layout->addWidget(wtie_button,4,0,1,2);
    wtie_layout->addWidget(wtie_buttonclose,4,3,1,2);
    wtie->setLayout(wtie_layout);
    connect(wtie_button,&QPushButton::clicked,this,&MainWindow::tieTwoMachine);
    connect(wtie_buttonclose,&QPushButton::clicked,wtie,&QDialog::close);

}
void MainWindow::initUntieGroupWidget(){
    wuntie = new QDialog;
    wtie->setWindowTitle("解绑设备组");
    wtie->resize(300,200);

    wuntie_layout = new QGridLayout;
    wuntie_label = new QLabel("设备组名称:");
    wuntie_msg = new QLabel();
    wuntie_group = new QComboBox;
    wuntie_button = new QPushButton("解绑");
    wuntie_buttonclose = new QPushButton("关闭");

    wuntie_layout->addWidget(wuntie_label,0,0,1,2);
    wuntie_layout->addWidget(wuntie_group,0,2,1,3);
    wuntie_layout->addWidget(wuntie_msg,1,0,1,5);
    wuntie_layout->addWidget(wuntie_button,3,0,1,2);
    wuntie_layout->addWidget(wuntie_buttonclose,3,3,1,2);
    wuntie->setLayout(wuntie_layout);
    connect(wuntie_button,&QPushButton::clicked,this,&MainWindow::untieTwoMachine);
    connect(wuntie_buttonclose,&QPushButton::clicked,wuntie,&QDialog::close);
}

void MainWindow::tieTwoMachine(){
    wtie_msg->setPalette(black);

    //judge group name
    if(wtie_groupname->text().size()==0)
    {
        wtie_msg->setPalette(red);
        wtie_msg->setText("错误：名称不能为空。");
        return ;
    }
    else{
        for(int i=0;i<allGroup.size();i++)
            if(allGroup.at(i)->groupInfo.name==wtie_groupname->text())
            {
                wtie_msg->setPalette(red);
                wtie_msg->setText("错误：名称已经存在。");
                return ;
            }
    }
    if(wtie_A->currentText()==wtie_B->currentText())
    {
        wtie_msg->setPalette(red);
        wtie_msg->setText("错误：设备A和B不能相同。");
        return ;
    }
    else{
        MyThread *a = NULL, *b = NULL;
        for(int i=0;i<allMachine.size();i++)
            if(allMachine.at(i)->getMachineID() == wtie_A->currentText())
            {
                if(allMachine.at(i)->getGroup()==NULL)
                    a = allMachine.at(i);
                else{
                    wtie_msg->setPalette(red);
                    wtie_msg->setText("错误：设备A已经被其他设备绑定。");
                    return ;
                }
            }
        for(int i=0;i<allMachine.size();i++)
            if(allMachine.at(i)->getMachineID() == wtie_B->currentText())
            {
                if(allMachine.at(i)->getGroup()==NULL)
                    b = allMachine.at(i);
                else{
                    wtie_msg->setPalette(red);
                    wtie_msg->setText("错误：设备B已经被其他设备绑定。");
                    return ;
                }
            }

        if(!a||!b)
        {
            wtie_msg->setPalette(red);
            wtie_msg->setText("错误：设备A或B不存在。");
            return ;
        }
        Group *g = new Group();
        g->tie(wtie_groupname->text(),a,b);
        allGroup.push_back(g);
        connect(g,SIGNAL(SendLog(QString,QString)),this,SLOT(showLog(QString,QString)));
        wtie_msg->setText("绑定成功。");
        wtie_groupname->clear();
    }

    updateListView();

}
void MainWindow::untieTwoMachine(){
    wuntie->setPalette(black);
    Group *g = NULL;
    for(int i=0;i<allGroup.size();i++)
        if(allGroup.at(i)->groupInfo.name==wuntie_group->currentText())
        {
            g = allGroup.at(i);
            allGroup.remove(i);
        }
    if(!g)
    {
        wuntie->setPalette(red);
        wuntie_msg->setText("错误：设备组不存在。");
        return ;
    }

    g->untie();
    wuntie_msg->setText("解绑成功。");
    updateListView();
}

void MainWindow::showTable(QModelIndex index){

    //组名在第一列
    curGroupName = model->data(index.sibling(index.row(),0)).toByteArray();
    ui->lineEditJiNumber_mode1->setText(curGroupName);

    updateTable();
}

void MainWindow::startVS1()
{
    if(!curGroupName.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){
        //judge if both are online.
        if(3 == allGroup.at(index)->getOnlineStatus())
        {
            if(allGroup.at(index)->allData.curAction==AllData::Action_die)
            {
                allGroup.at(index)->allData.curMode = AllData::Mode_VS1;
                allGroup.at(index)->allData.initValue_VS1_modeVS = ui->doubleSpinBoxVS1Value_vsmode->value();
                allGroup.at(index)->allData.VSCount = 0;
                allGroup.at(index)->allData.curWorker = ui->comboBoxWorker1->currentText();
                allGroup.at(index)->request_b();
            }
            else{
                showLog("This group is doing other action.");
            }
        }
    }
}

void MainWindow::startVS2(){

}

void MainWindow::startJingdu(){

}

void MainWindow::saveGroupShip(){
    QFile *file = new QFile("groupShip");
    bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(ok){
        DBG<<"write groupShip.";
        QTextStream out(file);

        for(int i=0;i<allGroup.size();i++)
        {
            out<<allGroup[i]->groupInfo.name<<" "<<allGroup[i]->groupInfo.machineA_id<<" "<<allGroup[i]->groupInfo.machineB_id<<" ";
            DBG<<allGroup[i]->groupInfo.name<<" "<<allGroup[i]->groupInfo.machineA_id<<" "<<allGroup[i]->groupInfo.machineB_id<<" ";
        }
    }
    file->close();
    delete file;
    file = NULL;

}

void MainWindow::readGroupShip(){
    QFile *file = new QFile("groupShip");
    bool ok = file->open(QIODevice::ReadOnly|QIODevice::Text);
    if(ok){
        DBG<<"read groupShip.";
        QTextStream in(file);
        QString n,a,b;

        while(!in.atEnd())
        {
            in>>n>>a>>b;
            if(!n.size()||!a.size()||!b.size())
                break;
            Group *g = new Group();
            g->tie_byID(n,a,b);
            allGroup.push_back(g);
            connect(g,SIGNAL(SendLog(QString,QString)),this,SLOT(showLog(QString,QString)));
        }
        file->close();
        delete file;
        file = NULL;
    }
}

void MainWindow::updateTable(){
    if(!curGroupName.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){

        if(!allGroup.at(index)->allData.returnData_FromVS(f_vecotrArr, s_vectorArr))
                return;
        DBG<<"update all table.";
        //TODO: show all data
        ui->tableWidgetvs1->clear();

        for(int i=0;i<7;i++)
            for(int j=0;j<f_vecotrArr[i]->size();j++)
            {
                ui->tableWidgetvs1->setItem(j,i,new QTableWidgetItem(QString::number(f_vecotrArr[i]->at(j))));
            }

        for(int i=0;i<3;i++)
            for(int j=0;j<s_vectorArr[i]->size();j++)
            {
                ui->tableWidgetvs1->setItem(j,i+7,new QTableWidgetItem(s_vectorArr[i]->at(j)));
            }
    }

    ui->tableWidgetvs1->setHorizontalHeaderLabels(headers);
    ui->tableWidgetvs2->setHorizontalHeaderLabels(headers);

}

bool MainWindow::findGroupInGroup(QString group, int &index){

    for(int i=0;i<allGroup.size();i++)
    {
        if(allGroup.at(i)->groupInfo.name == group){
            index = i;
            return true;
        }
    }
    return false;
}

bool MainWindow::findMachineInGroup(QString machine, int &index, bool &AorB){
    for(int i=0;i<allGroup.size();i++)
    {
        if(allGroup.at(i)->getMachineA_id()==machine)
        {
            index = i;
            AorB = true;
            return true;
            break;
        }
        else if(allGroup.at(i)->getMachineB_id()==machine)
        {
            index = i;
            AorB = false;
            return true;
            break;
        }
    }
    return false;
}

void MainWindow::scrollCurItem(QTableWidgetItem *cur){
    ui->tableWidgetvs1->scrollToItem(cur);
}

void MainWindow::manageWorker(){
    wworker_msg->clear();
    wworker->move((this->width()-wworker->width())/2,(this->height()-wworker->height())/2);
    wworker->hide();
    wworker->show();
}

void MainWindow::readWorkerList(){
    QFile *file = new QFile("workerList");
    bool ok = file->open(QIODevice::ReadOnly|QIODevice::Text);
    if(ok){
        DBG<<"read workerList.";
        QTextStream in(file);
        QString n;

        while(!in.atEnd())
        {
            in>>n;
            if(!n.size())
                break;
            workerList.push_back(n);
            wworker_workerList->addItem(n);
            ui->comboBoxWorker1->addItem(n);
        }
        file->close();
        delete file;
        file = NULL;
    }

}

void MainWindow::saveWorkerList(){
    QFile *file = new QFile("workerList");
    bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(ok){
        DBG<<"write workerList.";
        QTextStream out(file);

            for(int i=0;i<workerList.size();i++)
            {
                out<<workerList[i]<<endl;
            }
        }
        file->close();
        delete file;
        file = NULL;
}

void MainWindow::initWorkerWidget(){

    wworker = new QDialog;
    wworker->resize(400,200);

    wworker_layout = new QGridLayout;
    wworker_label = new QLabel("工作人员列表：");
    wworker_workerList = new QComboBox;
    wworker_buttonDel = new QPushButton("删除");
    wworker_label2 = new QLabel("新增人员：");
    wworker_lineedit = new QLineEdit;
    wworker_buttonAdd = new QPushButton("增加");
    wworker_msg = new QLabel;
    wworker_buttonclose = new QPushButton("关闭");


    wworker_layout->addWidget(wworker_label,0,0,1,2);
    wworker_layout->addWidget(wworker_workerList,0,2,1,3);
    wworker_layout->addWidget(wworker_buttonDel,0,5,1,2);
    wworker_layout->addWidget(wworker_label2,1,0,1,2);
    wworker_layout->addWidget(wworker_lineedit,1,2,1,3);
    wworker_layout->addWidget(wworker_buttonAdd,1,5,1,2);
    wworker_layout->addWidget(wworker_msg,2,0,2,7);
    wworker_layout->addWidget(wworker_buttonclose,4,2,1,3);
    wworker->setLayout(wworker_layout);

   // con7
    connect(wworker_buttonAdd,&QPushButton::clicked,[=](){
        QString worker = wworker_lineedit->text();
        if(worker.size())
        {
            if(workerList.contains(worker))
            {
                wworker_msg->setText("已经存在。");
            }
            else{
                workerList.push_back(worker);
                wworker_workerList->addItem(worker);
                ui->comboBoxWorker1->addItem(worker);
                wworker_msg->setText("添加成功。");
                wworker_lineedit->clear();
            }
        }
    });
    connect(wworker_buttonDel,&QPushButton::clicked,[=](){
        QString worker = wworker_workerList->currentText();
        if(worker.size())
        {
            if(workerList.contains(worker))
            {
                for(int i=0;i<workerList.size();i++)
                {
                    if(workerList[i] == worker){
                        workerList.remove(i);
                        break;
                    }
                }
                wworker_workerList->removeItem(wworker_workerList->currentIndex());
                ui->comboBoxWorker1->removeItem(ui->comboBoxWorker1->findText(worker));
                wworker_msg->setText("移除成功。");
            }
            else{
                wworker_msg->setText("列表中不存在该人员。");
            }
        }
    });
    connect(wworker_buttonclose, &QPushButton::clicked, wworker, &QDialog::close);
}

void MainWindow::setCurWorker(QString w){
    for(int i=0;i<allGroup.size();i++)
        allGroup[i]->allData.curWorker = w;
}

void MainWindow::stopDebug(){
    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){
        allGroup[index]->stop();
        showLog("终止了当前调试");
    }
    updateTable();

}

void MainWindow::initVSFormulaWidget(){
    wvsformula = new QDialog;
    wvsformula->resize(600,400);
    wvsformula->setWindowTitle("VS公式编辑");

    wvsformula_layout = new QGridLayout;
    wvsformula_rule = new QLabel("规则");
    wvsformula_label = new QLabel("VS公式列表：");
    wvsformula_vsformulaList = new QComboBox;
    wvsformula_buttonDel = new QPushButton("删除");
    wvsformula_label2 = new QLabel("新增公式：");
    wvsformula_lineedit = new QLineEdit;
    wvsformula_buttonAdd = new QPushButton("增加");
    wvsformula_msg = new QLabel;
    wvsformula_buttonclose = new QPushButton("关闭");


    wvsformula_layout->addWidget(wvsformula_rule,0,0,2,7);
    wvsformula_layout->addWidget(wvsformula_label,2,0,1,2);
    wvsformula_layout->addWidget(wvsformula_vsformulaList,2,2,1,3);
    wvsformula_layout->addWidget(wvsformula_buttonDel,2,5,1,2);
    wvsformula_layout->addWidget(wvsformula_label2,3,0,1,2);
    wvsformula_layout->addWidget(wvsformula_lineedit,3,2,1,3);
    wvsformula_layout->addWidget(wvsformula_buttonAdd,3,5,1,2);
    wvsformula_layout->addWidget(wvsformula_msg,4,0,2,7);
    wvsformula_layout->addWidget(wvsformula_buttonclose,6,2,1,3);
    wvsformula->setLayout(wvsformula_layout);

   // con7
    connect(wvsformula_buttonAdd,&QPushButton::clicked,[=](){
        QString formula = wvsformula_lineedit->text();
        if(formula.size())
        {
            if(vsformulaList.contains(formula))
            {
                wvsformula_msg->setText("已经存在。");
            }
            else{
                vsformulaList.push_back(formula);
                wvsformula_vsformulaList->addItem(formula);
                ui->comboBoxWorker1->addItem(formula);
                wvsformula_msg->setText("添加成功。");
                wvsformula_lineedit->clear();
            }
        }
    });
    connect(wvsformula_buttonDel,&QPushButton::clicked,[=](){
        QString formula = wvsformula_vsformulaList->currentText();
        if(formula.size())
        {
            if(vsformulaList.contains(formula))
            {
                for(int i=0;i<vsformulaList.size();i++)
                {
                    if(vsformulaList[i] == formula){
                        vsformulaList.remove(i);
                        break;
                    }
                }
                wvsformula_vsformulaList->removeItem(wvsformula_vsformulaList->currentIndex());
                wvsformula_msg->setText("移除成功。");
            }
            else{
                wvsformula_msg->setText("Error:5001 不存在该公式。");
            }
        }
    });
    connect(wvsformula_buttonclose, &QPushButton::clicked, wvsformula, &QDialog::close);

}


void MainWindow::showVSFormula(){
    wvsformula_msg->clear();
    wvsformula->move((this->width()-wvsformula->width())/2,(this->height()-wvsformula->height())/2);
    wvsformula->hide();
    wvsformula->show();
}

void MainWindow::readVSFormulaList(){
    QFile *file = new QFile("vsformulaList");
    bool ok = file->open(QIODevice::ReadOnly|QIODevice::Text);
    if(ok){
        DBG<<"read vsformula.";
        QTextStream in(file);
        QString n;

        while(!in.atEnd())
        {
            in>>n;
            if(!n.size())
                break;
            vsformulaList.push_back(n);
            wvsformula_vsformulaList->addItem(n);
        }
        file->close();
        delete file;
        file = NULL;
    }

}

void MainWindow::saveVSFormulaList(){
    QFile *file = new QFile("vsformulaList");
    bool ok = file->open(QIODevice::WriteOnly|QIODevice::Text);
    if(ok){
        DBG<<"write workerList.";
        QTextStream out(file);

            for(int i=0;i<vsformulaList.size();i++)
            {
                out<<vsformulaList[i]<<endl;
            }
        }
        file->close();
        delete file;
        file = NULL;
}
