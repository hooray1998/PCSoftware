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
    allData.curAction = AllData::Action_request_a;
    //if(allData.lastRequest==AllData::Action_receive_a)
        //allData.lastRequest = AllData::Action_die;
}

void Group::request_b(){
    QByteArray header = "06";
    machineB->WriteData(header);
    allData.curAction = AllData::Action_request_b;
    //if(allData.lastRequest==AllData::Action_receive_b)
        //allData.lastRequest = AllData::Action_die;
}

void Group::request_r(){
    QByteArray header = "07";
    machineB->WriteData(header);
    allData.curAction = AllData::Action_request_r;
    //if(allData.lastRequest==AllData::Action_receive_r)
        //allData.lastRequest = AllData::Action_die;
}

void Group::returnFinalResult(double final){
    QByteArray header = "05" + QString::asprintf("%08.2f",final).toLocal8Bit();
    machineA->WriteData(header);
    allData.curAction = AllData::Action_return;
}

void Group::returnThreeResult(){
	//TODO::返回三个值
    QByteArray header = "08" + QString::asprintf("%08.3f%08.3f%08.3f",allData.initValue_VS1_modeJingdu,allData.initValue_VS2_modeJingdu,allData.initValue_Yinliu_modeJingdu).toLocal8Bit();
    machineA->WriteData(header);
    allData.curAction = AllData::Action_return;
	emit SendLog(groupInfo.name, "回复给了A三个调整后的精度系数");

}
void Group::analyzeData_a(QByteArray data){

    if(allData.curAction!=AllData::Action_request_a){// request a
        emit SendLog(groupInfo.name, "receive a and leave it.");
        //allData.lastRequest = AllData::Action_die;
    }
    else{
        allData.push_a(data.toDouble());
        emit SendLog(groupInfo.name, "receive a.");
        allData.curAction = AllData::Action_receive_a;
        request_r();
    }
}

void Group::analyzeData_b(QByteArray data){
    if(allData.curAction!=AllData::Action_request_b){// request b
        emit SendLog(groupInfo.name, "receive b and leave it.");
        //allData.lastRequest = AllData::Action_die;
    }
    else{
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
    if(allData.curAction!=AllData::Action_request_r){// request r
        emit SendLog(groupInfo.name, "receive r and leave it.");
        //allData.lastRequest = AllData::Action_die;
    }
    else{
        allData.push_r(data.toDouble());
        emit SendLog(groupInfo.name, "receive r.");
        allData.curAction = AllData::Action_receive_r;
        //OK: 判断当前的模式和行为,不同的模式不同
        if(allData.curMode==AllData::Mode_VS1||allData.curMode==AllData::Mode_VS2){
			if(allData.VSCount==2)
			{
				returnFinalResult(allData.averageValue);
				emit SendLog(groupInfo.name, "将结果返回给了A设备。");
			}

            if(allData.vs1_ok){
                allData.vs1_ok = false;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "VS1调试ok");
            }
            else if(allData.vs2_ok){
                allData.vs2_ok = false;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "VS2调试ok");
            }
            else{
                request_b();
            }
        }
		else if(allData.curMode==AllData::Mode_Jingdu){
			//OK: step1完成后返回，step2完成后结束
			
			if(allData.jingdu_step == -1){
                allData.jingdu_step = 0;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "精度不稳，请检查");
			}
			else if(allData.jingdu_step == 2){
                allData.jingdu_step = 0;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "精度step2 is ok");
			}
			else if(allData.jingdu_step == 1 && allData.JingduCount == -1){
				allData.JingduCount++;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "精度step1 is ok");
				returnThreeResult();
				request_b();
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
    //allData.lastRequest = allData.curAction;
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
