#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "qtmqtt/qmqttclient.h"
#include "cmd.h"


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    QMqttClient * in();
    void publish_cmd(CMD cmd);
public:
    void subscribe(bool flag);
    void send_start();
private:
    void connections();
    void handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void handleMessageStatusChanged(qint32 id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties);
signals:
    void sig_test_start(QByteArray payload);
    void sig_test_status(QByteArray payload);
private:
    QMqttClient *m_client;
};

#endif // CLIENT_H
