#include "fosfileserver.h"

FOSFileServer::FOSFileServer(User *user,QObject *parent) :
    QObject(parent),m_user(user)
{
    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));

    start();
}


void FOSFileServer::start()
{
    if (!tcpServer.listen(QHostAddress::Any, 61236)) {
        qDebug() << tcpServer.errorString();
//        close();

        return;
    }
    totalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;
}

void FOSFileServer::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));
    connect(tcpServerConnection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    // 关闭服务器，不再进行监听
    tcpServer.close();
}


void FOSFileServer::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_2);

    // 如果接收到的数据小于16个字节，保存到来的文件头结构
    if (bytesReceived <= sizeof(qint64)*2) {
        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)
                && (fileNameSize == 0)) {
            // 接收数据总大小信息和文件名大小信息
            in >> totalBytes >> fileNameSize;
            bytesReceived += sizeof(qint64) * 2;
        }
        if((tcpServerConnection->bytesAvailable() >= fileNameSize)
                && (fileNameSize != 0)) {
            // 接收文件名，并建立文件
            in >> fileName;
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if (!localFile->open(QFile::WriteOnly)) {
                qDebug() << "server: open file error!";
                return;
            }
        } else {
            return;
        }
    }
    // 如果接收的数据小于总数据，那么写入文件
    if (bytesReceived < totalBytes) {
        bytesReceived += tcpServerConnection->bytesAvailable();
        inBlock = tcpServerConnection->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }

    // 接收数据完成时
    if (bytesReceived == totalBytes) {
        emit sendFileOver(tcpServerConnection->peerAddress());
        tcpServerConnection->close();
        localFile->close();
        start();
    }
}

void FOSFileServer::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << tcpServerConnection->errorString();
    tcpServerConnection->close();
}

