#ifndef FOSCHATWIDGET_H
#define FOSCHATWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QFrame>
#include <QList>
#include <QFileDialog>
#include <QTimerEvent>
#include "fostcpsocket.h"
#include "fosmsgitems.h"
#include "fosconstant.h"
//#include "fosfileclient.h"
//#include "fosfileserver.h"

class FOSChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FOSChatWidget(User *u,FOSTcpSocket *socket,QWidget *parent = 0);
    void receiveFile(QString);

protected:
    void timerEvent(QTimerEvent * event);

signals:
    void selectOK();
    void sendFileInfo(const User &user);

public slots:
    void on_receive_clicked(bool);
    void on_send_clicked(bool);
    void on_text_clicked(bool);
    void on_file_clicked(bool);
    void filter();
    void sendTalkTo();
//    void sendFileTo();
    void tellMainWindowToSendFile();
    void receiveTalk(int sendID, QString *meg);

public:
    //whole layout
    QVBoxLayout * allLayout;
    QHBoxLayout * userLayout;
    QScrollArea * msgsScrollArea;
    QHBoxLayout * typeLayout;
    QPlainTextEdit * textEdit;
    QHBoxLayout * sendLayout;
    //userLayout
    QPushButton * btn_t_user;//to
    QPushButton * btn_f_user;//from
    //typeLayout
    QPushButton * btn_t_type;//text
    QPushButton * btn_f_type;//file
    //sendLayout
    QPushButton * btn_f_send;//file
    QPushButton * btn_t_send;//text
    //messages list in msgsArea
    QWidget * msgsArea;
    QVBoxLayout * msgsLayout;
    QList<FOSMsgItems *> msgsList;

    int timerId;
    int timerId_2;
    //  zxh

    FOSTcpSocket *m_socket;
    User *m_toUserInfo;
    int m_blockSize;

//    FOSFileClient *m_fileClient;
//    FOSFileServer *m_fileServer;
};

#endif // FOSCHATWIDGET_H
