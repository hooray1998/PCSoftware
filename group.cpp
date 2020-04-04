#include "group.h"
#include "string.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

Group::Group()
{
    machineA = NULL;
    machineB = NULL;
}

void Group::tellToJD(){
    allData.initValue_VS1_modeJingdu = allData.final_VS1.back();
    allData.initValue_VS2_modeJingdu = allData.final_VS2.back();
    QByteArray header = "11";
    machineA->WriteData(header);
}

void Group::request_a(){
    QByteArray header;
    if(allData.curMode==AllData::Mode_VS1)
        header = "14";
    else if(allData.curMode==AllData::Mode_VS2)
        header = "24";
    else if(allData.curMode==AllData::Mode_Jingdu)
        header = "04";
    machineA->WriteData(header);
    allData.curAction = AllData::Action_request_a;
}

void Group::request_buchong(){
    QByteArray header;
    if(allData.curMode==AllData::Mode_VS1)
        header = "35";
    else if(allData.curMode==AllData::Mode_VS2)
        header = "45";
    else if(allData.curMode==AllData::Mode_Jingdu)
        header = "16";
    machineA->WriteData(header);
    allData.curAction = AllData::Action_request_buchong;
}

void Group::receive_buchong(){
    if(allData.curAction!=AllData::Action_request_buchong){
        if(allData.jingdu_step != -2){
            emit SendLog(groupInfo.name, "收到无用的补充回应");
        }
    }
    else{
        if(allData.curMode!=AllData::Mode_Jingdu){
            emit SendLog(groupInfo.name, "收到补充回应");
        }
        else{
            emit SendLog(groupInfo.name, "收到新一轮通知确认");
        }
        allData.curAction = AllData::Action_receive_buchong;
        request_b();
    }
}

void Group::request_b(){
    QByteArray header = "06";
    machineB->WriteData(header);
    allData.curAction = AllData::Action_request_b;
}

void Group::request_r(){
    QByteArray header = "07";
    machineB->WriteData(header);
    allData.curAction = AllData::Action_request_r;
    //if(allData.lastRequest==AllData::Action_receive_r)
        //allData.lastRequest = AllData::Action_die;
}

void Group::returnFinalResult(int mode){//mode 0:调整后返回，1：vs1初始返回，2：vs2初始返回
    char msg[12];
	if(mode == 0){
		sprintf(msg,"%08.2f",allData.averageValue);
		QByteArray header = "05" + QString(msg).toLocal8Bit();
		machineA->WriteData(header);
		allData.curAction = AllData::Action_return;
	}
	else if(mode == 1){
        sprintf(msg,"%08.2f",allData.initValue_VS1_modeVS);
		QByteArray header = "15" + QString(msg).toLocal8Bit();
		machineA->WriteData(header);
        allData.curAction = AllData::Action_request_answer;
    }
	else if(mode == 2){
        sprintf(msg,"%08.2f",allData.initValue_VS2_modeVS);
        QByteArray header = "25" + QString(msg).toLocal8Bit();
		machineA->WriteData(header);
        allData.curAction = AllData::Action_request_answer;
    }
}

void Group::returnThreeResult(int mode){
	//TODO::返回三个值
    char msg[25];
    sprintf(msg,"%08.3f%08.3f%08.3f",allData.initValue_VS1_modeJingdu,allData.initValue_VS2_modeJingdu,allData.initValue_Yinliu_modeJingdu);
	if(mode == 0){
		QByteArray header = "08" + QString(msg).toLocal8Bit();
		machineA->WriteData(header);
        allData.curAction = AllData::Action_request_buchong;
		emit SendLog(groupInfo.name, "回复给了A三个调整后的精度系数");
	}
    else if(mode == 1){
        QByteArray header = "65" + QString(msg).toLocal8Bit();
        machineA->WriteData(header);
        allData.curAction = AllData::Action_request_answer;
        emit SendLog(groupInfo.name, "回复给了A三个精度系数");
    }
    else if(mode == 2){
        QByteArray header = "18";
		machineA->WriteData(header);
        allData.curAction = AllData::Action_request_answer;
        emit SendLog(groupInfo.name, "精度调试OK，返回通知");
        end();
	}

}

void Group::analyzeData_answer(){
    if(allData.curAction!=AllData::Action_request_answer){
        emit SendLog(groupInfo.name, "收到无用的确认回应");
    }
    else{
        emit SendLog(groupInfo.name, "收到确认回应");
        allData.curAction = AllData::Action_receive_answer;

        if(allData.curMode==AllData::Mode_Jingdu){
            request_b();
        }
        else{
            request_buchong();
        }
    }
}
void Group::analyzeData_a(QByteArray data){

    if(allData.curAction!=AllData::Action_request_a){// request a
        emit SendLog(groupInfo.name, "收到无用的实际值a");
        //allData.lastRequest = AllData::Action_die;
    }
    else{
        allData.push_a(data.toDouble());
        emit SendLog(groupInfo.name, "收到实际值a");
        allData.curAction = AllData::Action_receive_a;
        request_r();
    }
}

void Group::analyzeData_b(QByteArray data){
    if(allData.curAction!=AllData::Action_request_b){// request b
        emit SendLog(groupInfo.name, "收到无用的初值b");
        //allData.lastRequest = AllData::Action_die;
    }
    else{
        double number = data.left(8).toDouble();
        if(allData.curMode!=AllData::Mode_Jingdu && number<allData.minWater){
            emit SendLog(groupInfo.name, QString("没水了（当前水量%1小于阈值%2）").arg(number).arg(allData.minWater));
			allData.complete();
			allData.curAction = AllData::Action_die;
        }
        else{
            allData.push_b(data.right(8).toDouble());
            emit SendLog(groupInfo.name, "收到初值b");
            allData.curAction = AllData::Action_receive_b;
            request_a();
        }
    }
}

void Group::analyzeData_r(QByteArray data){
    if(allData.curAction!=AllData::Action_request_r){// request r
        emit SendLog(groupInfo.name, "收到无用的末值r");
        //allData.lastRequest = AllData::Action_die;
    }
    else{
        allData.push_r(data.toDouble());
        emit SendLog(groupInfo.name, "收到末值r");
		if(allData.curAction==AllData::Action_die) return;
        allData.curAction = AllData::Action_receive_r;
        //OK: 判断当前的模式和行为,不同的模式不同
        if(allData.curMode==AllData::Mode_VS1){
            if(allData.vs1_ok){
                allData.vs1_ok = false;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "VS1调试OK");
                end();
            }
            else if(allData.VSCount==3){
                allData.vs1_ok = false;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "VS1不稳");
			}
            else{
                request_buchong();
            }
        }
        else if(allData.curMode==AllData::Mode_VS2){
            if(allData.vs2_ok){
                allData.vs2_ok = false;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "VS2调试OK");
                end();
            }
            else if(allData.VSCount==3){
                allData.vs2_ok = false;
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "VS2不稳");
            }
            else{
                request_buchong();
            }
        }
        else if(allData.curMode==AllData::Mode_Jingdu){

            bool return1= false;
			if(allData.updateFlag){
				allData.updateFlag = false;
                return1 = true;
                if(allData.jingdu_step != -2){
                    returnThreeResult(0);
                    emit SendLog(groupInfo.name, "精度参数更新");
                }
                else
                    returnThreeResult(2);
            }
			
			if(allData.jingdu_step == -1){
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "精度不稳，请检查");
			}
			else if(allData.jingdu_step == -2){
                allData.curAction=AllData::Action_die;
                emit SendLog(groupInfo.name, "精度调试成功");
			}
            else{
                if(!return1)
                    request_buchong();
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

bool Group::stop(){
    end();
    allData.complete();
    if(getOnlineStatus()>1){
        machineA->WriteData("91");
        allData.curAction = AllData::Action_die;
        return true;
    }
    else{
        allData.curAction = AllData::Action_die;
        return false;
    }
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

void Group::login(MyThread *machine){ //绑定设备a或b的线程
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

int Group::getOnlineStatus(){ //查看当前设备组在线的情况
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



void Group::readData(FILE *fp){
    qDebug()<<"cur Name:"<<groupInfo.name;
    allData.readData(fp);
}
void Group::saveData(FILE *fp){
    qDebug()<<"cur Name:"<<groupInfo.name;
    allData.saveData(fp);
}
void Group::end(){
    machineA->end();
    machineB->end();
}


void Group::sendLog(QString msg){
    emit SendLog(groupInfo.name, msg);
}
