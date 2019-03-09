#ifndef FOSTCPSERVER_H
#define FOSTCPSERVER_H
#include "fosconstant.h"
#include <QTcpServer>
#include <QMessageBox>
#include "fostcpsocket.h"
#include "fosdatabase.h"

class FOSTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit FOSTcpServer(QObject *parent=0);
    void talkRequest(const SaveInformation &save);
private slots:
    void incomingConnection(qintptr handle);
    void sendMessage(SaveInformation &save);
    void sendUserList(SaveInformation &save);
    void clientDisconnected(const int &id);
//    void changeStatu(User &);
    void sendUpdateUserListToAllUser();
private:
    SaveInformation m_save;
    QMap<int,FOSTcpSocket *> m_userMap;
    FOSDatabase *m_fosDatabase;// 操作数据库
    QMap<int,QHostAddress> m_ipList;
    QMap<int,QString> m_nnList;
};

#endif // FOSTCPSERVER_H
