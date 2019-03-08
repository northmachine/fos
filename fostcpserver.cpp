#include "fostcpserver.h"
#include <QList>

FOSTcpServer::FOSTcpServer(QObject *parent)
    :QTcpServer(parent)
{
    if (!listen(QHostAddress::Any, 1234))
    {
        qDebug("run fail");
        QMessageBox::critical(NULL, tr("FOS Server"),
                              tr("Unable to start the server: %1")
                              .arg(errorString()));
        exit(EXIT_FAILURE);
    }
    m_fosDatabase=FOSDatabase::getInstance();
//    m_ipList=new QMap<int,QHostAddress>;
//    m_nnList=new QMap<int,QString>;
}

void FOSTcpServer::talkRequest(const SaveInformation &save)
{
    m_save.m_text = save.m_text;
    m_save.m_receiverID=save.m_receiverID;
    m_save.m_user.m_userid=save.m_user.m_userid;
    m_save.m_replyKind = TALK_OK;
    m_userMap[m_save.m_receiverID]->sendMessage(m_save);

    //        // 聊天记录存储到数据库
    //        m_database.addHistoryMessage(m_save.m_message);

    //        if(m_userMap.end() == iter)
    //        {
    //            // 存储离线消息到数据库
    //            //m_database.messageRequest(m_save.message);
    //            m_database.addOfflineMessage(m_save.m_message);
    //        }
    //        else
//    iter.value()->sendMessage(m_save);

}

void FOSTcpServer::incomingConnection(qintptr handle)
{
    FOSTcpSocket *tmpSocket=new FOSTcpSocket(this);
    tmpSocket->setSocketDescriptor(handle);
    connect(tmpSocket, SIGNAL(deleteSignal(const int &)),
        this, SLOT(clientDisconnected(const int &)));
    connect(tmpSocket, SIGNAL(sendSignal(SaveInformation &)),
        this, SLOT(sendMessage(SaveInformation &)));
}

void FOSTcpServer::sendMessage(SaveInformation &save){
    m_save.m_request = save.m_request;
    switch(m_save.m_request){
    case LOGIN:
        m_save.m_user.m_userid = save.m_user.m_userid;
        m_save.m_user.pwd=save.m_user.pwd;
        m_save.m_socket = save.m_socket;
        // 判断是否已经登录
        if (m_userMap.contains(m_save.m_user.m_userid))
            m_save.m_replyKind = HAVE_LOGINED;
        else
        {
            // 访问数据库 登录
            m_save.m_replyKind = m_fosDatabase->searchUser(save.m_user.m_userid,save.m_user.pwd);
            if(LOGIN_SUCCESS == m_save.m_replyKind){
//                changeStatu(m_save.m_user);
                m_userMap.insert(m_save.m_user.m_userid, m_save.m_socket);
                qDebug()<<m_save.m_user.m_userid<<"---------------- ";
                qDebug()<<m_save.m_socket;
                //sendUpdateUserListToAllUser();    // remenber the teaching
            }
        }
        save.m_socket->sendMessage(m_save);
//        sendUpdateUserListToAllUser();
        break;
    case REGISTER:
        m_save.m_user.m_nickname = save.m_user.m_nickname;
        m_save.m_user.pwd=save.m_user.pwd;
        m_save.m_replyKind = m_fosDatabase->insertUser(save.m_user);
        m_save.m_user.m_userid=save.m_user.m_userid;
        save.m_socket->sendMessage(m_save);
        break;
    case TALK:
        talkRequest(save);
        break;
    case USERLIST:
        sendUserList(save);
        break;
    case BIND:
        m_userMap.insert(save.m_user.m_userid,save.m_socket);
        sendUpdateUserListToAllUser();
        break;
    default:
        break;
    }

}

void FOSTcpServer::sendUserList(SaveInformation &save)
{
    m_save.m_userlist=m_userMap.keys();
    m_save.m_replyKind=int(GET_USER_LIST);
    foreach(int id,m_userMap.keys()){
        m_ipList.insert(id,m_userMap[id]->peerAddress());
        qDebug()<<m_userMap[id]->peerAddress();
    }
    m_nnList=m_fosDatabase->getNicknameAndID();
    m_save.m_ipList=m_ipList;
    m_save.m_nnList=m_nnList;
    save.m_socket->sendMessage(m_save);
}

void FOSTcpServer::clientDisconnected(const int &id)
{
    m_userMap[id]->deleteLater();
    m_userMap.remove(id);
//    sendUpdateUserListToAllUser();
}

void FOSTcpServer::sendUpdateUserListToAllUser()
{
    foreach(FOSTcpSocket *temp,m_userMap){
        sendUserList(temp->m_save);
    }
}



//void FOSTcpServer::changeStatu(User &u)
//{
//    u.m_status=1-u.m_status;
//}
