#include "fosmainwindow.h"
#include <QTextStream>
#include <QScrollBar>
#include <QButtonGroup>
#include <QSize>
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include <QByteArray>

FOSMainWindow::FOSMainWindow(User *me,QWidget *parent) :
    QWidget(parent)
{
    User u;
    u.m_userid = me->m_userid;
    m_database = FOSDatabase::getDatabaseInstance(u);
    //initialize list
    //    layout
    topLayout = new QHBoxLayout();
    navLayout = new QVBoxLayout();
    centreLayout = new QVBoxLayout();
    mainLayout = new QHBoxLayout();
    allLayout = new QVBoxLayout();
    //    search
    edit_search = new QLineEdit;
    mode_search = new QPushButton("搜索");
    //    parts in navLayout
    QButtonGroup * mode = new QButtonGroup();
    mode_home = new QPushButton("首页");
    mode_social = new QPushButton("通信");
    mode_setting = new QPushButton("设置");
    mode_help = new QPushButton("帮助");
    mode->addButton(mode_home);
    mode->addButton(mode_social);
    mode->addButton(mode_setting);
    mode->addButton(mode_help);
    //    parts in centreLayout
    scroll_home = new QScrollArea();
    scroll_social = new QScrollArea();
    scroll_setting = new QScrollArea();
    scroll_help = new QScrollArea();

    currentPage = scroll_home;
    //    statusBar
    statusBar = new QStatusBar();

    //topLayout details
    topLayout->addWidget(edit_search);
    topLayout->addWidget(mode_search);
    edit_search->setMinimumWidth(96);
    edit_search->setFixedHeight(32);
    mode_search->setFixedWidth(48);
    mode_search->setMinimumHeight(32);
    topLayout->setContentsMargins(2,2,2,2);
    topLayout->setSpacing(0);
    edit_search->setObjectName("edit_search");
    mode_search->setObjectName("mode_search");

    //navLayout details
    navLayout->addWidget(mode_home);
    navLayout->addWidget(mode_social);
    navLayout->addStretch();
    navLayout->addWidget(mode_setting);
    navLayout->addWidget(mode_help);
    mode_home->setCheckable(true);
    mode_home->setFixedWidth(60);
    mode_home->setMinimumHeight(60);
    mode_social->setCheckable(true);
    mode_social->setFixedWidth(60);
    mode_social->setMinimumHeight(60);
    mode_setting->setCheckable(true);
    mode_setting->setFixedWidth(60);
    mode_setting->setMinimumHeight(60);
    mode_help->setCheckable(true);
    mode_help->setFixedWidth(60);
    mode_help->setMinimumHeight(60);
    navLayout->setMargin(0);
    navLayout->setSpacing(0);

    //centreLayout details
    centreLayout->addWidget(scroll_home);
    centreLayout->addWidget(scroll_social);
    scroll_social->hide();
    centreLayout->addWidget(scroll_setting);
    scroll_setting->hide();
    centreLayout->addWidget(scroll_help);
    scroll_help->hide();
    centreLayout->setMargin(0);
    centreLayout->setSpacing(0);

    //mainLayout details
    mainLayout->addLayout(navLayout,1);
    mainLayout->addLayout(centreLayout,3);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    //allLayout details
    allLayout->addLayout(topLayout);
    allLayout->addLayout(mainLayout);
    allLayout->addSpacing(1);
    allLayout->addWidget(statusBar);
    allLayout->setMargin(0);
    allLayout->setSpacing(0);


    //parts in mode_home
    page_home = new QWidget(scroll_home);
    wlcm_image = new QLabel();
    QPixmap * img = new QPixmap(":/png/images/homeImg.png");
    QPixmap scaledPixmap = img->scaled(QSize(220,200), Qt::KeepAspectRatio);
    wlcm_image->setPixmap(scaledPixmap);
    wlcm_image->setAlignment(Qt::AlignCenter);
    feedback = new QFrame();
    ttl_FB = new QLabel("意见与建议");
    ttl_FB->setAlignment(Qt::AlignCenter);
    pte_FB = new QPlainTextEdit();
    btn_FB = new QPushButton("提交");
    shower_home = new QVBoxLayout();
    shower_home->setAlignment(Qt::AlignTop);
    shower_home->setContentsMargins(2,0,2,0);
    shower_home->setSpacing(1);
    shower_home->addSpacing(5);
    shower_home->addWidget(wlcm_image,5);
    shower_home->addWidget(feedback,3);
    QVBoxLayout * FBLayout = new QVBoxLayout();
    FBLayout->addWidget(ttl_FB,1);
    FBLayout->addWidget(pte_FB,4);
    FBLayout->addWidget(btn_FB,1);
    feedback->setLayout(FBLayout);
    page_home->setLayout(shower_home);
    scroll_home->setWidget(page_home);
    scroll_home->setWidgetResizable(true);


    //parts in social_mode
    page_social = new QWidget(scroll_social);
    shower_social = new QVBoxLayout();
    shower_social->setAlignment(Qt::AlignTop);
    shower_social->setContentsMargins(2,0,2,0);
    shower_social->setSpacing(1);
    page_social->setLayout(shower_social);
    scroll_social->setWidget(page_social);
    scroll_social->setWidgetResizable(true);

    //parts in setting_mode
    page_setting = new QWidget(scroll_setting);
    QVBoxLayout * layout = new QVBoxLayout();
    layout->setMargin(0);
    page_setting->setLayout(layout);
    scroll_setting->setWidget(page_setting);
    scroll_setting->setWidgetResizable(true);
    wait = new QLabel("敬请期待");
    wait->setAlignment(Qt::AlignCenter);
    wait->setStyleSheet("font-size:20px;");
    layout->addWidget(wait);


    //parts in help_mode
    page_help = new QWidget(scroll_help);
    pte_help = new QPlainTextEdit();
    pte_help->setFocusPolicy(Qt::NoFocus);
    shower_help = new QVBoxLayout();
    shower_help->setAlignment(Qt::AlignTop);
    shower_help->setContentsMargins(2,0,2,0);
    shower_help->addWidget(pte_help);
    page_help->setLayout(shower_help);
    scroll_help->setWidget(page_help);
    scroll_help->setWidgetResizable(true);
    QFile helpTXT(":/txt/text/help.txt");
    helpTXT.open(QFile::ReadOnly);
    if(helpTXT.isOpen())
    {
        QTextStream in(&helpTXT);
        in.setCodec("UTF-8");
        QString txt = in.readAll();
        pte_help->setPlainText(txt);
    }
    helpTXT.close();


    this->setWindowTitle(tr("FOSMainWindow"));
    this->setLayout(allLayout);
    this->resize(QSize(320,380));
    this->setObjectName("mainWindow");

    //set icon
    this->setWindowIcon(QIcon("fos.ico"));

    //read qt style sheet
    QFile ssFile_main(":/qss/styleSheets/main.qss");
    ssFile_main.open(QFile::ReadOnly);
    if(ssFile_main.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(ssFile_main.readAll());
        qDebug() << styleSheet;
        this->setStyleSheet(styleSheet);
    }
    ssFile_main.close();

    //connections
    connect(mode_search,SIGNAL(clicked()),this,SLOT(on_mode_search_clicked()));
    connect(mode_home,SIGNAL(clicked()),this,SLOT(on_mode_home_clicked()));
    connect(mode_social,SIGNAL(clicked()),this,SLOT(on_mode_social_clicked()));
    connect(mode_setting,SIGNAL(clicked()),this,SLOT(on_mode_setting_clicked()));
    connect(mode_help,SIGNAL(clicked()),this,SLOT(on_mode_help_clicked()));


    /************ zxh *************/

    //  zxh
    m_fileClient=new FOSFileClient(me,this);
    m_fileServer=new FOSFileServer(me,this);
    connect(m_fileServer,SIGNAL(sendFileOver(QHostAddress)),this,SLOT(setFileDialog(QHostAddress)));

    m_me.m_userid=me->m_userid;
    m_me.m_nickname=me->m_nickname;

    m_tcpSocket=new FOSTcpSocket(this);
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    m_tcpSocket->requestConnect();
    bindWithServerID();
    getUserList();
}


void FOSMainWindow::bindWithServerID()
{
    if (NULL == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << int(BIND) << m_me.m_userid;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}

void FOSMainWindow::getUserList()
{
    qDebug()<<"getlist";
    if (NULL == m_tcpSocket)
        return;
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_2);
    out << quint16(0) << int(USERLIST) << m_me.m_userid;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
    qDebug()<<"getlist";
    ;
}

void FOSMainWindow::on_mode_search_clicked()
{
    if(currentPage != scroll_social)
        on_mode_social_clicked();//change to social
    bool * trans = new bool;
    int target = edit_search->text().toInt(trans, 10);
    int count = 0;
    foreach(FOSUserItems * item, onLineList)
    {
        if(trans && (item->m_userInfo->m_userid != target))
            item->hide();
        else
            count++;
    }
    QString statusMsg = "已搜索到" + QString::number(count) + "个结果";
    statusBar->showMessage(statusMsg, 10000);
}


//void FOSMainWindow::sendTalkTo(const int &id, const QString &meg)
//{
//    if (NULL == m_tcpSocket)
//        return;
//    m_blockSize = 0;
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_2);
//    out << quint16(0) << int(TALK) << m_me.m_userid<<id<<meg;
//    out.device()->seek(0);
//    out << quint16(block.size() - sizeof(quint16));
//    m_tcpSocket->write(block);
//}

//void FOSMainWindow::receiveTalk(const int &id, const QString &meg)
//{
//    m_messageStore[id].append(meg);
//}

void FOSMainWindow::on_mode_home_clicked()
{
    if(currentPage != scroll_home)
    {
        currentPage->hide();
        scroll_home->show();
        currentPage = scroll_home;
    }
}

void FOSMainWindow::on_mode_social_clicked()
{
    if(currentPage != scroll_social)
    {
        currentPage->hide();
        scroll_social->show();
        currentPage = scroll_social;
    }
    getUserList();
}

void FOSMainWindow::on_mode_setting_clicked()
{
    if(currentPage != scroll_setting)
    {
        currentPage->hide();
        scroll_setting->show();
        currentPage = scroll_setting;
    }
}

void FOSMainWindow::on_mode_help_clicked()
{
    if(currentPage != scroll_help)
    {
        currentPage->hide();
        scroll_help->show();
        currentPage = scroll_help;
    }
}

void FOSMainWindow::on_item_clicked(FOSUserItems * targetItem)
{
    if(targetItem != currentItem)
    {
        targetItem->showTools();
        if(currentItem != NULL)
            currentItem->hideTOOls();
        else;
        currentItem = targetItem;
    }
    else;
}

void FOSMainWindow::changeStar(bool star, User m_otherid)
{
    if(star)
    {
        m_database->AddMyFriend(m_me, m_otherid);
        on_mode_social_clicked();
        QString statusMsg = "成功将" + QString::number(m_otherid.m_userid) + "添加星标";
        statusBar->showMessage(statusMsg,10000);
    }
    else
    {
        m_database->DeleteMyFriend(m_me, m_otherid);
        on_mode_social_clicked();
        QString statusMsg = "成功将" + QString::number(m_otherid.m_userid) + "取消星标";
        statusBar->showMessage(statusMsg,10000);
    }
}

void FOSMainWindow::setFileDialog(QHostAddress ip)
{
    foreach(FOSUserItems *tmp,onLineList){
        if(tmp->m_userInfo->m_ip==ip){
            tmp->chatWidget->receiveFile(m_fileServer->fileName);
        }
    }

}

void FOSMainWindow::receiveMessage()
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_5_2);
    if (m_blockSize == 0)
    {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
        qDebug()<<m_blockSize;
    }

    if (m_tcpSocket->bytesAvailable() < m_blockSize)
        return;

    in >> replyKind;
    qDebug()<<replyKind;
    switch(replyKind){
    case GET_USER_LIST:
    {
        for(int i=onLineList.size();i>0;i--){
            onLineList.at(i-1)->deleteLater();
            onLineList.removeLast();
        }
        while(!in.atEnd()){
            User *tmpUser=new User;
            in>>tmpUser->m_userid>>tmpUser->m_nickname>>tmpUser->m_ip;
            qDebug()<<tmpUser->m_ip<<tmpUser->m_nickname;
            tmpUser->m_myID=m_me.m_userid;
            FOSUserItems *temp=new FOSUserItems(tmpUser,m_tcpSocket);

            connect(temp->chatWidget,SIGNAL(sendFileInfo(const User &)),
                    this->m_fileClient,SLOT(send(const User &)));

            //            qDebug()<<temp->m_userInfo.m_userid;

            onLineList.append(temp);
        }
        int size = onLineList.size();
        int min = 0;
        for (int i = size - 1; i >= min; i--) {
            if(m_database->searchMyFriend(m_me,*(onLineList.at(i)->m_userInfo)))
            {
                qDebug() << m_me.m_userid <<"   "<<onLineList.at(i)->m_userInfo->m_userid;
                onLineList.at(i)->btn_star->setChecked(true);
                onLineList.at(i)->btn_star->setText("取消星标");
                onLineList.at(i)->lbl_info->setStyleSheet("QLabel{border-right:5px solid #fff777;}");
                for(int j = i; j > 0; j--)
                {
                    FOSUserItems * sort = onLineList[j];
                    onLineList[j] = onLineList[j-1];
                    onLineList[j-1] = sort;
                }
                i++;
                min++;
            }

        }
        foreach(FOSUserItems * item, onLineList)
        {
            shower_social->addWidget(item);
            connect(item,SIGNAL(item_clicked(FOSUserItems*)),this,SLOT(on_item_clicked(FOSUserItems*)));
            connect(item,SIGNAL(star_clicked(bool,User)),this,SLOT(changeStar(bool,User)));
        }
        currentItem = NULL;
    }
        break;
    case TALK_OK:
    {
        int senderID;
        QString *theMeg=new QString;
        in>>senderID>>*theMeg;
        //        m_messageStore[senderID].append(theMeg);
        foreach (FOSUserItems *u, onLineList) {
            u->chatWidget->receiveTalk(senderID,theMeg);
//            u->chatWidget->msgsScrollArea->verticalScrollBar()->setValue(u->chatWidget->msgsScrollArea->verticalScrollBar()->maximum());
        }

        break;
    }
    default:;
        break;
    }

    m_blockSize=0;
    if (m_tcpSocket->bytesAvailable() > 0)
    {
        receiveMessage();
    }
    foreach (FOSUserItems *u, onLineList) {
//        u->chatWidget->receiveTalk(senderID,theMeg);
          u->chatWidget->msgsScrollArea->verticalScrollBar()->setValue(u->chatWidget->msgsScrollArea->verticalScrollBar()->maximum());
    }
}
