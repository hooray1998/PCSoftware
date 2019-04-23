#ifndef ALLDATA_H
#define ALLDATA_H

#include "expression.h"


class AllData
{
public:
    AllData();

    void saveAlpha();
    void analyAlpha();
    void saveBeta();
    void analyBeta();

    void calData(Expression cur);
    void returnData(int mode);
    void saveToExcel();


private:
    int curExpression;
};

#endif // ALLDATA_H
