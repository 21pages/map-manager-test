#include "client.h"
#include "choosedialog.h"

#define TOPIC_SELF "M-Desktop"
#define TOPIC_ARM "M-APP1"

#define IID_CMD 0x9999
#define IOP_CMD_START 0x0010
#define IOP_CMD_STATUS 0x0011

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
    for(int i = 0; i < ChooseDialog::s_entries.size(); i++) {
        uint16_t val = ChooseDialog::s_entries.at(i);
        cmd.m_msg_payload.append(val>>8);
        cmd.m_msg_payload.append(val);
    }
    publish_cmd(cmd);
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
                        break;
                    case IOP_CMD_STATUS:
                        emit sig_test_status(cmd.m_msg_payload);
                        break;
                }
            }
        }
    }
}
