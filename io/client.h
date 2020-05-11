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
    void recv_start(QByteArray payload);
private:
    void connections();
    void handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void handleMessageStatusChanged(qint32 id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties);
signals:
    void sig_test_start(QByteArray payload);
    void sig_test_status(QByteArray payload);
    void sig_start_ret(int ret);
private:
    QMqttClient *m_client;
    int start_ret;
};

#endif // CLIENT_H
