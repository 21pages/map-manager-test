#include "client.h"
#include <QTimer>
#include "choosedialog.h"

#define TOPIC_SELF "M-Desktop"
#define TOPIC_ARM "M-APP1"

#define IID_CMD 0x9999
#define IOP_CMD_START 0x0001
#define IOP_CMD_STATUS 0x0002

Client::Client(QObject *parent) : QObject(parent)
{
    m_client = new QMqttClient(this);
    connections();
}

QMqttClient *Client::in()
{
    return m_client;
}

void Client::publish_cmd(CMD cmd)
{
    static int index = 0;
    cmd.m_pri_prm = 1;
    cmd.m_index = index++;
    srand(index);
    cmd.m_label = rand();
    cmd.m_source = QString(TOPIC_SELF).toLatin1();
    cmd.m_source.append('\0');
    cmd.m_destination = QString(TOPIC_ARM).toLatin1();
    cmd.m_destination.append('\0');
    QByteArray hex = cmd.encode();
    in()->publish(QMqttTopicName(QString("%1/%2").arg(TOPIC_SELF).arg(TOPIC_ARM)),
                  hex);
}

void Client::subscribe(bool flag)
{
    QString topic = QString("%1/%2").arg(TOPIC_ARM).arg(TOPIC_SELF);
    if(flag) {
        in()->subscribe(topic,0);
    } else {
        in()->unsubscribe(topic);
    }
}

void Client::send_start()
{
    CMD cmd;
    cmd.m_tag = (IID_CMD << 16) | IOP_CMD_START;

    uint32_t arr[4] = {0};
    for(int i = 0; i < ChooseDialog::s_entries.size(); i++) {
        uint16_t val = ChooseDialog::s_entries.at(i);
        arr[val >> 8]  |= (1 << ((val & 0xFF) - 1));
    }
    int valid = 0;
    for (int i = 0; i < 4; i++) {
        if(arr[i] != 0) {
            valid++;
        }
    }
    cmd.m_msg_payload.append(static_cast<quint8>(valid));
    for(int i = 0; i < 4; i++) {
        if(arr[i] != 0) {
            cmd.m_msg_payload.append(static_cast<quint8>(i));
            cmd.m_msg_payload.append(static_cast<quint8>(arr[i] >> 24));
            cmd.m_msg_payload.append(static_cast<quint8>(arr[i] >> 16));
            cmd.m_msg_payload.append(static_cast<quint8>(arr[i] >> 8));
            cmd.m_msg_payload.append(static_cast<quint8>(arr[i] >> 0));
        }
    }
    publish_cmd(cmd);
    start_ret = -2;
    QTimer::singleShot(1000, [&](){
       emit sig_start_ret(start_ret);
    });
}

void Client::recv_start(QByteArray payload)
{
    if(payload.size() == 1 && static_cast<quint8>(payload[0]) == 1) {
        start_ret = 0;
    } else {
        start_ret = -1;
    }
}

void Client::connections()
{
    connect(m_client, &QMqttClient::messageReceived,this,&Client::handleMessageReceived);
}

void Client::handleMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QString recvTopic = QString("%1/%2").arg(TOPIC_ARM).arg(TOPIC_SELF);
    if(topic.name() == recvTopic) {
        CMD cmd;
        bool ret = cmd.decode(message);
        if(ret) {
            if(cmd.iid() == IID_CMD) {
                switch (cmd.iop()) {
                    case IOP_CMD_START:
                        recv_start(cmd.m_msg_payload);
                        break;
                    case IOP_CMD_STATUS:
                        emit sig_test_status(cmd.m_msg_payload);
                        break;
                }
            }
        }
    }
}
