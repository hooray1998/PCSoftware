#include "alldata.h"

#define DBG qDebug()<<__FILE__<<" "<<__FUNCTION__<<"()"<<__LINE__

AllData::AllData()
{
    length_VS2 = 0;
    length_VS2 = 0;

    initValue_VS1_modeVS = 1000;
    initValue_VS2_modeVS = 1000;

    initValue_VS1_modeJingdu = 1000;
    initValue_VS2_modeJingdu = 1000;
    initValue_Yinliu_modeJingdu = 1000;

    curWorker = "hoorayitt.";
}

bool AllData::returnData_FromVS(QVector<double> **a,
                                QVector<QString> **b){

    if(curMode==Mode_VS1){
        a[0] = &original_VS1;
        a[1] = &b_VS1;
        a[2] = &r_VS1;
        a[3] = &differential_VS1;
        a[4] = &a_VS1;
        a[5] = &adjust_VS1;
        a[6] = &final_VS1;
        b[0] = &status_VS1;
        b[1] = &date_VS1;
        b[2] = &worker_VS1;
    }
    else if(curMode==Mode_VS2){
        a[0] = &original_VS2;
        a[2] = &b_VS2;
        a[2] = &r_VS2;
        a[3] = &differential_VS2;
        a[4] = &a_VS2;
        a[5] = &adjust_VS2;
        a[6] = &final_VS2;
        b[0] = &status_VS2;
        b[2] = &date_VS2;
        b[2] = &worker_VS2;
    }
    else if(curMode==Mode_Jingdu){
        a[0] = &original_VS1;
        a[1] = &b_VS1;
        a[2] = &r_VS1;
        a[3] = &differential_VS1;
        a[4] = &a_VS1;
        a[5] = &adjust_VS1;
        a[6] = &final_VS1;
        b[0] = &status_VS1;
        b[1] = &date_VS1;
        b[2] = &worker_VS1;
    }
    else{
        return false;
    }

    return true;
}

void AllData::saveToExcel(){

}

double AllData::cal_adjustValue(){
    QString expression;
    if(curMode==Mode_VS1)
    {
        expression = Expression_VS1;
        DBG<<length_VS1<<" "<<expression;
        double a = a_VS1[length_VS1-1];
        double b = b_VS1[length_VS1-1];
        double r = r_VS1[length_VS1-1];


        expression = expression.split("a").join(QString::number(a));
        expression = expression.split("b").join(QString::number(b));
        expression = expression.split("r").join(QString::number(r));

        return cal_expression(expression);

    }
    else if(curMode==Mode_VS2)
    {
        expression = Expression_VS2;
        double a = a_VS2[length_VS2-1];
        double b = b_VS2[length_VS2-1];
        double r = r_VS2[length_VS2-1];


        expression = expression.split("a").join(QString::number(a));
        expression = expression.split("b").join(QString::number(b));
        expression = expression.split("r").join(QString::number(r));

        return cal_expression(expression);

    }
    else if(curMode==Mode_Jingdu)
        expression = Expression_VS2;



}

double AllData::cal_expression(QString e){//use stack to calculate the expression
    DBG<<"expression final is :"<<e;
    e = "666";
    return 666;
}

void AllData::push_a(double a){
    if(curMode==Mode_VS1)
    {
        a_VS1.push_back(a);
        if(length_VS1 < a_VS1.size())
            length_VS1 = a_VS1.size();
    }
    else if(curMode==Mode_VS2)
    {
        a_VS2.push_back(a);
        if(length_VS2 < a_VS2.size())
            length_VS2 = a_VS2.size();
    }
}

void AllData::push_b(double b){
    if(curMode==Mode_VS1)
    {
        b_VS1.push_back(b);
        if(length_VS1 < b_VS1.size())
            length_VS1 = b_VS1.size();
        original_VS1.push_back(initValue_VS1_modeVS);
        worker_VS1.push_back(curWorker);
        date_VS1.push_back(QDate::currentDate().toString("yy-MM-dd"));
    }
    else if(curMode==Mode_VS2)
    {
        b_VS2.push_back(b);
        if(length_VS2 < b_VS2.size())
            length_VS2 = b_VS2.size();
        original_VS1.push_back(initValue_VS2_modeVS);
        worker_VS2.push_back(curWorker);
        date_VS2.push_back(QDate::currentDate().toString("yy-MM-dd"));
    }

}


void AllData::push_r(double r){
    if(curMode==Mode_VS1)
    {
        r_VS1.push_back(r);
        if(length_VS1 < r_VS1.size())
            length_VS1 = r_VS1.size();
        differential_VS1.push_back(r-b_VS1[length_VS1-1]);
        adjust_VS1.push_back(cal_adjustValue());
        final_VS1.push_back(cal_adjustValue());
    }
    else if(curMode==Mode_VS2)
    {
        r_VS2.push_back(r);
        if(length_VS2 < r_VS2.size())
            length_VS2 = r_VS2.size();
        differential_VS2.push_back(r-b_VS2[length_VS2-1]);
        adjust_VS2.push_back(cal_adjustValue());
        final_VS2.push_back(cal_adjustValue());
    }

}

void AllData::complete(){//let the length equal
    //status is "STOP"
    if(curMode==Mode_VS1){
        if(a_VS1.size()<length_VS1){
            a_VS1.push_back(0);
        }
        if(b_VS1.size()<length_VS1){
            b_VS1.push_back(0);
        }
        if(r_VS1.size()<length_VS1){
            r_VS1.push_back(0);
        }
        if(original_VS1.size()<length_VS1){
            original_VS1.push_back(0);
        }
        if(differential_VS1.size()<length_VS1){
            differential_VS1.push_back(0);
        }
        if(adjust_VS1.size()<length_VS1){
            adjust_VS1.push_back(0);
        }
        if(final_VS1.size()<length_VS1){
            final_VS1.push_back(0);
        }
        if(status_VS1.size()<length_VS1){
            status_VS1.push_back("STOP");
        }
    }
    else if(curMode==Mode_VS2){
        if(a_VS2.size()<length_VS2){
            a_VS2.push_back(0);
        }
        if(b_VS2.size()<length_VS2){
            b_VS2.push_back(0);
        }
        if(r_VS2.size()<length_VS2){
            r_VS2.push_back(0);
        }
        if(original_VS2.size()<length_VS2){
            original_VS2.push_back(0);
        }
        if(differential_VS2.size()<length_VS2){
            differential_VS2.push_back(0);
        }
        if(adjust_VS2.size()<length_VS2){
            adjust_VS2.push_back(0);
        }
        if(final_VS2.size()<length_VS2){
            final_VS2.push_back(0);
        }
        if(status_VS2.size()<length_VS2){
            status_VS2.push_back("STOP");
        }
    }

}
