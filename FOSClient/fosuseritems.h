#ifndef FOSUSERITEMS_H
#define FOSUSERITEMS_H

#include "foschatwidget.h"
#include <QPoint>
#include <QWidget>
#include <QFrame>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QDebug>
#include "fosconstant.h"
#include "fostcpsocket.h"
#include "fosdatabase.h"

class FOSUserItems : public QFrame
{
    Q_OBJECT
public:
    explicit FOSUserItems(User *user,FOSTcpSocket *socket=0,QWidget *parent=0);
    void showTools(){ toolsWidget->show();}
    void hideTOOls(){ toolsWidget->hide();}
    void mousePressEvent(QMouseEvent *);

signals:
    void item_clicked(FOSUserItems *);
    void star_clicked(bool down, User m_otherid);

private slots:
    void on_btn_send_clicked();
    void on_btn_star_clicked(bool);

public:
    QVBoxLayout * itemLayout;
    QWidget * toolsWidget;
    QGridLayout * toolsLayout;
    QLabel * lbl_info;
    QPushButton * btn_send;
    QPushButton * btn_star;
    FOSChatWidget * chatWidget;
    // 用户信息接收

    //  zxh

    FOSTcpSocket *m_socket;
public:
    User *m_userInfo;
};

#endif // FOSUSERITEMS_H
