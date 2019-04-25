#ifndef ALLDATA_H
#define ALLDATA_H

#include "expression.h"
#include <QVector>
#include <QDebug>


class AllData
{
public:
    AllData();

    void saveAlpha();
    void analyAlpha(QByteArray data);
    void saveBeta();
    void analyBeta(QByteArray data);

    void calData();
    void returnData(QVector<int> **a, QVector<int> **b, QVector<int> **r, int mode);
    void saveToExcel();


private:
    Expression express;
    QVector<int> dataA;
    QVector<int> dataB;
    QVector<int> result;
};

#endif // ALLDATA_H
