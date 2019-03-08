#ifndef FOSSERVERWIDGET_H
#define FOSSERVERWIDGET_H

#include <QWidget>
#include "fostcpserver.h"

namespace Ui {
class FOSServerWidget;
}

class FOSServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FOSServerWidget(QWidget *parent = 0);
    ~FOSServerWidget();

private:
    Ui::FOSServerWidget *ui;
    FOSTcpServer *m_server;
};

#endif // FOSSERVERWIDGET_H
