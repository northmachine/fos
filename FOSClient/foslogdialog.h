#ifndef FOSLOGDIALOG_H
#define FOSLOGDIALOG_H

#include <QDialog>
#include <QString>
#include <QPoint>
#include <QMouseEvent>
#include <QKeyEvent>
#include "fosloginctrl.h"
#include "fosregisterctrl.h"
#include "fosconstant.h"
#include "fosdatabase.h"

namespace Ui {
class FOSLogDialog;
}

class FOSLogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FOSLogDialog(QWidget *parent = 0);
    ~FOSLogDialog();

    // mouse drag event
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    // reminder for CAPSLOCK
    void capsLock();
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent( QKeyEvent *);

    // alert info

    // change mode (login/register)
    void changeMode();

private slots:
    void on_closeButton_clicked();

    void on_minimizeButton_clicked();

    void on_loginButton_clicked();

    void on_registerButton_clicked();

    // zxh
    void setLabelStatus(QString mes, bool kind);

private:
    QPoint m_position;
    bool isDrag;
    int m_mode; //(0->login & 1->register)
    Ui::FOSLogDialog *ui;
    QString acc_log, pas_log;
    FOSRegisterCtrl *m_registerCtrl;
    FOSLoginCtrl *m_loginCtrl;
    User m_userInfo;
//    FOSDatabase * m_database;//操作数据库
    //regexp
    QString nameExp = "^([\u4e00-\u9fa5]{1,6}|[a-zA-Z0-9]{1,12})$";
    QString passwordExp = "^[a-zA-Z0-9]{6,16}$";
    //    FOSDatabase * m_database;//操作数据库
};

#endif // FOSLOGDIALOG_H
