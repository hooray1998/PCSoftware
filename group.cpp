#include "group.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

Group::Group()
{
    machineA = NULL;
    machineB = NULL;
    meishuile = false;
}

void Group::request_a(){
    QByteArray header = "04";
    machineA->WriteData(header);
    allData.curAction = AllData::Action_receive_a;
    if(allData.lastRequest==AllData::Action_receive_a)
        allData.lastRequest = AllData::Action_die;
}

void Group::request_b(){
    QByteArray header = "06";
    machineB->WriteData(header);
    allData.curAction = AllData::Action_receive_b;
    if(allData.lastRequest==AllData::Action_receive_b)
        allData.lastRequest = AllData::Action_die;
}

void Group::request_r(){
    QByteArray header = "07";
    machineB->WriteData(header);
    allData.curAction = AllData::Action_receive_r;
    if(allData.lastRequest==AllData::Action_receive_r)
        allData.lastRequest = AllData::Action_die;
}

void Group::returnFinalResult(double final){
    QByteArray header = "05" + QString::asprintf("%08.2f",final).toLocal8Bit();
    machineA->WriteData(header);
    allData.curAction = AllData::Action_return;
}
void Group::analyzeData_a(QByteArray data){
    DBG<<"reveice data from a:"<<data;

    if(allData.lastRequest==AllData::Action_receive_a){// request a
        emit SendLog(groupInfo.name, "receive a and leave it.");
        allData.lastRequest = AllData::Action_die;
    }
    else{
        //TODO: mode and action
        allData.push_a(data.toDouble());
        emit SendLog(groupInfo.name, "receive a.");
        allData.curAction = AllData::Action_receive_a;
        request_r();
    }
}

void Group::analyzeData_b(QByteArray data){
    DBG<<"reveice data from b:"<<data;
    if(allData.lastRequest==AllData::Action_receive_b){// request b
        emit SendLog(groupInfo.name, "receive b and leave it.");
        allData.lastRequest = AllData::Action_die;
    }
    else{
        //TODO:
        double number = data.left(8).toDouble();
        if(number<100){
            emit SendLog(groupInfo.name, "没水了。");
            meishuile = true;
        }
        else{
            allData.push_b(data.right(8).toDouble());
            emit SendLog(groupInfo.name, "receive b.");
            allData.curAction = AllData::Action_receive_b;
            request_a();
        }
    }
}

void Group::analyzeData_r(QByteArray data){
    DBG<<"reveice data from r:"<<data;
    if(allData.lastRequest==AllData::Action_receive_r){// request r
        emit SendLog(groupInfo.name, "receive r and leave it.");
        allData.lastRequest = AllData::Action_die;
    }
    else{
        //TODO:
        allData.push_r(data.toDouble());
        emit SendLog(groupInfo.name, "receive r.");
        allData.curAction = AllData::Action_receive_r;
        if(allData.VSCount==2)
        {
            returnFinalResult(allData.averageValue);
            emit SendLog(groupInfo.name, "将结果返回给了A设备。");
        }
        if(allData.curMode==AllData::Mode_VS1){
            if(allData.vs1_ok){
                allData.vs1_ok = false;
                emit SendLog(groupInfo.name, "VS1调试ok");
            }
            else{
                request_b();
            }
        }
        else if(allData.curMode==AllData::Mode_VS2){
            if(allData.vs2_ok){
                allData.vs2_ok = false;
                emit SendLog(groupInfo.name, "VS2调试ok");
            }
            else{
                request_b();
            }
        }
    }
}

void Group::tie(QString gname, MyThread *a, MyThread *b){
    groupInfo.name = gname;
    machineA = a;
    machineB = b;
    a->setGroup(this);
    b->setGroup(this);
    groupInfo.machineA_id = a->getMachineID();
    groupInfo.machineB_id = b->getMachineID();
}

void Group::untie(){
    if(machineA)
        machineA->setGroup(NULL);
    if(machineB)
        machineB->setGroup(NULL);
    machineA = NULL;
    machineB = NULL;
}

void Group::setMachineA(MyThread *a){
    machineA = a;
    a->setGroup(this);
}

void Group::setMachineB(MyThread *b){
    machineB = b;
    b->setGroup(this);
}

void Group::tie_byID(QString gname, QString a, QString b){
    groupInfo.name = gname;
    groupInfo.machineA_id = a;
    groupInfo.machineB_id = b;
}
QString Group::getMachineA_id(){
    return groupInfo.machineA_id;
}


QString Group::getMachineB_id(){
    return groupInfo.machineB_id;
}

void Group::stop(){
    allData.complete();
    allData.lastRequest = allData.curAction;
    allData.curAction = AllData::Action_die;
}

void Group::setWorker(QString w){
    allData.curWorker  = w;
}

void Group::logout(MyThread *machine){
    if(machine == machineA)
        machineA = NULL;
    else if(machine == machineB)
        machineB = NULL;
    else{
        DBG<<"Error : Don't have this machine in this group.";
    }
}

void Group::login(MyThread *machine){
    if(machine->getMachineID() == groupInfo.machineA_id)
    {
        machineA = machine;
        machine->setGroup(this);
    }
    else if(machine->getMachineID() == groupInfo.machineB_id)
    {
        machineB = machine;
        machine->setGroup(this);
    }
    else{
        DBG<<"Error : Don't have this machine in this group.";
    }
}

int Group::getOnlineStatus(){
    if(machineA)
        if(machineB)
            return 3;
        else
            return 2;
    else
        if(machineB)
            return 1;
        else
            return 0;
}
