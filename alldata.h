#ifndef ALLDATA_H
#define ALLDATA_H

#include "expression.h"
#include <QVector>
#include <QDebug>
#include <QDate>


class AllData
{
public:
    AllData();
    enum Mode{
        Mode_VS1,
        Mode_VS2,
        Mode_Jingdu
    };
    enum Action{
        Action_die=0,
        Action_request_b,
        Action_receive_b,
        Action_request_a,
        Action_receive_a,
        Action_request_r,
        Action_receive_r,
        Action_return
    };

    //original
    void saveToExcel();
    bool returnData_FromVS(QVector<double> **a,
                    QVector<QString> **b);


    //update
    void push_a(double a);
    void push_b(double b);
    void push_r(double r);


    double cal_adjustValue();//Mode curMode. gen ju dang qian de mo shi ,
    double cal_expression(QString e);

    void complete();//let all vector length same.


    QString curWorker;
    double initValue_VS1_modeVS;
    double initValue_VS2_modeVS;
    //VS1
    QVector<double> original_VS1;
    QVector<double> b_VS1;
    QVector<double> r_VS1;
    QVector<double> differential_VS1;
    QVector<double> a_VS1;
    QVector<double> adjust_VS1;
    QVector<double> final_VS1;
    QVector<QString> status_VS1;
    QVector<QString> date_VS1;
    QVector<QString> worker_VS1;
    QString Expression_VS1;
    int length_VS1;

    //VS2
    QVector<double> original_VS2;
    QVector<double> b_VS2;
    QVector<double> r_VS2;
    QVector<double> differential_VS2;
    QVector<double> a_VS2;
    QVector<double> adjust_VS2;
    QVector<double> final_VS2;
    QVector<QString> status_VS2;
    QVector<QString> date_VS2;
    QVector<QString> worker_VS2;
    QString Expression_VS2;
    int length_VS2;


    double initValue_VS1_modeJingdu;
    double initValue_VS2_modeJingdu;
    double initValue_Yinliu_modeJingdu;

    Mode curMode;
    Action curAction;
    Action lastRequest;
    bool saved;

};

#endif // ALLDATA_H
