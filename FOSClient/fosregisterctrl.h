#ifndef FOSREGISTERCTRL_H
#define FOSREGISTERCTRL_H

#include <QObject>
#include "fostcpsocket.h"
#include "fosconstant.h"

class FOSRegisterCtrl : public QObject
{
    Q_OBJECT
public:
    explicit FOSRegisterCtrl(QObject *parent = 0);
    ~FOSRegisterCtrl();
    // 注册
    void registerAccount(const User &);
public slots:

private slots:
    // 请求注册
    void requestRegister();
    // 读取信息
    void readMessage();
private:

private:
    User m_user;
    quint16 m_blockSize;
    FOSTcpSocket *m_registerSocket;
};

#endif // FOSREGISTERCTRL_H
