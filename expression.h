#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>
#include <QVector>


//Store all expression string.
class Expression
{
public:
    Expression();

    //cal a*b-r+100 = ?
    //cal (a*(b-r)+(r-10)*3) = ?


    void findExpression(QString e);//get the index of cur mode's expression.
    void addExpression(QString e);
    void saveExpression();
    void chooseExpression();

private:
    QVector<QString> allExpression;
};

#endif // EXPRESSION_H
