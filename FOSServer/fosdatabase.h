#ifndef FOSDATABASE_H
#define FOSDATABASE_H
#include "fosconstant.h"
#include <QSqlDatabase>

class FOSDatabase : public QSqlDatabase
{
public:
    static FOSDatabase* getInstance();
    int insertUser(User &user);//插入
    int getMaxUserid();//取得最大id 用作新用户的ID
    struct User getUserInfo(int userid, QString pwd);
    int searchUser(int userid, QString pwd);
    QMap<int,QString> getNicknameAndID();
private:
    FOSDatabase();
    QSqlDatabase m_sqldb;
    static FOSDatabase* m_db;
};

#endif // FOSDATABASE_H
