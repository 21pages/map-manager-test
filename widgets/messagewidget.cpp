#include "messagewidget.h"
#include <QVBoxLayout>
#include <QDateTime>
#include "mainwindow.h"

MessageWidget::MessageWidget(Client *client, QWidget *parent) : QWidget(parent)
{
    m_client = client;
    initView();
    connections();
}

void MessageWidget::initView()
{
    m_textEdit_msg = new QTextEdit(this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(m_textEdit_msg);
    setLayout(vLayout);
}

void MessageWidget::connections()
{
    connect(m_client->in(),&QMqttClient::messageReceived,this,&MessageWidget::handleMessageReceived);
}

void MessageWidget::handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    m_textEdit_msg->append(topic.name() + ":\t" + QString::fromUtf8(message));
}

void MessageWidget::slot_publish_one(PublishItem *item)
{
    QString dataStr;
    if(item->type == PublishItem::DataType::hex) {
        dataStr = QString::fromUtf8(item->data.toHex().toUpper());
    } else {
        dataStr = item->data;
    }
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss zzz");
    m_textEdit_msg->append(dataStr);
}
