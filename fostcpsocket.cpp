#include "fostcpsocket.h"
#include "fosconstant.h"
#include <QDataStream>

FOSTcpSocket::FOSTcpSocket(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteSocket()));
    m_blockSize = 0;
    m_save.m_socket = this;
}

void FOSTcpSocket::receiveMessage()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_5_2);
    if (m_blockSize == 0)
    {
        if (bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    if (bytesAvailable() < m_blockSize)
        return;

    in >> m_save.m_request;
    switch(m_save.m_request){
    case LOGIN:
        in>>m_save.m_user.m_userid;
        in>>m_save.m_user.pwd;
        break;
    case REGISTER:
        in>>m_save.m_user.m_nickname;
        in>>m_save.m_user.pwd;
        break;
    case TALK:
        in>>m_save.m_user.m_userid;
        in>>m_save.m_receiverID;
        in>>m_save.m_text;
        break;
    case USERLIST:
        in>>m_save.m_user.m_userid;
//        in>>m_save.m_request;
        break;
    case BIND:
        in>>m_save.m_user.m_userid;
        break;
    default:
        break;
    }
//    in>>m_save.m_ip;

    emit sendSignal(m_save);
    m_blockSize =0;

    if (bytesAvailable() > 0)
    {
        receiveMessage();
    }
}

void FOSTcpSocket::sendMessage(SaveInformation &temp)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << (int)temp.m_replyKind;
    switch(temp.m_replyKind){
    case LOGIN_SUCCESS:
        out<<temp.m_user.m_nickname<<temp.m_user.m_userid;
        break;
    case LOGIN_FAIL:
        break;
    case HAVE_LOGINED:
        break;
    case REGISTER_SUCCESS:
        out<<temp.m_user.m_nickname<<temp.m_user.m_userid;
        break;
    case REGISTER_FAIL:
        break;
    case TALK_OK:
        out<<temp.m_user.m_userid/*<<temp.m_user.m_nickname*/<<temp.m_text;
        break;
    case GET_USER_LIST:
        foreach(int tmpid,temp.m_userlist){
            out<<tmpid<<temp.m_nnList[tmpid]<<temp.m_ipList[tmpid];
            qDebug()<<temp.m_nnList[tmpid]<<temp.m_ipList[tmpid];
        }
        break;
    default:
        break;
    }
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    write(block);
}

void FOSTcpSocket::deleteSocket()
{

    emit deleteSignal(m_save.m_user.m_userid);
    deleteLater();
}

