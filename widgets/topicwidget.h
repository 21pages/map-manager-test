#ifndef TOPICWIDGET_H
#define TOPICWIDGET_H

#include <QWidget>
#include "client.h"

class TopicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopicWidget(Client *client, QWidget *parent = nullptr);
private:
    void initView();
    void connections();
private:
    Client *m_client;

};

#endif // TOPICWIDGET_H
