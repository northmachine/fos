#ifndef FOSFILECLIENT_H
#define FOSFILECLIENT_H

#include <QObject>
#include "fosconstant.h"
#include <QFile>
#include <QTcpSocket>

class FOSFileClient : public QObject
{
    Q_OBJECT
public:
    explicit FOSFileClient(User *user,QObject *parent = 0);
private:
    QTcpSocket *tcpClient;
    QFile *localFile;     // 要发送的文件
    qint64 totalBytes;    // 发送数据的总大小
    qint64 bytesWritten;  // 已经发送数据大小
    qint64 bytesToWrite;  // 剩余数据大小
    qint64 payloadSize;   // 每次发送数据的大小
    QString fileName;     // 保存文件路径
    QByteArray outBlock;  // 数据缓冲区，即存放每次要发送的数据块

    User *m_user;


private slots:
    void openFile(QString);
    void send(const User &);
    void startTransfer();
    void updateClientProgress(qint64);
    void displayError(QAbstractSocket::SocketError);


};

#endif // FOSFILECLIENT_H
