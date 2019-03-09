#include "fosdatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

FOSDatabase* FOSDatabase::m_sqldatabase = NULL;   // 静态成员的定义及初始化
FOSDatabase* FOSDatabase::getDatabaseInstance(User &u)
{
    if (NULL == m_sqldatabase)
    {
        m_sqldatabase = new FOSDatabase(u);
    }
    return m_sqldatabase;
}
// 返回单例对象

bool FOSDatabase::AddMyFriend(User &u,User &f)//设为星标
{

    int name = u.m_userid;
    int name2 = f.m_userid;
    QString sql = QString("insert into '%1' (Fid) values (%2);")
                  .arg(name)
                  .arg(name2);

    QSqlQuery query(m_db);
    int ret = query.exec(sql);

    if (!ret)
    {
        qDebug() << "insert:" << query.lastError().text();
        return false;
    }

    return true;

}

bool FOSDatabase::DeleteMyFriend(User &u,User &f)//取消星标
{

//    QSqlQuery query(m_db);
//    query.prepare("delete from  '%1' where Fid = ");
//    query.addBindValue(f.m_userid);
//    return true;
    int name = u.m_userid;
    int name2 = f.m_userid;
    QString sql = QString("delete from '%1' where Fid = '%2'")
                  .arg(name)
                  .arg(name2);
    QSqlQuery query(m_db);
    int ret = query.exec(sql);
    if(!ret)
    {
        qDebug()<<"delete:"<<query.lastError().text();
        return false;
    }
    return true;

}

FOSDatabase::FOSDatabase(User &u)
{

        // 1.加载数据库驱动
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("FOSDatabase.db");    // 设置数据库name

        // 2.打开数据库
        if (!m_db.open())
        {
            qDebug() << "打开数据库失败" << m_db.lastError().text();
            return ;
        }

        // 打开成功

        // 3.执行sql语句。
        QSqlQuery query(m_db);
//        QString sql = "create table if not exists FOSMyFriend (Mid integer ,Fid integer, Fname vchar);";
//        query.exec(sql);
//        query.prepare("create table if not exists ? (Mid integer);");
//        query.addBindValue(u.m_userid);
//        query.exec();
        int name = u.m_userid;
        QString sql = QString("create table if not exists '%1' (Fid integer);").arg(name);
        int ret = query.exec(sql);
        if (!ret)
        {
            qDebug() << "create:" << query.lastError().text();
        }
}

bool FOSDatabase::searchMyFriend(User &u,User &f)
{
    QSqlQuery query;
    int name = u.m_userid;
    int name2 = f.m_userid;
    QString sql = QString("select * from '%1';").arg(name);
    query.exec(sql);
//    if (!query.exec(sql));
//    //从  对应用户ID的这张表里查找Fid 为 函数里的参数的所有数据
//    {
//       qDebug()<<"search:"<<query.lastError().text();
//       return false;
//    }

    while(query.next())
    {
        int Fid = query.value(0).toInt();
        if(Fid == name2)
        {
            return true;
        }
    }
    return false;
}




