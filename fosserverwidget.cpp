#include "fosserverwidget.h"
#include "ui_fosserverwidget.h"

FOSServerWidget::FOSServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FOSServerWidget)
{
    ui->setupUi(this);
    m_server=new FOSTcpServer(this);
}

FOSServerWidget::~FOSServerWidget()
{
    delete ui;
}
