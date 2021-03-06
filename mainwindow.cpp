#include "mainwindow.h"
#include "ui_mainwindow.h"
#define DBG qDebug()

const qint64 LOADBYTES = 4096; //4K

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DBG<<"start--------------------";
    ui->setupUi(this);
    this->setWindowTitle("上位机软件");
    setStyle(MainWindow::Style_Silvery);
    setWindowState(Qt::WindowMaximized);


    initUI();

    //初始化其他设置窗口
    initIpWidget();
    initTieGroupWidget();
    initUntieGroupWidget();
    initWorkerWidget();
    initVSFormulaWidget();
    initTcpServer();

    readConfig();
    //数据处理

    listenButtonClickSlot();//auto to connect
    updateListView();
    setCurWorker(ui->comboBoxWorker1->currentText());

    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonStop->setPalette(QColor(0, 0, 0));
    ui->pushButtonChangeJD->setEnabled(false);
    ui->pushButtonChangeJD->setPalette(QColor(0, 0, 0));
}

MainWindow::~MainWindow()
{
    delete wip;
    delete wtie;
    delete wuntie;

    delete ui;
}

//TODO::断开连接, 此时 设备的socket会收到disconnected信号
void MainWindow::closeEvent(QCloseEvent *event)
{
    int lived = -1;
    for(int i=0;i<allGroup.size();i++){
        if(allGroup[i]->allData.curAction != AllData::Action_die){
            lived = i;
            break;
        }
    }
    if(lived != -1){
        QMessageBox::warning(this, "警告", QString("设备组%1正在进行调试，不可退出").arg(allGroup.at(lived)->groupInfo.name));
        event->ignore();
        return;
    }


    for(int i=allMachine.size()-1;i>=0;i--)
    {
        allMachine.at(i)->disconnectAll();
    }
    saveConfig();
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
    ui->tableWidget_2->setColumnCount(19);
    ui->tabWidget->setCurrentIndex(0);

    headers<<"原值"<<"初值b"<<"末值r"<<"实际值r-b"<<"显示值a"<<"调整值"<<"最终值"<<"备注"<<"日期"<<"工作人员";
	//headers2<<"VS1"<<"VS2"<<"引流系数"<<"VS1（调）"<<"VS2（调）"<<"引流系数（调）"<<"0周期引流量初值"<<"0周期引流量末值"<<"0周期引流量实际值"<<"0周期引流量显示值"<<"0周期引流量精度"<<"1周期注入量初值"<<"1周期注入量末值"<<"1周期注入量实际值"<<"1周期注入量显示值"<<"1周期注入量精度"<<"设备状态及备注"<<"日期"<<"工作人员";
	headers2<<"VS1"<<"VS2"<<"引流系数"<<"VS1-调"<<"VS2-调"<<"引流系数-调"<<"引-初值"<<"引-末值"<<"引-实际值"<<"引-显示值"<<"引-精度"<<"注-初值"<<"注-末值"<<"注-实际值"<<"注-显示值"<<"注-精度"<<"备注"<<"日期"<<"工作人员";

    ui->tableWidgetvs1->setHorizontalHeaderLabels(headers);
    ui->tableWidgetvs2->setHorizontalHeaderLabels(headers);
    ui->tableWidget_2->setHorizontalHeaderLabels(headers2);
    //ui->tableWidgetvs1->setFixedWidth(ui->tabWidget->width());
    ui->tableWidgetvs1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetvs1->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableWidgetvs1->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);
    //ui->tableWidgetvs2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetvs2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetvs2->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Stretch);
    ui->tableWidgetvs2->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Stretch);


    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setColumnWidth(16,500);

    ui->listView_2->setUpdatesEnabled(true);

    ui->comboBoxWorker1->setFixedWidth(140);
    ui->lineEditJiNumber_mode1->setFixedWidth(300);


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

    red.setColor(QPalette::WindowText, Qt::red);
    black.setColor(QPalette::WindowText, Qt::black);
    green.setRgb(127,0,0,127);


    //connect(ui->tableWidgetvs1,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(scrollCurItem1(QTableWidgetItem*)));
    //connect(ui->tableWidgetvs2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(scrollCurItem2(QTableWidgetItem*)));
    //connect(ui->tableWidget_2,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(scrollCurItem3(QTableWidgetItem*)));
    connect(ui->tableWidgetvs1,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(changeComment1(QModelIndex)));
    connect(ui->tableWidgetvs2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(changeComment2(QModelIndex)));
    connect(ui->tableWidget_2,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(changeComment3(QModelIndex)));

    //pop action to the top

    QAction *action = new QAction("保存", this);
    ui->menuBar->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(saveTable2Excel()));
    ui->menuBar->addAction(action);

    action = new QAction("公式",this);
    ui->menuBar->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(showVSFormula()));
    ui->menuBar->addAction(action);

    action = new QAction("IP设置",this);
    ui->menuBar->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(showIpWidget()));
    ui->menuBar->addAction(action);

    action = new QAction("参数设置",this);
    ui->menuBar->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(showSetting()));
    ui->menuBar->addAction(action);

    action = new QAction("工作人员",this);
    ui->menuBar->addAction(action);
    connect(action,SIGNAL(triggered()),this,SLOT(manageWorker()));
    ui->menuBar->addAction(action);

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
    ui->menuBar->addMenu(themeMenu);
    connect(ui->pushButtonTie,SIGNAL(clicked()),this,SLOT(showTieGroupWidget()));
    connect(ui->pushButtonUntie,SIGNAL(clicked()),this,SLOT(showUntieGroupWidget()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(showTable(QModelIndex)));

    connect(ui->pushButtonStop,SIGNAL(clicked()), this,SLOT(stopDebug()));
    connect(ui->pushButtonStartVS1,SIGNAL(clicked(bool)),this,SLOT(startVS1()));
    connect(ui->pushButtonStartVS2,SIGNAL(clicked(bool)),this,SLOT(startVS2()));
    connect(ui->pushButtonStartJingdu,SIGNAL(clicked(bool)),this,SLOT(startJingdu()));

    connect(ui->comboBoxWorker1,SIGNAL(currentTextChanged(QString)),this,SLOT(setCurWorker(QString)));

	//TODO: 修改掉
    ui->doubleSpinBoxVS1Value_vsmode->setMaximum(9999);
    ui->doubleSpinBoxVS2Value_vsmode->setMaximum(9999);
    ui->doubleSpinBoxVS1Value_jdmode->setMaximum(99999);

    ui->doubleSpinBoxVS1Value_jdmode->setMinimum(-99999);
    ui->doubleSpinBoxVS2Value_jdmode->setMinimum(-99999);
    ui->doubleSpinBoxYinliuValue_jdmode->setMaximum(9999);
    ui->doubleSpinBoxYinliuValue_jdmode->setMinimum(-9999);

    ui->actionSave_as->setEnabled(false);

    connect(&debugInitValue, SIGNAL(updateDebugValue()), this, SLOT(updateAllDebugValue()));

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

// NOTE::如果有连接请求，就会触发slot_connect函数
void MainWindow::initTcpServer(){
    //通信
    pTcpServer = NULL;
    pTcpServer = new QTcpServer(this);
    //只要一建立连接成功，就会自动触发newConnection函数
    connect(pTcpServer,SIGNAL(newConnection()),this,SLOT(slot_connect()));
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

    if(IndexList.size()==0)
        return ;

    QMessageBox msgBox;
    msgBox.setText(QString("是否解绑这%1个设备组?").arg(IndexList.size()/2));
    msgBox.setInformativeText("是否确认解绑?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if(QMessageBox::Ok != msgBox.exec())
         return;

    QMap<int,int>rowMap;
    foreach (QModelIndex index, IndexList)
    {
        if(! index.isValid()) return;
        rowMap.insert(index.row(),0);
    }

    QMapIterator<int,int> Iterator(rowMap);
    Iterator.toBack();
    while(Iterator.hasPrevious())
    {
        Iterator.previous();
        int rowm=Iterator.key();
        if(allGroup.at(rowm)->groupInfo.name==curGroupName){
            curGroupName = ' ';
            ui->lineEditJiNumber_mode1->setText(curGroupName);

        }
        if(allGroup[rowm]->allData.curAction == AllData::Action_die){
            allGroup.at(rowm)->untie();
            allGroup.remove(rowm);
            allGroupLog.remove(rowm);
        }
        else
            QMessageBox::warning(this, "警告", QString("设备组%1正在进行调试，不可退出").arg(allGroup.at(rowm)->groupInfo.name));
    }
    updateListView();

}


void MainWindow::listenButtonClickSlot()
{
	if(pTcpServer->isListening()){
		if(QMessageBox::Ok != QMessageBox::warning(wip, "警告","当前设备正在监听，是否重启设备进行新的监听",QMessageBox::Ok|QMessageBox::Cancel)) return;
		else{
			saveConfig();
			qApp->exit(2);
			return;
		}
	}
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
        wip_button->setText("重新绑定");
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

    int index;
    bool AorB, ok = findMachineInGroup(header.left(6), index, AorB);

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
        if(ok){
            allGroup.at(index)->login(machine);
            int status = allGroup.at(index)->getOnlineStatus();
            QString newlog;
            switch (status) {
            case 3:
                newlog = "均已上线";
                break;
            case 2:
                newlog = "设备B未上线";
                break;
            case 1:
                newlog = "设备A未上线";
                break;
            case 0:
                newlog = "均未上线";
                break;
            default:
                newlog = "不会出现这句005";
                break;
            }
            allGroupLog[index] = newlog;
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
        if(ok){
            int status = allGroup.at(index)->getOnlineStatus();
            QString newlog;
            switch (status) {
            case 3:
                newlog = "均已上线";
                break;
            case 2:
                newlog = "设备B未上线";
                break;
            case 1:
                newlog = "设备A未上线";
                break;
            case 0:
                newlog = "均未上线";
                break;
            default:
                newlog = "不会出现这句005";
                break;
            }
            allGroupLog[index] = newlog;
            groupName = "Group:"+allGroup.at(index)->groupInfo.name;

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

    int index;
    if(findGroupInGroup(group, index)){
		allGroupLog[index].clear();
		allGroupLog[index] = msg;
		model->setItem(index,1,new QStandardItem(allGroupLog.at(index)));
		//model->item(index,1)->setText(msg);
        if(msg.contains("仍无回复")||msg.contains("没水了")||msg.contains("精度不稳")||msg.contains("禁止启动调试")){
			model->item(index, 1)->setBackground(QBrush(QColor(255, 0, 0))); 
		}
        if(msg.contains("OK")||msg.contains("ok")||msg.contains("重新连接")){
			model->item(index, 1)->setBackground(QBrush(QColor(0, 255, 0))); 
		}
    }

//if(msg.contains("VS1调试ok")//startVS2(index);
        if(msg.contains("精度参数更新")){
            updateTable();
        }
}

void MainWindow::updateListView(){
    groupStringList.clear();
    singleStringList.clear();
    model->clear();
    for(int i=0;i<allGroup.size();i++)
    {
        groupStringList.append(allGroup.at(i)->groupInfo.name);
        model->setItem(i,0,new QStandardItem(allGroup.at(i)->groupInfo.name));
        model->setItem(i,1,new QStandardItem(allGroupLog.at(i)));
    }

    for(int i=0;i<allMachine.size();i++)
    {
        if(!allMachine.at(i)->getGroup())
            singleStringList.append(QString("%1").arg(allMachine.at(i)->getMachineID()));
    }

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
    wip = new QWidget;
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
    connect(wip_button,SIGNAL(clicked()),this,SLOT(listenButtonClickSlot()));

}
void MainWindow::initTieGroupWidget(){
    wtie = new QWidget;
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
    connect(wtie_button,SIGNAL(clicked()),this,SLOT(tieTwoMachine()));
    connect(wtie_buttonclose,SIGNAL(clicked()),wtie,SLOT(close()));

}
void MainWindow::initUntieGroupWidget(){
    wuntie = new QWidget;
    wuntie->setWindowTitle("解绑设备组");
    wuntie->resize(300,200);

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
    connect(wuntie_button,SIGNAL(clicked()),this,SLOT(untieTwoMachine()));
    connect(wuntie_buttonclose,SIGNAL(clicked()),wuntie,SLOT(close()));
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
                    showLog("错误：设备A已经被其他设备绑定。groupName:"+allMachine.at(i)->getGroup()->groupInfo.name);
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
                    showLog("错误：设备B已经被其他设备绑定。groupName:"+allMachine.at(i)->getGroup()->groupInfo.name);
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
        setInitValue(allGroup.size()-1, true);
        allGroupLog.push_back(QString("new group"));
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
            allGroupLog.remove(i);
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
void MainWindow::startVS1(int index){

	//judge if both are online.
    if(3 == allGroup.at(index)->getOnlineStatus())
    {
        if(allGroup.at(index)->allData.curAction==AllData::Action_die)
        {
            if(QMessageBox::Ok != QMessageBox::information(this,"提示",QString("设备组名称%1\nVS1值：%2\n是否开始调试").arg(allGroup[index]->groupInfo.name).arg(ui->doubleSpinBoxVS1Value_vsmode->value()),QMessageBox::Ok | QMessageBox::Cancel)){
                return ;
            }
            allGroup.at(index)->allData.curMode = AllData::Mode_VS1;
            allGroup.at(index)->allData.Expression_VS1 = wvsformula_vsformulaList->currentText();
            allGroup.at(index)->allData.VSCount = 0;
            allGroup.at(index)->allData.vs1_ok = false;
            allGroup.at(index)->allData.curWorker = ui->comboBoxWorker1->currentText();

            allGroup.at(index)->allData.initValue_VS1_modeVS = ui->doubleSpinBoxVS1Value_vsmode->value();
            allGroup.at(index)->allData.initValue_VS2_modeVS = ui->doubleSpinBoxVS2Value_vsmode->value();


			allGroup.at(index)->returnFinalResult(1);
            //allGroup.at(index)->request_b();
        }
        else{
            showLog("该设备组正在进行其他动作，禁止启动调试。");
            QMessageBox::warning(this, "警告", "该设备组正在进行其他动作，禁止启动调试。");
        }
    }
    else{
        QMessageBox::warning(this, "警告", "该设备组未上线");
    }
}

void MainWindow::startVS1()
{
	if(!curGroupName.size()){
        QMessageBox::warning(this, "提示", "未选中设备组");
		return ;
	}
    int index;
    if(findGroupInGroup(curGroupName, index)){
		startVS1(index);
        updateTable();
    }
    else{
        QMessageBox::warning(this, "警告", "该设备组不存在");
        return ;
    }
}

void MainWindow::startVS2(int index){

    //judge if both are online.
    if(3 == allGroup.at(index)->getOnlineStatus())
    {
		if(allGroup.at(index)->allData.curAction==AllData::Action_die)
        {
            if(QMessageBox::Ok != QMessageBox::information(this,"提示",QString("设备组名称%1\nVS2值：%2\n是否开始调试").arg(allGroup[index]->groupInfo.name).arg(ui->doubleSpinBoxVS2Value_vsmode->value()),QMessageBox::Ok | QMessageBox::Cancel)){
                return ;
            }
            allGroup.at(index)->allData.curMode = AllData::Mode_VS2;
            allGroup.at(index)->allData.Expression_VS2 = wvsformula_vsformulaList->currentText();
            allGroup.at(index)->allData.VSCount = 0;
            allGroup.at(index)->allData.vs2_ok = false;
            allGroup.at(index)->allData.curWorker = ui->comboBoxWorker1->currentText();

            allGroup.at(index)->allData.initValue_VS2_modeVS = ui->doubleSpinBoxVS2Value_vsmode->value();

			allGroup.at(index)->returnFinalResult(2);
            //allGroup.at(index)->request_b();
        }
        else{
            showLog("该设备组正在进行其他动作，禁止启动调试。");
            qDebug()<<"cod:"<<allGroup.at(index)->allData.curAction;
            QMessageBox::warning(this, "警告", "该设备组正在进行其他动作，禁止启动调试。");
        }
    }
    else{
        QMessageBox::warning(this, "警告", "该设备组未上线");
    }
}

void MainWindow::startVS2(){

	if(!curGroupName.size()){
        QMessageBox::warning(this, "提示", "未选中设备组");
		return ;
	}
    int index;
	if(findGroupInGroup(curGroupName, index)){
		startVS2(index);
        updateTable();
    }
    else{
        QMessageBox::warning(this, "警告", "该设备组不存在");
        return ;
    }
}

void MainWindow::updateTable(){
    if(!curGroupName.size())
        return ;

    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonStop->setPalette(QColor(0, 0, 0));
    ui->pushButtonChangeJD->setEnabled(false);
    ui->pushButtonChangeJD->setPalette(QColor(0, 0, 0));

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
	Group *curGroup = allGroup.at(index);
    if(ok){

        updateDebugLabel(index);

		ui->doubleSpinBoxVS1Value_vsmode->setValue(curGroup->allData.initValue_VS1_modeVS);
		ui->doubleSpinBoxVS2Value_vsmode->setValue(curGroup->allData.initValue_VS2_modeVS);

		ui->doubleSpinBoxVS1Value_jdmode->setValue(curGroup->allData.initValue_VS1_modeJingdu);
		ui->doubleSpinBoxVS2Value_jdmode->setValue(curGroup->allData.initValue_VS2_modeJingdu);
		ui->doubleSpinBoxYinliuValue_jdmode->setValue(curGroup->allData.initValue_Yinliu_modeJingdu);

        //TODO: show all data
        QTableWidget *curTable;

        AllData::Mode m[3] = {AllData::Mode_VS1, AllData::Mode_VS2, AllData::Mode_Jingdu};
        for(int i=0;i<3;i++){
            AllData::Mode mode = m[i];
            if(!curGroup->allData.returnData_FromVS(mode, f_vectorArr, s_vectorArr))
                    return;

            int column = 7;
            if(mode==AllData::Mode_VS1)
                curTable = ui->tableWidgetvs1;
            else if(mode==AllData::Mode_VS2)
                curTable = ui->tableWidgetvs2;
            else if(mode==AllData::Mode_Jingdu){
                column = 16;
                curTable = ui->tableWidget_2;
            }

            curTable->clear();

            for(int i=0;i<column;i++)
                for(int j=0;j<f_vectorArr[i]->size();j++)
                {
                     curTable->setItem(j,i,new QTableWidgetItem(QString::number(f_vectorArr[i]->at(j))));
                }

            for(int i=0;i<3;i++)
                for(int j=0;j<s_vectorArr[i]->size();j++)
                {
                    curTable->setItem(j,i+column,new QTableWidgetItem(s_vectorArr[i]->at(j)));
                }

        }

        if(curGroup->allData.curAction != AllData::Action_die){
            ui->pushButtonStop->setEnabled(true);
            ui->pushButtonStop->setPalette(QColor(220, 0, 0));
        }
        else{
            ui->pushButtonChangeJD->setEnabled(true);
            ui->pushButtonChangeJD->setPalette(QColor(220, 0, 0));
        }

    }

    ui->tableWidgetvs1->setHorizontalHeaderLabels(headers);
    ui->tableWidgetvs2->setHorizontalHeaderLabels(headers);
    ui->tableWidget_2->setHorizontalHeaderLabels(headers2);

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

void MainWindow::scrollCurItem1(QTableWidgetItem *cur){
    ui->tableWidgetvs1->scrollToItem(cur);
}

void MainWindow::scrollCurItem2(QTableWidgetItem *cur){
    ui->tableWidgetvs2->scrollToItem(cur);
}

void MainWindow::scrollCurItem3(QTableWidgetItem *cur){
    ui->tableWidget_2->scroll(cur->row(),1);
}

void MainWindow::manageWorker(){
    wworker_msg->clear();
    wworker->move((this->width()-wworker->width())/2,(this->height()-wworker->height())/2);
    wworker->hide();
    wworker->show();
}

void MainWindow::initWorkerWidget(){

    wworker = new QWidget;
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

    connect(wworker_buttonAdd,SIGNAL(clicked()),this,SLOT(slot_addWorkers()));
    connect(wworker_buttonDel,SIGNAL(clicked()),this,SLOT(slot_delWorkers()));
    connect(wworker_buttonclose,SIGNAL(clicked()), wworker,SLOT(close()));
}

void MainWindow::setCurWorker(QString w){
    for(int i=0;i<allGroup.size();i++)
        allGroup[i]->allData.curWorker = w;
}

void MainWindow::stopDebug(){
    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){
		QString modeName = (allGroup[index]->allData.curMode == AllData::Mode_Jingdu)?"精度调试":(allGroup[index]->allData.curMode == AllData::Mode_VS1)?"VS1调试":"VS2调试";

        if(QMessageBox::Ok != QMessageBox::information(this,"提示",QString("当前设备组%1,正在进行%2，是否需要中止？中止后需要重新开始当前测试").arg(curGroupName).arg(modeName),QMessageBox::Ok | QMessageBox::Cancel)) return ;
		
        if(allGroup[index]->stop()){
            showLog("终止了当前调试");
        }
        else{
            showLog("终止了当前调试, 但设备A不在线");
        }
    }
    updateTable();

}

void MainWindow::initVSFormulaWidget(){
    wvsformula = new QWidget;
    wvsformula->resize(600,400);
    wvsformula->setWindowTitle("VS公式编辑");

    wvsformula_layout = new QGridLayout;
    wvsformula_rule = new QLabel("规则: c-原值 a-显示值 b-初值 r-末值");
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
    connect(wvsformula_buttonAdd,SIGNAL(clicked()),this,SLOT(slot_addFormula()));
    connect(wvsformula_buttonDel,SIGNAL(clicked()),this,SLOT(slot_delFormula()));
    connect(wvsformula_buttonclose,SIGNAL(clicked()), wvsformula,SLOT(close()));

}

void MainWindow::showVSFormula(){
    wvsformula_msg->clear();
    wvsformula->move((this->width()-wvsformula->width())/2,(this->height()-wvsformula->height())/2);
    wvsformula->hide();
    wvsformula->show();
}


void MainWindow::saveTable2Excel(){

    if(!curGroupName.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
	if(ok){

        if(allGroup.at(index)->allData.curAction!=AllData::Action_die){
            QMessageBox::warning(this, "调试正在进行中","当前调试还未结束,不可中途保存");
			return;
		}
		QXlsx::Format blueBackground;
		blueBackground.setPatternBackgroundColor(Qt::blue);
		QXlsx::Format greenBackground;
		greenBackground.setPatternBackgroundColor(Qt::green);

		AllData::Mode m[3] = {AllData::Mode_VS1, AllData::Mode_VS2, AllData::Mode_Jingdu};

		if(ui->tabWidget->currentIndex()==0){//vs

            QString filename = debugInitValue.savePath + "VS调试_" + curGroupName + "_" +QDate::currentDate().toString("yyyyMMdd") +'_'+ QTime::currentTime().toString("hhmmss") + ".xlsx";

			if(!filename.size())
				return ;
			filename = QFileDialog::getSaveFileName(this, tr("Save Excel"), filename, tr(".xlsx"));
			QXlsx::Document xlsx(filename);

			if(!allGroup.at(index)->allData.returnData_FromVS(m[0], f_vectorArr, s_vectorArr))
				return;

			for(int i=0;i<10;i++)
				xlsx.write(1, i+2,headers.at(i));

			for(int i=0;i<7;i++)
				for(int j=0;j<f_vectorArr[i]->size();j++)
				{
					xlsx.write(j+2,i+2,f_vectorArr[i]->at(j), greenBackground);
				}

			for(int i=0;i<3;i++)
				for(int j=0;j<s_vectorArr[i]->size();j++)
				{
					xlsx.write(j+2,i+9,s_vectorArr[i]->at(j), greenBackground);
				}
			//序号
			xlsx.write(1, 1, "序号", greenBackground);
            for(int i=0;i<f_vectorArr[1]->size();i++){
				xlsx.write(i+2, 1, i+1, greenBackground);
			}

			//VS2
			int lastLen = s_vectorArr[0]->size()+3;

			if(!allGroup.at(index)->allData.returnData_FromVS(m[1], f_vectorArr, s_vectorArr))
				return;
			for(int i=0;i<10;i++)
				xlsx.write(1+lastLen, i+2,headers.at(i));

			for(int i=0;i<7;i++)
				for(int j=0;j<f_vectorArr[i]->size();j++)
				{
					xlsx.write(j+2+lastLen,i+2,f_vectorArr[i]->at(j), blueBackground);
				}

			for(int i=0;i<3;i++)
				for(int j=0;j<s_vectorArr[i]->size();j++)
				{

					xlsx.write(j+2+lastLen,i+9,s_vectorArr[i]->at(j), blueBackground);
				}

			//序号
			xlsx.write(1+lastLen, 1, "序号", blueBackground);
            for(int i=0;i<f_vectorArr[1]->size();i++){
				xlsx.write(i+2+lastLen, 1, i+1, blueBackground);
			}

			xlsx.save();
		}
		else{//精度调试
            QString filename = debugInitValue.savePath + "精度调试_" + curGroupName + "_" +QDate::currentDate().toString("yyyyMMdd") +'_'+ QTime::currentTime().toString("hhmmss") + ".xlsx";

			if(!filename.size())
				return ;
			filename = QFileDialog::getSaveFileName(this, tr("Save Excel"), filename, tr(".xlsx"));
			QXlsx::Document xlsx(filename);


			if(!allGroup.at(index)->allData.returnData_FromVS(m[2], f_vectorArr, s_vectorArr))
				return;

			for(int i=0;i<19;i++)
				xlsx.write(1, i+2,headers2.at(i));

			for(int i=0;i<16;i++)
				for(int j=0;j<f_vectorArr[i]->size();j++)
				{
					xlsx.write(j+2,i+2,f_vectorArr[i]->at(j), greenBackground);
				}

			for(int i=0;i<3;i++)
				for(int j=0;j<s_vectorArr[i]->size();j++)
				{

					xlsx.write(j+2,i+18,s_vectorArr[i]->at(j), greenBackground);
				}
            //序号
            xlsx.write(1, 1, "序号", greenBackground);
            for(int i=0;i<f_vectorArr[1]->size();i++){
                xlsx.write(i+2, 1, i+1, greenBackground);
            }
            xlsx.save();
		}

	}

}


void MainWindow::saveAsTable2Excel(){

    if(!curGroupName.size())
        return ;

    //这是另存为
    QString filename = curGroupName + "_" +QDate::currentDate().toString("yyyyMMdd") +'_'+ QTime::currentTime().toString("hhmmss") + ".xlsx";
    filename = QFileDialog::getSaveFileName(this, tr("Save Excel"), "/home/"+filename, tr(".xlsx"));

    if(!filename.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){

        QXlsx::Format blueBackground;
        blueBackground.setPatternBackgroundColor(Qt::blue);
        QXlsx::Format greenBackground;
        greenBackground.setPatternBackgroundColor(Qt::green);

        AllData::Mode m[3] = {AllData::Mode_VS1, AllData::Mode_VS2, AllData::Mode_Jingdu};

        if(ui->tabWidget->currentIndex()==0){//vs
            QXlsx::Document xlsx("vs调试_"+filename);

            if(!allGroup.at(index)->allData.returnData_FromVS(m[0], f_vectorArr, s_vectorArr))
                    return;

            for(int i=0;i<10;i++)
               xlsx.write(1, i+1,headers.at(i));

            for(int i=0;i<7;i++)
                for(int j=0;j<f_vectorArr[i]->size();j++)
                {
                    xlsx.write(j+2,i+1,f_vectorArr[i]->at(j), greenBackground);
                }

            for(int i=0;i<3;i++)
                for(int j=0;j<s_vectorArr[i]->size();j++)
                {

                    xlsx.write(j+2,i+8,s_vectorArr[i]->at(j), greenBackground);
                }
            int lastLen = s_vectorArr[0]->size()+3;

            if(!allGroup.at(index)->allData.returnData_FromVS(m[1], f_vectorArr, s_vectorArr))
                    return;
            for(int i=0;i<10;i++)
               xlsx.write(1+lastLen, i+1,headers.at(i));

            for(int i=0;i<7;i++)
                for(int j=0;j<f_vectorArr[i]->size();j++)
                {
                    xlsx.write(j+2+lastLen,i+1,f_vectorArr[i]->at(j), blueBackground);
                }

            for(int i=0;i<3;i++)
                for(int j=0;j<s_vectorArr[i]->size();j++)
                {

                    xlsx.write(j+2+lastLen,i+8,s_vectorArr[i]->at(j), blueBackground);
                }

            xlsx.save();
        }
        else{//精度调试
            QXlsx::Document xlsx("精度调试_"+filename);


            if(!allGroup.at(index)->allData.returnData_FromVS(m[2], f_vectorArr, s_vectorArr))
                    return;

            for(int i=0;i<10;i++)
               xlsx.write(1, i+1,headers.at(i));

            for(int i=0;i<16;i++)
                for(int j=0;j<f_vectorArr[i]->size();j++)
                {
                    xlsx.write(j+2,i+1,f_vectorArr[i]->at(j), greenBackground);
                }

            for(int i=0;i<3;i++)
                for(int j=0;j<s_vectorArr[i]->size();j++)
                {

                    xlsx.write(j+2,i+17,s_vectorArr[i]->at(j), greenBackground);
                }
            xlsx.save();
        }

    }

}


void MainWindow::startJingdu(){
	if(!curGroupName.size()){
        QMessageBox::warning(this, "提示", "未选中设备组");
		return ;
	}
    int index;
    if(findGroupInGroup(curGroupName, index)){
		startJingdu(index);
        updateTable();
    }
    else{
        QMessageBox::warning(this, "警告", "该设备组不存在");
        return ;
    }
}

void MainWindow::startJingdu(int index){

	//judge if both are online.
	if(3 == allGroup.at(index)->getOnlineStatus())
	{
		//TODO:看三个值是否有效,当VS结束时赋予这些值
		if(allGroup.at(index)->allData.curAction==AllData::Action_die)
		{
            if(QMessageBox::Ok != QMessageBox::information(this,"提示",QString("设备组名称%1\nVS1值：%2\nVS2值：%3\n引流系数：%4\n是否开始调试").arg(allGroup[index]->groupInfo.name).arg(ui->doubleSpinBoxVS1Value_jdmode->value()).arg(ui->doubleSpinBoxVS2Value_jdmode->value()).arg(ui->doubleSpinBoxYinliuValue_jdmode->value()),QMessageBox::Ok | QMessageBox::Cancel)){
                return ;
            }

            allGroup.at(index)->allData.curMode = AllData::Mode_Jingdu;
			allGroup.at(index)->allData.initValue_VS1_modeJingdu = ui->doubleSpinBoxVS1Value_jdmode->value();
			allGroup.at(index)->allData.initValue_VS2_modeJingdu = ui->doubleSpinBoxVS2Value_jdmode->value();
			allGroup.at(index)->allData.initValue_Yinliu_modeJingdu = ui->doubleSpinBoxYinliuValue_jdmode->value();


			//TODO: 暂时没有用公式
			allGroup.at(index)->allData.jingdu_step = 0;
			allGroup.at(index)->allData.JingduCount = 0;
			allGroup.at(index)->allData.curWorker = ui->comboBoxWorker1->currentText();
			allGroup.at(index)->returnThreeResult(1);
		}
		else{
			showLog("该设备组正在进行其他动作，禁止启动调试。");
            QMessageBox::warning(this, "警告", "该设备组正在进行其他动作，禁止启动调试。");
		}
	}
    else{
        QMessageBox::warning(this, "警告", "该设备组未上线");
    }

}


void MainWindow::readConfig(){
	
    QFile loadFile("config.json");

	if(!loadFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "could't open projects json";
		return;
	}

	QByteArray allData = loadFile.readAll();
	loadFile.close();

	QJsonParseError json_error;
	QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

	if(json_error.error != QJsonParseError::NoError)
	{
		qDebug() << "json error!";
		return;
	}

	QJsonObject rootObj = jsonDoc.object();

    debugInitValue.savePath = rootObj.value("savePath").toString();

    if(rootObj.contains("debugValues")){
        QJsonObject debugValues = rootObj.value("debugValues").toObject();

        debugInitValue.minWater = debugValues.value("minWater").toDouble();
        debugInitValue.VS1_vsmode = debugValues.value("VS1_vsmode").toDouble();
        debugInitValue.VS2_vsmode = debugValues.value("VS2_vsmode").toDouble();
        debugInitValue.range_vsmode = debugValues.value("range_vsmode").toDouble();
        debugInitValue.flow_jingdu = debugValues.value("flow_jingdu").toDouble();
        debugInitValue.threshold1 = debugValues.value("Threshold1").toDouble();
        debugInitValue.threshold2 = debugValues.value("Threshold2").toDouble();
        debugInitValue.range1 = debugValues.value("range1").toDouble();
        debugInitValue.range2 = debugValues.value("range2").toDouble();
        debugInitValue.range3 = debugValues.value("range3").toDouble();
        debugInitValue.range4 = debugValues.value("range4").toDouble();

    }

    debugInitValue.readValueToWidget();
    QJsonObject tcpObj = rootObj.value("TCP").toObject();
    wip_ip->setText(tcpObj.value("IP").toString());
    wip_port->setValue(tcpObj.value("Port").toInt());

	QJsonArray workers = rootObj.value("workers").toArray();
	for(int i = 0; i< workers.size(); i++)
	{
		QString worker = workers.at(i).toString();
		workerList.push_back(worker);
		wworker_workerList->addItem(worker);
		ui->comboBoxWorker1->addItem(worker);
	}


	QJsonArray formulas = rootObj.value("vsFormula").toArray();
	for(int i = 0; i< formulas.size(); i++)
	{
		QString formula = formulas.at(i).toString();
		vsformulaList.push_back(formula);
		wvsformula_vsformulaList->addItem(formula);
	}


    bool init = false;
    if(QFile("allData.dat").exists())
        init = true;
    FILE *fp;
    fp = fopen("allData.dat","rb");

    QJsonArray groupShip = rootObj.value("groupShipList").toArray();
    for(int i=0;i<groupShip.size();i++){
		Group *g = new Group();
		QString n = groupShip.at(i).toObject().value("groupShipName").toString();
		QString a = groupShip.at(i).toObject().value("MachineA").toString();
		QString b = groupShip.at(i).toObject().value("MachineB").toString();
		g->tie_byID(n,a,b);

        allGroup.push_back(g);
        setInitValue(allGroup.size()-1, true);
        allGroupLog.push_back(QString("均未上线"));
        if(init) g->readData(fp);
        connect(g,SIGNAL(SendLog(QString,QString)),this,SLOT(showLog(QString,QString)));
    }
    fclose(fp);

	QJsonObject exitStatus = rootObj.value("exitStatus").toObject();
    ui->comboBoxWorker1->setCurrentIndex(exitStatus.value("curWorker").toInt());
    wvsformula_vsformulaList->setCurrentIndex(exitStatus.value("curVSFormula").toInt());

}

void MainWindow::saveConfig(){

    QFile file("config.json");
	if(!file.open(QIODevice::WriteOnly)){
		qDebug()<<"File open error.";
	}

	QJsonObject rootObject;

	QJsonObject tcpObj;
	tcpObj.insert("IP",wip_ip->text());
	tcpObj.insert("Port",wip_port->value());

	QJsonObject exitStatus;
	exitStatus.insert("curWorker",ui->comboBoxWorker1->currentIndex());
	exitStatus.insert("curVSFormula",wvsformula_vsformulaList->currentIndex());

	QJsonArray workers;
	for(int i=0;i<workerList.size();i++)
	{
		workers.append(workerList[i]);
	}

	QJsonArray vsFormula;
	for(int i=0;i<vsformulaList.size();i++)
	{
		vsFormula.append(vsformulaList[i]);
	}

    FILE *fp;
    fp = fopen("allData.dat","wb");
    QJsonArray groupShip;
	for(int i=0;i<allGroup.size();i++)
	{
		QJsonObject curGroup;
		curGroup.insert("groupShipName",allGroup[i]->groupInfo.name);
		curGroup.insert("MachineA",allGroup[i]->groupInfo.machineA_id);
		curGroup.insert("MachineB",allGroup[i]->groupInfo.machineB_id);
		groupShip.append(curGroup);

        allGroup[i]->saveData(fp);
	}
    fclose(fp);

	QJsonObject debugValues;

	debugValues.insert("minWater",debugInitValue.minWater);
	debugValues.insert("VS1_vsmode",debugInitValue.VS1_vsmode);
	debugValues.insert("VS2_vsmode",debugInitValue.VS2_vsmode);
	debugValues.insert("range_vsmode",debugInitValue.range_vsmode);
	debugValues.insert("flow_jingdu",debugInitValue.flow_jingdu);
	debugValues.insert("Threshold1",debugInitValue.threshold1);
	debugValues.insert("Threshold2",debugInitValue.threshold2);
	debugValues.insert("range1",debugInitValue.range1);
	debugValues.insert("range2",debugInitValue.range2);
	debugValues.insert("range3",debugInitValue.range3);
	debugValues.insert("range4",debugInitValue.range4);

	rootObject.insert("TCP", tcpObj);
	rootObject.insert("exitStatus", exitStatus);
	rootObject.insert("workers", workers);
	rootObject.insert("vsFormula", vsFormula);
	rootObject.insert("groupShipList", groupShip);
    rootObject.insert("savePath", debugInitValue.savePath);
    rootObject.insert("debugValues", debugValues);


	QJsonDocument jsonDoc;
	jsonDoc.setObject(rootObject);
	file.write(jsonDoc.toJson());
	file.close();
	qDebug()<<"配置保存成功";

}

void MainWindow::showSetting(){
    debugInitValue.show();
}

void MainWindow::changeComment1(QModelIndex modelIndex){

    if(!curGroupName.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){
        //judge if both are online.
        if(3 == allGroup.at(index)->getOnlineStatus()) {
            if(modelIndex.column()!=7) return;
            if(modelIndex.row()>=allGroup.at(index)->allData.status_VS1.size()) return;

            bool ok;
            QString text = QInputDialog::getText(this, tr("修改备注"),
                                                 tr("新的备注:"), QLineEdit::Normal,
                                                 modelIndex.data().toString(), &ok);
            if (ok && !text.isEmpty()){
                allGroup.at(index)->allData.status_VS1.replace(modelIndex.row(),text);
                updateTable();
            }
        }
    }
}

void MainWindow::changeComment2(QModelIndex modelIndex){

    if(!curGroupName.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){
        //judge if both are online.
        if(3 == allGroup.at(index)->getOnlineStatus()) {
            if(modelIndex.column()!=7) return;
            if(modelIndex.row()>=allGroup.at(index)->allData.status_VS2.size()) return;

            bool ok;
            QString text = QInputDialog::getText(this, tr("修改备注"),
                                                 tr("新的备注:"), QLineEdit::Normal,
                                                 modelIndex.data().toString(), &ok);
            if (ok && !text.isEmpty()){
                allGroup.at(index)->allData.status_VS2.replace(modelIndex.row(),text);
                updateTable();
            }
        }
    }
}
void MainWindow::changeComment3(QModelIndex modelIndex){

    if(!curGroupName.size())
        return ;

    int index;
    bool ok = findGroupInGroup(curGroupName, index);
    if(ok){
        //judge if both are online.
        if(3 == allGroup.at(index)->getOnlineStatus()) {
            if(modelIndex.column()!=16) return;
            if(modelIndex.row()>=allGroup.at(index)->allData.status_Jingdu.size()) return;

            bool ok;
            QString text = QInputDialog::getText(this, tr("修改备注"),
                                                 tr("新的备注:"), QLineEdit::Normal,
                                                 modelIndex.data().toString(), &ok);
            if (ok && !text.isEmpty()){
                allGroup.at(index)->allData.status_Jingdu.replace(modelIndex.row(),text);
                updateTable();
            }
        }
    }

}

void MainWindow::slot_connect(){
	//取出建立好的连接套接字
	pTcpSocket = pTcpServer->nextPendingConnection();
	MyThread *temp = new MyThread(pTcpSocket); //使用该socket初始化该线程
	allMachine.push_back(temp); //存储该线程
	temp->start();

	connect(temp, SIGNAL(SendLog(QString)), this, SLOT(showLog(QString)));
	connect(temp, SIGNAL(SendLog(MyThread*,QByteArray)), this, SLOT(showLog(MyThread*,QByteArray)));
}

void MainWindow::slot_addWorkers(){
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
}

void MainWindow::slot_delWorkers(){
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
}

void MainWindow::slot_addFormula(){
	QString formula = wvsformula_lineedit->text();
	if(formula.size())
	{
		if(vsformulaList.contains(formula))
		{
			wvsformula_msg->setText("已经存在。");
		}
		else{

            QString charList = "abcr+-*/().";
			bool ok = true;
			for(int i=0;i<formula.size();i++) {
				if(!formula[i].isDigit()&&!charList.contains(formula[i]))
				{
					wvsformula_msg->setText(QString("存在非法字符'%1'").arg(formula[i]));
					ok = false;
					break;
				}
			}
			int kuohao = 0;
			for(int i=0;i<formula.size();i++) {
				if(formula[i] == '(')
					kuohao++;
				else if(formula[i] == ')')
					kuohao--;
			}
			if(kuohao){
				wvsformula_msg->setText(QString("括号不匹配"));
				ok = false;
			}

			if(ok){
				vsformulaList.push_back(formula);
				wvsformula_vsformulaList->addItem(formula);
				wvsformula_msg->setText("添加成功。");
				wvsformula_lineedit->clear();
			}
		}
	}
}

void MainWindow::slot_delFormula(){
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
}

void MainWindow::setInitValue(int index, bool all){
	allGroup.at(index)->allData.minWater = debugInitValue.minWater;

	allGroup.at(index)->allData.initValue_VS1_modeVS = debugInitValue.VS1_vsmode;
	allGroup.at(index)->allData.initValue_VS2_modeVS = debugInitValue.VS2_vsmode;
    if(all){
        allGroup.at(index)->allData.range_vsmode = debugInitValue.range_vsmode;

        allGroup.at(index)->allData.initValue_Yinliu_modeJingdu = debugInitValue.flow_jingdu;
    }
	allGroup.at(index)->allData.Threshold1 = debugInitValue.threshold1;
	allGroup.at(index)->allData.Threshold2 = debugInitValue.threshold2;
	allGroup.at(index)->allData.range1 = debugInitValue.range1;
	allGroup.at(index)->allData.range2 = debugInitValue.range2;
	allGroup.at(index)->allData.range3 = debugInitValue.range3;
	allGroup.at(index)->allData.range4 = debugInitValue.range4;

}

void MainWindow::on_pushButtonChangeJD_clicked()
{
    int index;

    if(!curGroupName.size()) return ;
    if(!findGroupInGroup(curGroupName, index)) return ;
    if(3 != allGroup.at(index)->getOnlineStatus()) return ;

    allGroup.at(index)->tellToJD();
    showLog("请求该设备组切换为精度调试。");
    updateTable();
}

void MainWindow::on_toolButton_clicked()
{
    if(!curGroupName.size()) return ;
    int index;
    if(!findGroupInGroup(curGroupName, index)) return ;
    //judge if both are online.

    allGroup.at(index)->allData.initValue_VS1_modeVS = ui->doubleSpinBoxVS1Value_vsmode->value();

}

void MainWindow::on_toolButton_2_clicked()
{
    if(!curGroupName.size()) return ;
    int index;
    if(!findGroupInGroup(curGroupName, index)) return ;
    //judge if both are online.

    allGroup.at(index)->allData.initValue_VS2_modeVS = ui->doubleSpinBoxVS2Value_vsmode->value();

}

void MainWindow::on_toolButton_3_clicked()
{
    if(!curGroupName.size()) return ;
    int index;
    if(!findGroupInGroup(curGroupName, index)) return ;
    //judge if both are online.

    allGroup.at(index)->allData.initValue_VS1_modeJingdu = ui->doubleSpinBoxVS1Value_jdmode->value();
    allGroup.at(index)->allData.initValue_VS2_modeJingdu = ui->doubleSpinBoxVS2Value_jdmode->value();
    allGroup.at(index)->allData.initValue_Yinliu_modeJingdu = ui->doubleSpinBoxYinliuValue_jdmode->value();

}

void MainWindow::updateDebugLabel(int index){

    Group *curGroup = allGroup.at(index);

    QString debugs;

    debugs.append(QString("最低水量阈值：%1      VS最大稳定距离：%2\n").arg(QString::number(curGroup->allData.minWater),10).arg(QString::number(curGroup->allData.range_vsmode),10));
    debugs.append(QString("引流精度浮动范围：%1     注入精度浮动范围:%2\n").arg(QString::number(curGroup->allData.Threshold1),6).arg(QString::number(curGroup->allData.Threshold2),10));

    debugs.append(QString("引流精度下限：%1        引流精度上限：%2\n").arg(QString::number(curGroup->allData.range1),10).arg(QString::number(curGroup->allData.range2),10));
    debugs.append(QString("注入精度下限：%1        注入精度上限：%2").arg(QString::number(curGroup->allData.range3),10).arg(QString::number(curGroup->allData.range4),10));


    ui->debugLabel->setText(debugs);
}



void MainWindow::updateAllDebugValue(){
    for(int i=0;i<allGroup.size();i++){
        setInitValue(i, false);
    }
    showLog("参数发生了更新");
    updateTable();

}
