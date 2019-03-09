#ifndef FOSCONSTANT_H
#define FOSCONSTANT_H
#include <QString>
#include <QObject>
#include <QList>
#include <QHostAddress>
//#include "fostcpsocket.h"
class FOSTcpSocket;
enum RequestKind{
    LOGIN,
    REGISTER,
    TALK,
    USERLIST,
    BIND
};

enum ReplyKind{
    HAVE_LOGINED,
    LOGIN_SUCCESS,
    LOGIN_FAIL,
    REGISTER_SUCCESS,
    REGISTER_FAIL,
    TALK_OK,
    GET_USER_LIST
};

enum STATUS{
    ONLINE,
    OFFLINE
};

struct User {
    int m_userid;     // 用户id
    int m_myID;     // 只在对话时用于储存自己id
    QString m_nickname;   // 用户昵称
    QString pwd;    // 密码
    QHostAddress m_ip;
    QString m_fileName;
//    int m_status;
};

struct SaveInformation{
    quint16 m_block;
    int m_request;
    User m_user;
    int m_receiverID;
    int m_replyKind;
    QString m_text;
    QString m_ip;
    FOSTcpSocket *m_socket;
    QList<int> m_userlist;
};

#endif // FOSCONSTANT_H
