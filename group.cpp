#include "group.h"
#define DBG qDebug()<<__FILE__<<__FUNCTION__<<"():"<<__LINE__

Group::Group()
{

}


void Group::analyzeDataA(QByteArray data)
{
    DBG<<"reveice data from A:"<<data;

}

void Group::analyzeDataB(QByteArray data)
{
    DBG<<"reveice data from A:"<<data;
}
