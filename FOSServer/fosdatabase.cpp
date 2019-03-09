#include "fosdatabase.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
FOSDatabase* FOSDatabase::m_db = NULL;

FOSDatabase::FOSDatabase()
{
    m_sqldb = QSqlDatabase::addDatabase("QSQLITE");
    m_sqldb.setDatabaseName("FOSDatabase.db");

    if (!m_sqldb.open()) {
        QMessageBox::warning(NULL, "error", m_sqldb.lastError().text());
    }

    QSqlQuery query;
    QString sql = "create table if not exists FOSUser(userid integer primary key, pwd vchar(100),nickname vchar(100));";
    query.exec(sql);
}
//插入
int FOSDatabase::insertUser(User &user) {
    QSqlQuery query;
    query.prepare("insert into FOSUser(userid, pwd, nickname)"
                  "values(:userid, :pwd, :nickname)");
    user.m_userid=getMaxUserid()+1;
    query.bindValue(":userid",user.m_userid);
    query.bindValue(":nickname", user.m_nickname);
    query.bindValue(":pwd", user.pwd);

    query.exec();
    return REGISTER_SUCCESS;
}

int FOSDatabase::getMaxUserid() {
    QSqlQuery query;
    query.exec("select ifnull(MAX(userid), 20000) from FOSUser;");
    if (!query.next()) {
        return 20001;
    }
    return query.value(0).toInt();


}

User FOSDatabase::getUserInfo(int userid, QString pwd) {
    QSqlQuery query;
    query.prepare("select userid, nickname from FOSUser where userid = :userid and pwd = :pwd;");
    query.bindValue(":userid", userid);
    query.bindValue(":pwd", pwd);
    query.exec();
    User ret;
    if (!query.next()) {
        ret.m_userid = -1;
        return ret;
    }

    QSqlRecord record = query.record();
    ret.m_userid = record.value("userid").toInt();
    ret.m_nickname = record.value("name").toString();

    return ret;
}


int FOSDatabase::searchUser(int userid, QString pwd)
{
    QSqlQuery query;
    query.prepare("select * from FOSUser where userid = :id;");
    query.bindValue(":id",userid);
    query.exec();
    //从 FOSUser 这张表里查找userid 为 函数里的参数的所有数据
    if(!query.next())
    {
        //如果没找到这样的userid
        return LOGIN_FAIL;
    }
    else{
        //找到了 将这条数据里的pwd 赋给ret
        if(query.value("pwd").toString() == pwd)
        {
            return LOGIN_SUCCESS;
        }
        else{
            return LOGIN_FAIL;
        }
    }

}

QMap<int, QString> FOSDatabase::getNicknameAndID()
{
    QSqlQuery query;
    QString sql="select userid,nickname from FOSUser;";
    query.exec(sql);
    QMap<int,QString> tmp;
    while(query.next()){
        tmp.insert(query.value(0).toInt(),query.value(1).toString());
        qDebug()<<query.value(0).toInt()<<query.value(1).toString();
    }
    return tmp;
}


FOSDatabase* FOSDatabase::getInstance() {
    if (m_db == NULL) {
        m_db = new FOSDatabase;
    }
    return m_db;
}


