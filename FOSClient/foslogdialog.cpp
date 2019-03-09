#include "foslogdialog.h"
#include "ui_foslogdialog.h"
#include "windows.h"
#include "fosmainwindow.h"
#include <QFile>
#include <QSize>
#include <QIcon>
#include <QDebug>
#include <QGridLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QRegExp>

FOSLogDialog::FOSLogDialog(QWidget *parent) :
    QDialog(parent),
    m_mode(0),
    ui(new Ui::FOSLogDialog)
{
    ui->setupUi(this);

    //read qt style sheet
    QFile ssFile(":/qss/styleSheets/log.qss");
    ssFile.open(QFile::ReadOnly);
    if(ssFile.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(ssFile.readAll());
        this->setStyleSheet(styleSheet);
    }
    ssFile.close();

    // remove outside frame
    this->setWindowOpacity(1);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowMinimizeButtonHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    capsLock();// initialize CapsLock state

    //set icon
    this->setWindowIcon(QIcon("fos.ico"));

    //tab order
    this->ui->account->setFocus();
    this->setTabOrder(this->ui->account,this->ui->password);
    this->setTabOrder(this->ui->password,this->ui->loginButton);
    this->setTabOrder(this->ui->loginButton,this->ui->registerButton);

    // zxh
    m_registerCtrl=new FOSRegisterCtrl(this);
    m_loginCtrl=new FOSLoginCtrl(this);
    connect(m_loginCtrl,SIGNAL(getLoginMessgae(QString,bool,const User*)),
            this,SLOT(setLabelStatus(QString,bool)));
}

FOSLogDialog::~FOSLogDialog()
{
    delete ui;
}

// close and minimize button
void FOSLogDialog::on_closeButton_clicked()
{
    this->close();
}

void FOSLogDialog::on_minimizeButton_clicked()
{
    this->showMinimized();
}

// mouse drag event
void FOSLogDialog::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        isDrag = true;
        m_position = e->globalPos() - this->pos();
        e->accept();
    }
}

void FOSLogDialog::mouseMoveEvent(QMouseEvent *e)
{
    if(isDrag && (e->buttons() && Qt::LeftButton))
    {
        move(e->globalPos() - m_position);
        e->accept();
    }
}

void FOSLogDialog::mouseReleaseEvent(QMouseEvent *)
{
    isDrag = false;
}

// reminder for CAPSLOCK
void FOSLogDialog::capsLock()
{
    if(LOBYTE(GetKeyState(VK_CAPITAL))) {
        this->ui->groupBox_2->setStyleSheet("QGroupBox#groupBox_2{background-image:url(:/png/images/capital.png);}");
    }
    else {
        this->ui->groupBox_2->setStyleSheet("");
    }
}

void FOSLogDialog::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_CapsLock)
        capsLock();
    QDialog::keyPressEvent(event);
}

void FOSLogDialog::keyReleaseEvent( QKeyEvent *event)
{
    if(event->key() == Qt::Key_CapsLock)
        capsLock();
    QDialog::keyReleaseEvent(event);
}

// change mode (login/register)
void FOSLogDialog::changeMode()
{
    if(0 == m_mode)
    {
        this->ui->loginButton->setText("确认注册");
        this->ui->registerButton->setText("返回登录");
        this->ui->label->setText("昵称");
        this->ui->label_2->setText("密码");
        acc_log = this->ui->account->text();
        pas_log = this->ui->password->text();
        this->ui->account->setPlaceholderText("中文1~6或字符1~12");
        this->ui->password->setPlaceholderText("字符限制6~16");
        this->ui->account->setText("");
        this->ui->password->setText("");
        m_mode = 1;
    }
    else{
        this->ui->loginButton->setText("登录");
        this->ui->registerButton->setText("注册");
        this->ui->label->setText("账号");
        this->ui->label_2->setText("密码");
        this->ui->account->setInputMask("");
        this->ui->password->setInputMask("");
        this->ui->account->setText(acc_log);
        this->ui->password->setText(pas_log);
        m_mode = 0;
    }
}

void FOSLogDialog::on_loginButton_clicked()
{
    if(1==m_mode){
        QRegExp rx(nameExp), rx_2(passwordExp);
        if(!rx.exactMatch(this->ui->account->text()))
        {
            this->ui->alert->setText("昵称限制：中文1~6或字符1~12");
            this->ui->account->setText("");
            return;
        }
        else if(!rx_2.exactMatch(this->ui->password->text()))
        {
            this->ui->alert->setText("密码限制：字母或数字6~16");
            this->ui->password->setText("");
            return;
        }
        QString acc = this->ui->account->text();
        QString pas = this->ui->password->text();
        m_userInfo.m_nickname = acc;
        m_userInfo.pwd = pas;
        m_registerCtrl->registerAccount(m_userInfo);
        changeMode();
        this->ui->account->setText("");
        this->ui->password->setText("");
    }
    else{
        m_userInfo.m_userid=this->ui->account->text().toInt();
        m_userInfo.pwd=this->ui->password->text();
        m_loginCtrl->m_userInfo.m_userid=m_userInfo.m_userid;
        m_loginCtrl->m_userInfo.pwd=m_userInfo.pwd;
        m_loginCtrl->m_loginSocket->abort();
        m_loginCtrl->m_loginSocket->connectToHost(FOSTcpSocket::s_hostAddress, FOSTcpSocket::s_hostPort);
        //        FOSMainWindow * w = new FOSMainWindow();
        //        w->show();
        //        this->close();
        //        User u;
        //        u.m_userid = this->ui->account->text().toInt();
        //        m_database = FOSDatabase::getDatabaseInstance(u);
    }
}

void FOSLogDialog::on_registerButton_clicked()
{
    changeMode();
}

void FOSLogDialog::setLabelStatus(QString mes, bool kind)
{
    if(kind){
        //        in >> m_user.m_userid;
        //        QMessageBox::about(NULL, tr("登录结果"),
        //                           tr("%1, 你好！ 恭喜,登录成功. "
        //                              "您的ID是%2, 请牢牢记住.").arg(m_user.m_nickname).arg(m_user.m_userid));
        FOSMainWindow * w = new FOSMainWindow(&m_loginCtrl->m_userInfo);
        w->show();
        m_loginCtrl->m_loginSocket->disconnectFromHost();
        close();
    }
    else
        QMessageBox::about(NULL, tr("登录结果"), tr("登录失败")+mes);
}
