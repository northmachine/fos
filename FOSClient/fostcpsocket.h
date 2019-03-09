#ifndef FOSTCPSOCKET_H
#define FOSTCPSOCKET_H
#include <QTcpSocket>


class FOSTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit FOSTcpSocket(QObject *parent=0);
    void requestConnect();
    // 是否连接
    bool isConnected();

    void setFlag(int);
    int flag();

    // 获取ip
    static QString getIP();
    //获取当前的日期和时间
    static QString getCurrentDateTime();

    static QHostAddress s_hostAddress;
    static int s_hostPort;

signals:
    void showConnectionStatus(const QString &, bool isLogin = false);

private slots:
    // 连接被客户端关闭
    void connectionClosed();
    // 连接创建
    void connectionCreate();
    // 显示错误
    void dispalyError(QAbstractSocket::SocketError);

private:
    // 链接信号与槽
    void linkSignalWithSlot();

    bool m_isConnected;
    int m_flag;

};

#endif // FOSTCPSOCKET_H
