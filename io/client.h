#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "qtmqtt/qmqttclient.h"

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    QMqttClient * in();
private:
    void connections();
    void handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void handleMessageStatusChanged(qint32 id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties);
private:
    QMqttClient *m_client;
};

#endif // CLIENT_H
