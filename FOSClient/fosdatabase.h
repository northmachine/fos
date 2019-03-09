#ifndef FOSDATABASE_H
#define FOSDATABASE_H

#include<QSqlDatabase>
#include"fosconstant.h"
#include <QMap>
class FOSDatabase
{
public:
    static FOSDatabase* getDatabaseInstance(User &u);

    bool AddMyFriend(User &u, User &f);  // 插入
    bool DeleteMyFriend(User &u, User &f);  //删除
    FOSDatabase(User &u);
    bool searchMyFriend(User &u, User &f);//查找
private:
    static FOSDatabase *m_sqldatabase;   // 声明
    QSqlDatabase m_db;
};

#endif // FOSDATABASE_H
