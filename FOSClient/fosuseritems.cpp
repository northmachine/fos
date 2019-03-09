#include "fosuseritems.h"
#include <QString>
#include <QDebug>
class FOSMainWindow;
FOSUserItems::FOSUserItems(User *user,FOSTcpSocket *so, QWidget *parent) :
    QFrame(),m_socket(so)
{


    m_userInfo=user;

    chatWidget = new FOSChatWidget(m_userInfo,m_socket);

    itemLayout = new QVBoxLayout();
    toolsWidget = new QWidget();
    toolsLayout = new QGridLayout();
    lbl_info = new QLabel(m_userInfo->m_nickname+" : "+QString::number(m_userInfo->m_userid,10));
    btn_send = new QPushButton("发送信息");
    btn_star = new QPushButton("设为星标");
    btn_star->setCheckable(true);
    //检测是否设为星标



    toolsLayout->addWidget(btn_send, 0, 0);
    toolsLayout->addWidget(btn_star, 0, 1);
    btn_send->setMinimumHeight(36);
    btn_star->setMinimumHeight(36);
    toolsLayout->setMargin(0);
    toolsLayout->setSpacing(0);

    toolsWidget->setLayout(toolsLayout);
    toolsWidget->hide();

    itemLayout->addWidget(lbl_info);
    lbl_info->setMinimumHeight(36);
    lbl_info->setAlignment(Qt::AlignCenter);
    itemLayout->addWidget(toolsWidget);
    itemLayout->setMargin(0);
    itemLayout->setSpacing(0);


    this->setLayout(itemLayout);

    connect(btn_send, SIGNAL(clicked()), this, SLOT(on_btn_send_clicked()));
    connect(btn_star, SIGNAL(clicked(bool)), this, SLOT(on_btn_star_clicked(bool)));


}

void FOSUserItems::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton || e->button() == Qt::RightButton)
    {
        int x = e->pos().x(), y = e->pos().y();
        int width = itemLayout->geometry().width();
        int height = itemLayout->geometry().height();
        if(x >= 0 && x <= width && y >= 0 && y <= height)
            emit item_clicked(this);
    }
}


void FOSUserItems::on_btn_send_clicked()
{
    chatWidget->show();
}

void FOSUserItems::on_btn_star_clicked(bool down)
{
    if(down)
    {
        btn_star->setText("取消星标");
        emit star_clicked(true, *m_userInfo);
    }
    else{
        btn_star->setText("设为星标");
        emit star_clicked(false, *m_userInfo);
    }
}

