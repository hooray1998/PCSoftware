#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>

class Expression
{
public:
    Expression();
    void analyExpression();
    void addExpression();
    void saveExpression();
    void chooseExpression();

private:
    QString allExpression[];
    int expressionSize;
};

#endif // EXPRESSION_H
