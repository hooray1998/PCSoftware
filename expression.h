#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>
#include <QVector>

class Expression
{
public:
    Expression();
    void analyExpression();
    void addExpression();
    void saveExpression();
    void chooseExpression();

private:
    QVector<QString> allExpression;
};

#endif // EXPRESSION_H
