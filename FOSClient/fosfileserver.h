#ifndef FOSFILESERVER_H
#define FOSFILESERVER_H

#include <QObject>
#include <QFile>
#include <QTcpSocket>
#include <QTcpServer>
#include "fosconstant.h"

class FOSFileServer : public QObject
{
    Q_OBJECT
public:
    explicit FOSFileServer(User *user,QObject *parent = 0);

signals:
    void sendFileOver(QHostAddress ip);\


public:

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;     // 存放总大小信息
    qint64 bytesReceived;  // 已收到数据的大小
    qint64 fileNameSize;   // 文件名的大小信息
    QString fileName;      // 存放文件名
    QFile *localFile;      // 本地文件
    QByteArray inBlock;    // 数据缓冲区

    User *m_user;


private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);

};

#endif // FOSFILESERVER_H
