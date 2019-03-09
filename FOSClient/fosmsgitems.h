#ifndef FOSMSGITEMS_H
#define FOSMSGITEMS_H

#include <QString>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class FOSMsgItems : public QFrame
{
public:
    FOSMsgItems(bool send);//if send or not
    void msgshow(){
        if(!this->isVisible())
            this->show();
    }
    void msghide(){
        if(this->isVisible())
            this->hide();
    }

    bool ifSend;
    int type;//(0 means text) & (1 means file)
    QHBoxLayout * itemLayout;


};

#endif // FOSMSGITEMS_H

//text type message items
class FOStMsgItems : public FOSMsgItems
{
public:
    FOStMsgItems(bool send, QString content);
    QLabel * lbl_content;
};


//file type message items
class FOSfMsgItems : public FOSMsgItems
{
public:
    FOSfMsgItems(bool send, QString fileName);
    QLabel * lbl_fileName;
};
