#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    connections();
}

QMqttClient *Client::in()
{
    return m_client;
}

void Client::connections()
{
    connect(m_client, &QMqttClient::messageReceived,this,&Client::handleMessageReceived);
    connect(m_client, &QMqttClient::messageStatusChanged,this,&Client::handleMessageStatusChanged);
}

void Client::handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{

}

void Client::handleMessageStatusChanged(qint32 id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties)
{
    switch (s) {
    case QMqtt::MessageStatus::Published:
        break;

    }
}
