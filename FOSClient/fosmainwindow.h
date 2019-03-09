#ifndef FOSMAINWINDOW_H
#define FOSMAINWINDOW_H

#include "fosuseritems.h"
#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QStatusBar>
#include <QList>
#include "fosconstant.h"
#include "fostcpsocket.h"
#include "fosdatabase.h"
#include <QMap>
#include <QScrollArea>
#include <QScrollBar>
#include "fosfileclient.h"
#include "fosfileserver.h"

class FOSMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit FOSMainWindow(User *me,QWidget *parent = 0);

    /**************** zxh ******************/
    void bindWithServerID();
    void getUserList();
//    void sendTalkTo(const int &id,const QString &meg);
//    void receiveTalk(const int &id,const QString &meg);
signals:


private slots:
    void on_mode_search_clicked();
    void on_mode_home_clicked();
    void on_mode_social_clicked();
    void on_mode_setting_clicked();
    void on_mode_help_clicked();
    void on_item_clicked(FOSUserItems *);
    void changeStar(bool, User);

    void setFileDialog(QHostAddress ip);

    //   zxh

    void receiveMessage();

private:


    //layout
    QHBoxLayout * topLayout;
    QVBoxLayout * navLayout;
    QVBoxLayout * centreLayout;
    QHBoxLayout * mainLayout;
    QVBoxLayout * allLayout;
    //search
    QLineEdit * edit_search;
    QPushButton * mode_search;
    //parts in navLayout
    QPushButton * mode_home;
    QPushButton * mode_social;
    QPushButton * mode_setting;
    QPushButton * mode_help;
    //parts in centreLayout
    QScrollArea * scroll_home;
    QScrollArea * scroll_social;
    QScrollArea * scroll_setting;
    QScrollArea * scroll_help;
    QWidget * currentPage;
    //statusBar
    QStatusBar * statusBar;

    //parts in mode_home
    QWidget * page_home;
    QVBoxLayout * shower_home;
    QLabel * wlcm_image;
    QFrame * feedback;
    QLabel * ttl_FB;
    QPlainTextEdit * pte_FB;
    QPushButton * btn_FB;

    //parts in mode_social
    QWidget * page_social;
    QVBoxLayout * shower_social;
    QList<FOSUserItems *> onLineList;
    FOSUserItems * currentItem = NULL;

    //parts in mode_setting
    QWidget * page_setting;
    QLabel * wait;
    QGridLayout * skinLayout;
    QPushButton * skin1;
    QPushButton * skin2;
    QPushButton * skin3;
    QPushButton * skin4;

    //parts in mode_help
    QWidget * page_help;
    QVBoxLayout * shower_help;
    QPlainTextEdit * pte_help;



    FOSDatabase * m_database;//操作数据库

    // zxh

    User m_me;
    FOSTcpSocket *m_tcpSocket;
    quint16 m_blockSize;
    int replyKind;

    FOSFileClient *m_fileClient;
    FOSFileServer *m_fileServer;

//    QMap<int, QList<String> > m_messageStore;
};

#endif // FOSMAINWINDOW_H
