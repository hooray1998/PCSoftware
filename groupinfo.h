#ifndef GROUPINFO_H
#define GROUPINFO_H

#include <QString>
#include <QDate>

class GroupInfo
{
public:
    GroupInfo();
    int getOnlineTime();

    unsigned short status;
    int id;
    QString name;
    QDate loginTime;
};

#endif // GROUPINFO_H
