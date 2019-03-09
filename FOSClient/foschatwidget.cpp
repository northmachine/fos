#include "foschatwidget.h"
#include <QSize>
#include <QIcon>
#include <QScrollBar>
#include <QButtonGroup>
#include <qDebug>

FOSChatWidget::FOSChatWidget(User *u,FOSTcpSocket *so,QWidget *parent) : QWidget(parent),m_socket(so),m_toUserInfo(u)
{
    //  zxh
//    m_fileClient=new FOSFileClient(u,this);
//    m_fileServer=new FOSFileServer(u,this);




    //whole layout
    allLayout = new QVBoxLayout();
    userLayout = new QHBoxLayout();
    msgsScrollArea = new QScrollArea();
    typeLayout = new QHBoxLayout();
    textEdit = new QPlainTextEdit();
    sendLayout = new QHBoxLayout();
    //userLayout
    btn_t_user = new QPushButton(m_toUserInfo->m_nickname);//to
    btn_f_user = new QPushButton("我");//from
    //typeLayout
    btn_t_type = new QPushButton("文本");//text
    btn_f_type = new QPushButton("文件");//file
    //sendLayout
    btn_f_send = new QPushButton("文件分享");//file
    btn_t_send = new QPushButton("发送消息");//text
    //messages list in msgsArea
    msgsArea = new QWidget(msgsScrollArea);
    msgsLayout = new QVBoxLayout();

    //userLayout details
    userLayout->addWidget(btn_t_user);
    btn_t_user->setCheckable(true);
    userLayout->addWidget(btn_f_user);
    btn_f_user->setCheckable(true);
//    QButtonGroup * user = new QButtonGroup();
//    user->addButton(btn_t_user,0);
//    user->addButton(btn_f_user,1);
    btn_t_user->setMinimumWidth(60);
    btn_t_user->setMinimumHeight(30);
    btn_f_user->setMinimumWidth(60);
    btn_f_user->setMinimumHeight(30);
    userLayout->setMargin(0);
    userLayout->setSpacing(2);

    //msgsArea details
    msgsScrollArea->setWidget(msgsArea);
    msgsScrollArea->setWidgetResizable(true);
    msgsScrollArea->verticalScrollBar()->hide();
    msgsArea->setLayout(msgsLayout);
    msgsLayout->addStretch();
    msgsLayout->setContentsMargins(0,5,0,5);
    msgsLayout->setSpacing(3);

    //typeLayout details
    typeLayout->addWidget(btn_t_type);
    btn_t_type->setCheckable(true);
    typeLayout->addWidget(btn_f_type);
    btn_f_type->setCheckable(true);
//    QButtonGroup * type = new QButtonGroup();
//    type->addButton(btn_t_type,0);
//    type->addButton(btn_f_type,1);
    typeLayout->setMargin(0);
    typeLayout->setSpacing(2);
    btn_t_type->setMinimumHeight(30);
    btn_f_type->setMinimumHeight(30);

    //sendLayout details
    sendLayout->addWidget(btn_t_send);
    sendLayout->addWidget(btn_f_send);
    btn_t_send->setMinimumHeight(30);
    btn_f_send->setMinimumHeight(30);
    sendLayout->setMargin(0);
    sendLayout->setSpacing(2);

    //allLayout details
    allLayout->addLayout(userLayout,1);
    allLayout->addWidget(msgsScrollArea,9);
    allLayout->addLayout(typeLayout,1);
    allLayout->addWidget(textEdit,3);
    allLayout->addLayout(sendLayout,1);
    allLayout->setMargin(0);
    allLayout->setSpacing(2);


    this->setObjectName("chatWidget");
    this->setLayout(allLayout);
    this->resize(QSize(310,400));

    //read qt style sheet
    QFile ssFile(":/qss/styleSheets/chat.qss");
    ssFile.open(QFile::ReadOnly);
    if(ssFile.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(ssFile.readAll());
        this->setStyleSheet(styleSheet);
    }
    ssFile.close();
    //set icon
    this->setWindowIcon(QIcon("fos.ico"));

    //connection
    connect(btn_t_user,SIGNAL(clicked(bool)),this,SLOT(on_receive_clicked(bool)));
    connect(btn_f_user,SIGNAL(clicked(bool)),this,SLOT(on_send_clicked(bool)));

    connect(btn_t_type,SIGNAL(clicked(bool)),this,SLOT(on_text_clicked(bool)));
    connect(btn_f_type,SIGNAL(clicked(bool)),this,SLOT(on_file_clicked(bool)));

    connect(btn_t_send,SIGNAL(clicked(bool)),this,SLOT(sendTalkTo()));
    connect(btn_f_send,SIGNAL(clicked(bool)),this,SLOT(tellMainWindowToSendFile()));

//    connect(this,SIGNAL(selectOK()),m_fileClient,SLOT(send()));
//    connect(m_socket,SIGNAL(readyRead()),this,SLOT(receiveTalk()));

//    connect(btn_f_send,SIGNAL(clicked(bool)),this,SLOT());
    //    qDebug()<<"chatOK";
}

void FOSChatWidget::receiveFile(QString name)
{
    FOSMsgItems * tempMsg = new FOSfMsgItems(false,name);
    msgsList.append(tempMsg);
    msgsLayout->addWidget(msgsList.at(msgsList.size()-1));
    timerId = startTimer(100);
    msgsScrollArea->setStyleSheet("QScrollArea{border-bottom:3px solid #d3ffdc;}");
    timerId_2 = startTimer(3000);
}

void FOSChatWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerId)
    {
        msgsScrollArea->verticalScrollBar()->setValue(msgsScrollArea->verticalScrollBar()->maximum());
        killTimer(timerId);
    }
    if(event->timerId() == timerId_2)
    {
        msgsScrollArea->setStyleSheet("");
        killTimer(timerId_2);
    }
}

void FOSChatWidget::on_receive_clicked(bool checked)
{
    if(checked)
    {
        if(btn_f_user->isChecked())
            btn_f_user->setChecked(false);
    }
    filter();
}

void FOSChatWidget::on_send_clicked(bool checked)
{
    if(checked)
    {
        if(btn_t_user->isChecked())
            btn_t_user->setChecked(false);
    }
    filter();
}

void FOSChatWidget::on_text_clicked(bool checked)
{
    if(checked)
    {
        if(btn_f_type->isChecked())
            btn_f_type->setChecked(false);
    }
    filter();
}

void FOSChatWidget::on_file_clicked(bool checked)
{
    if(checked)
    {
        if(btn_t_type->isChecked())
            btn_t_type->setChecked(false);
    }
    filter();
}

void FOSChatWidget::filter()
{
    //create 4 hideLists
    static QList<FOSMsgItems*> sendHide, receiveHide, textHide, fileHide;
    foreach(FOSMsgItems * item, msgsList)
    {
        if((!(item->ifSend)) && (!sendHide.contains(item)))
        {
            sendHide.append(item);
        }
        if(item->ifSend && (!receiveHide.contains(item)))
        {
            receiveHide.append(item);
        }
        if(item->type && !textHide.contains(item))
        {
            textHide.append(item);
        }
        if(!item->type && !fileHide.contains(item))
        {
            fileHide.append(item);
        }
    }
    //show all first
    foreach (FOSMsgItems * item, msgsList) {
        item->msgshow();
    }
    //if user hide or not
    if(btn_t_user->isChecked())
    {
        foreach(FOSMsgItems * item, receiveHide)
            item->msghide();
    }
    else if(btn_f_user->isChecked())
    {
        foreach(FOSMsgItems * item, sendHide)
            item->msghide();
    }
    //if type hide or not
    if(btn_t_type->isChecked())
    {
        foreach(FOSMsgItems * item, textHide)
            item->msghide();
    }
    else if(btn_f_type->isChecked())
    {
        foreach(FOSMsgItems * item, fileHide)
            item->msghide();
    }
}


void FOSChatWidget::sendTalkTo()
{
    qDebug()<<"sendtalk";
    if ("" == textEdit->toPlainText())
        return;
    if (NULL == m_socket)
        return;

    FOSMsgItems * tempMsg = new FOStMsgItems(true,textEdit->toPlainText());
    msgsList.append(tempMsg);
    msgsLayout->addWidget(msgsList.at(msgsList.size()-1));
    timerId = startTimer(100);

    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << TALK << m_toUserInfo->m_myID<<m_toUserInfo->m_userid<<textEdit->toPlainText();
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_socket->write(block);
    qDebug()<<"sendtalk";
    m_blockSize=0;
    textEdit->setPlainText("");
}

void FOSChatWidget::tellMainWindowToSendFile()
{
    QFileDialog * dlg_file = new QFileDialog(0,"File Dialog","C:\\","全部文件(*);;图片(*.jpg *.png *.gif);;word文档(*.doc *.docx);;演示文稿(*.ppt *.pptx)");
//    QString path;
    if(dlg_file->exec() == QDialog::Accepted)
    {
        m_toUserInfo->m_fileName = dlg_file->selectedFiles()[0];
//        qDebug()<<path;
    }

    FOSMsgItems * tempMsg = new FOSfMsgItems(true,
                 m_toUserInfo->m_fileName.right(m_toUserInfo->m_fileName.size()-m_toUserInfo->m_fileName.lastIndexOf('/')-1));
    msgsList.append(tempMsg);
    msgsLayout->addWidget(msgsList.at(msgsList.size()-1));
    timerId = startTimer(100);
    emit sendFileInfo(*m_toUserInfo);
}

//void FOSChatWidget::sendFileTo()
//{
//    QFileDialog * dlg_file = new QFileDialog(0,"File Dialog","C:\\","全部文件;;图片(*.jpg *.png *.gif);;word文档(*.doc *.docx);;演示文稿(*.ppt *.pptx)");
////    QString path;
//    if(dlg_file->exec() == QDialog::Accepted)
//    {
//        m_toUserInfo->m_fileName = dlg_file->selectedFiles()[0];
////        qDebug()<<path;
//    }
//    emit selectOK();
//}

void FOSChatWidget::receiveTalk(int sendID,QString *meg)
{
//    QDataStream in(m_socket);
//    in.setVersion(QDataStream::Qt_5_2);
//    if (m_blockSize == 0)
//    {
//        if (m_socket->bytesAvailable() < (int)sizeof(quint16))
//            return;
//        in >> m_blockSize;
//        qDebug()<<m_blockSize;
//    }

//    if (m_socket->bytesAvailable() < m_blockSize)
//        return;

//    int replyKind;
//    in >> replyKind;
//    qDebug()<<replyKind;
//    switch(replyKind){
//    case TALK_OK:
//        int sendID;
//        in>>sendID;
        if(m_toUserInfo->m_userid==sendID){
//            in>>*ts;
            FOSMsgItems * tempMsg = new FOStMsgItems(false,*meg);
            msgsList.append(tempMsg);
            msgsLayout->addWidget(msgsList.at(msgsList.size()-1));
            timerId = startTimer(100);
            msgsScrollArea->setStyleSheet("QScrollArea{border-bottom:3px solid #d3ffdc;}");
            timerId_2 = startTimer(3000);
        }
        else
            ;
//        break;
//    default:
//        break;
//    }
//    m_blockSize=0;
}
