#include "fosloginctrl.h"
#include <QByteArray>

FOSLoginCtrl::FOSLoginCtrl(QObject *parent) : QObject(parent)
{
    m_loginSocket = new FOSTcpSocket(this);
    m_loginSocket->setFlag(1);
    connect(m_loginSocket, SIGNAL(showConnectionStatus(QString, bool)),
            this, SLOT(changeLoginMessage(QString, bool)));

    connect(m_loginSocket, SIGNAL(connected()), this, SLOT(login()));
    connect(m_loginSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

FOSLoginCtrl::~FOSLoginCtrl()
{

}


/***********************************************************************************
 * ********************************************************************************/

void FOSLoginCtrl::login()
{
    m_kind=LOGIN;
//    m_userInfo.m_userid = id;
//    m_userInfo.pwd = pwd;
    if (m_loginSocket->isConnected())
    {
        requestLogin();
    }
    else
    {
        m_blockSize = 0;
        m_loginSocket->requestConnect();
    }
    //delete clientSender;
}

/*************************************************
Function Name： changeLoginMessage()
Description: 改变登录信息
Input： const QString &mes： 信息, bool isLogin：状态
Output： NULL
Changes： NULL
*************************************************/
void FOSLoginCtrl::changeLoginMessage(const QString &mes, bool isLogin)
{
    emit getLoginMessgae(mes, isLogin);
}

// mark: private slots:--------------------------------------------------

/*************************************************
Function Name： requestLogin()
Description: 请求登录
Input： NULL
Output： NULL
Changes： NULL
*************************************************/
void FOSLoginCtrl::requestLogin()
{
    if (NULL == m_loginSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << int(LOGIN) << m_userInfo.m_userid<<m_userInfo.pwd;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_loginSocket->write(block);
}





/*************************************************
Function Name： readMessage()
Description: 读取信息
Input： NULL
Output： NULL
Changes： NULL
*************************************************/
void FOSLoginCtrl::readMessage()
{
    QDataStream in(m_loginSocket);
    in.setVersion(QDataStream::Qt_5_2);
    if (m_blockSize == 0)
    {
        if (m_loginSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    if (m_loginSocket->bytesAvailable() < m_blockSize)
        return;
    int type;

    in >> type;

//    QString id;
//    bool is;
//    QString nickName;
    switch (type)
    {
    case LOGIN_SUCCESS:
        in >> m_userInfo.m_nickname>>m_userInfo.m_userid;
        emit getLoginMessgae(tr("登录成功"),true,&m_userInfo);
        break;
    case LOGIN_FAIL:
        emit getLoginMessgae(tr("登录失败.帐号或者密码错误."),false);
        m_loginSocket->disconnectFromHost();
        break;
    case HAVE_LOGINED:
        emit getLoginMessgae(tr("登录失败.该用户已经登录."),false);
        m_loginSocket->disconnectFromHost();
        break;
    default:
        m_loginSocket->disconnectFromHost();
        break;
    }

    QByteArray data = m_loginSocket->readAll();
    qDebug() << "leaved in socket: " << data.size();
    m_blockSize = 0;
}
