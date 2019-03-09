#ifndef FOSLOGINCTRL_H
#define FOSLOGINCTRL_H

#include <QObject>
#include "fostcpsocket.h"
#include "fosconstant.h"

class FOSLoginCtrl : public QObject
{
    Q_OBJECT
public:
    explicit FOSLoginCtrl(QObject *parent = 0);
    ~FOSLoginCtrl();
    // 发送登录信息


signals:
    void getLoginMessgae(QString,bool isLogin = false,const User * me = 0);

public slots:
    // 改变登录信息
    void changeLoginMessage(const QString &mes, bool isLogin);
    void login();


private slots:

    // 发送登录请求
    void requestLogin();

    // 读取信息
    void readMessage();

private:

public:
    int m_kind;
    quint16 m_blockSize;
    FOSTcpSocket *m_loginSocket;
    User m_userInfo;
//    UserInformation m_myself;
};

#endif // FOSLOGINCTRL_H
