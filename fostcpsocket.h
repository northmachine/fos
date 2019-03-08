#ifndef FOSTCPSOCKET_H
#define FOSTCPSOCKET_H
#include "fosconstant.h"
#include <QTcpSocket>


class FOSTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit FOSTcpSocket(QObject *parent=0);

signals:
    void sendSignal(SaveInformation &temp);
    void deleteSignal(const int &id);
public slots:
    void receiveMessage();
    void sendMessage(SaveInformation &temp);
    void deleteSocket();
public:
    quint16 m_blockSize;
    SaveInformation m_save;
};

#endif // FOSTCPSOCKET_H
