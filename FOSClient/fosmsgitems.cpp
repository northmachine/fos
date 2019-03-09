#include "fosmsgitems.h"
#include <QFont>

FOSMsgItems::FOSMsgItems(bool send):
    QFrame()
{
    ifSend = send;
    itemLayout = new QHBoxLayout(this);
    this->setLayout(itemLayout);
    itemLayout->setMargin(0);
    itemLayout->setSpacing(0);
    if(!send)
    {
        itemLayout->setAlignment(Qt::AlignLeft);
        this->setStyleSheet("QLabel{border-right:none;}");
    }
    else
    {
        itemLayout->setAlignment(Qt::AlignRight);
        this->setStyleSheet("QLabel{border-left:none;}");
    }
}

FOStMsgItems::FOStMsgItems(bool send, QString content):
    FOSMsgItems(send)
{
    type = 0;
    lbl_content = new QLabel(content);
    itemLayout->addWidget(lbl_content);
    itemLayout->setContentsMargins(2,3,2,3);
}


FOSfMsgItems::FOSfMsgItems(bool send, QString fileName):
     FOSMsgItems(send)
{
    type = 1;
    lbl_fileName = new QLabel("文件：" + fileName);
    itemLayout->addWidget(lbl_fileName);
    itemLayout->setContentsMargins(2,3,2,3);

}
