#ifndef ALLDATA_H
#define ALLDATA_H

#include "expression.h"
#include <QVector>
#include <QDebug>
#include <QDate>
#include <QMessageBox>


class AllData
{
public:
    AllData();
    enum Mode{
        Mode_VS1 = 1,
        Mode_VS2 = 2,
        Mode_Jingdu = 3
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

	double myAbs(double b);

    //original
    bool returnData_FromVS(Mode mode, QVector<double> **a,
                    QVector<QString> **b);


    //update
    void push_a(double a);
    void push_b(double b);
    void push_r(double r);


    double cal_adjustValue();//Mode curMode. gen ju dang qian de mo shi ,
    double cal_expression(QString e);

	void cal_finalValues_JingduMode();
    void complete();//let all vector length same.

	void updateValue(int distance);
	bool StableJudge(int distance);
	bool RangeJudge(int distance);


    QString curWorker;

    double initValue_VS1_modeVS;
    double initValue_VS2_modeVS;

	double minWater;
	double range_vsmode;



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
    bool vs1_ok;

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
    bool vs2_ok;

	double averageValue;


	//===========================================================
	//  精度调试
	//===========================================================
	double Threshold1;
	double Threshold2;

	double range1;
	double range2;
	double range3;
	double range4;

    double initValue_VS1_modeJingdu;
    double initValue_VS2_modeJingdu;
    double initValue_Yinliu_modeJingdu;


    //原值
    QVector<double> originalVS1_Jingdu;
    QVector<double> originalVS2_Jingdu;
    QVector<double> originalFlow_Jingdu;
	//调整值
    QVector<double> adjustVS1_Jingdu;
    QVector<double> adjustVS2_Jingdu;
    QVector<double> adjustFlow_Jingdu;
	//0周期
    QVector<double> b0_Jingdu;
    QVector<double> r0_Jingdu;
    QVector<double> differential0_Jingdu;
    QVector<double> a0_Jingdu;
    QVector<double> accuracy0_Jingdu;
	//1周期
    QVector<double> b1_Jingdu;
    QVector<double> r1_Jingdu;
    QVector<double> differential1_Jingdu;
    QVector<double> a1_Jingdu;
    QVector<double> accuracy1_Jingdu;
	//备注
    QVector<QString> status_Jingdu;
    QVector<QString> date_Jingdu;
    QVector<QString> worker_Jingdu;

    QString Expression_Jingdu;
    int length_Jingdu;

	int jingdu_step;
    int VSCount;//记录第几次，三次一检查。
	int JingduCount;

	bool updateFlag;

    Mode curMode;
    Action curAction;
    //Action lastRequest;
    bool saved;

};

#endif // ALLDATA_H
