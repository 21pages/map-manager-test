#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include "client.h"
#include "taskwidget.h"

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(Client *client, QWidget *parent = nullptr);
private:
    void initView();
    void connections();
    void handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
public slots:
    void slot_publish_one(PublishItem *item);
private:
    QTextEdit *m_textEdit_msg;
private:
    Client *m_client;
};

#endif // MESSAGEWIDGET_H
