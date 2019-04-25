#include "alldata.h"

AllData::AllData()
{

}
void AllData::saveAlpha(){

}
void AllData::analyAlpha(QByteArray data){
    dataA.push_back(data.toInt());
    this->calData();
    qDebug()<<"alldata receive a"<<endl;
}
void AllData::saveBeta(){

}
void AllData::analyBeta(QByteArray data){
    dataB.push_back(data.toInt());
    this->calData();
    qDebug()<<"alldata receive b"<<endl;
}

void AllData::calData(){
    int min = (dataA.size()<dataB.size())?dataA.size():dataB.size();
    if(min>result.size())
        result.push_back(dataA.at(min-1)+dataB.at(min-1));
}
void AllData::returnData(QVector<int> **a, QVector<int> **b, QVector<int> **r, int mode){
    if(mode==0)
    {
        *a = &dataA;
        *b = &dataB;
        *r = &result;
    }
}

void AllData::saveToExcel(){

}
