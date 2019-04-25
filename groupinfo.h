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

    QString name;
    QString machineA_id;
    QString machineB_id;
    QDate loginTime;
};

#endif // GROUPINFO_H
