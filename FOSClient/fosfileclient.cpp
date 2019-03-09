#include "fosfileclient.h"
#include <QFileDialog>

FOSFileClient::FOSFileClient(User *user,QObject *parent) : QObject(parent),m_user(user)
{
    payloadSize = 64*1024; // 64KB
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);

    // 当连接服务器成功时，发出connected()信号，开始传送文件
    connect(tcpClient, SIGNAL(connected()), this, SLOT(startTransfer()));
    connect(tcpClient, SIGNAL(bytesWritten(qint64)),
            this, SLOT(updateClientProgress(qint64)));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

}

void FOSFileClient::openFile(QString filename)
{
    fileName = filename;
}

void FOSFileClient::send(const User &toUser)
{
    openFile(toUser.m_fileName);
    // 初始化已发送字节为0
    bytesWritten = 0;
    tcpClient->connectToHost(toUser.m_ip,
                             61236);

//    qDebug()<<toUser.m_ip;
}


void FOSFileClient::startTransfer()
{
    localFile = new QFile(fileName);
    if (!localFile->open(QFile::ReadOnly)) {
        qDebug() << "client: open file error!";
        return;
    }
    // 获取文件大小
    totalBytes = localFile->size();

    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_2);
    QString currentFileName = fileName.right(fileName.size()
                                             - fileName.lastIndexOf('/')-1);
    // 保留总大小信息空间、文件名大小信息空间，然后输入文件名
    sendOut << qint64(0) << qint64(0) << currentFileName;

    // 这里的总大小是总大小信息、文件名大小信息、文件名和实际文件大小的总和
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);

    // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    sendOut << totalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    // 发送完文件头结构后剩余数据的大小
    bytesToWrite = totalBytes - tcpClient->write(outBlock);

    outBlock.resize(0);
}

void FOSFileClient::updateClientProgress(qint64 numBytes)
{
    // 已经发送数据的大小
    bytesWritten += (int)numBytes;

    // 如果已经发送了数据
    if (bytesToWrite > 0) {
        // 每次发送payloadSize大小的数据，这里设置为64KB，如果剩余的数据不足64KB，
        // 就发送剩余数据的大小
        outBlock = localFile->read(qMin(bytesToWrite, payloadSize));

        // 发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)tcpClient->write(outBlock);

        // 清空发送缓冲区
        outBlock.resize(0);
    } else { // 如果没有发送任何数据，则关闭文件
        localFile->close();
    }
    // 更新进度条
    // 如果发送完毕
    if(bytesWritten == totalBytes) {
        localFile->close();
        tcpClient->close();
    }
}

void FOSFileClient::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
}



