#include "fosregisterctrl.h"
#include <QMessageBox>

FOSRegisterCtrl::FOSRegisterCtrl(QObject *parent) : QObject(parent)
{
    m_registerSocket = new FOSTcpSocket(this);
    connect(m_registerSocket, SIGNAL(connected()), this, SLOT(requestRegister()));
    connect(m_registerSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

FOSRegisterCtrl::~FOSRegisterCtrl()
{

}

void FOSRegisterCtrl::registerAccount(const User &user)
{
    m_user = user;
    if (m_registerSocket->isConnected())
    {
        requestRegister();
    }
    else
    {
        m_blockSize = 0;
        m_registerSocket->requestConnect();
    }
}


void FOSRegisterCtrl::requestRegister()
{
//    qDebug() << "requestRegister";
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << int(REGISTER)  << m_user.m_nickname<<m_user.pwd;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_registerSocket->write(block);
//    qDebug() << "bytesAvailable:" << m_tcpSocket->bytesAvailable();
}

void FOSRegisterCtrl::readMessage()
{
    QDataStream in(m_registerSocket);
    in.setVersion(QDataStream::Qt_5_2);
    if (m_blockSize == 0)
    {
        if (m_registerSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    if (m_registerSocket->bytesAvailable() < m_blockSize)
        return;
    int type;

    in >> type;

    switch (type)
    {
    case REGISTER_SUCCESS:
        in >> m_user.m_nickname>>m_user.m_userid;
        QMessageBox::about(NULL, tr("注册结果"),
                           tr("%1, 你好！ 恭喜,注册成功. "
                              "您的ID是%2, 请牢牢记住.").arg(m_user.m_nickname).arg(m_user.m_userid));
        m_registerSocket->disconnectFromHost();
        break;
    case REGISTER_FAIL:
        QMessageBox::about(NULL, tr("注册结果"), tr("注册失败"));
        m_registerSocket->disconnectFromHost();
        break;
    default:
        break;
    }
    m_blockSize = 0;
}

