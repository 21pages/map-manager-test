#include "taskwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>

TaskWidget::TaskWidget(Client *client, QWidget *parent) : QWidget(parent)
{
    m_client = client;
    initView();
    connections();
}

void TaskWidget::initView()
{
    QLabel *label_topic = new QLabel("主题:",this);
    m_lineEdit_topic = new QLineEdit(this);
    QLabel *label_message = new QLabel("消息:",this);
    m_lineEdit_message = new QLineEdit(this);
    m_checkBox_hex = new QCheckBox("16进制",this);
    m_comboBox_qos = new QComboBox(this);
    m_comboBox_qos->addItems(QStringList()<<"QoS 0"<<"QoS 1"<<"QoS 2");
    m_checkBox_retain = new QCheckBox("retain",this);
    m_btn_publish = new QPushButton("发布",this);
    m_textEdit_log = new QTextEdit(this);
    QHBoxLayout *hLayout_topic = new QHBoxLayout;
    hLayout_topic->addWidget(label_topic);
    hLayout_topic->addWidget(m_lineEdit_topic);
    QHBoxLayout *hLayout_message = new QHBoxLayout;
    hLayout_message->addWidget(label_message);
    hLayout_message->addWidget(m_lineEdit_message);
    hLayout_message->addWidget(m_checkBox_hex);
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(m_comboBox_qos);
    hLayout3->addWidget(m_checkBox_retain);
    hLayout3->addWidget(m_btn_publish);
    m_btn_publish->setEnabled(false);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout_topic);
    vLayout->addLayout(hLayout_message);
    vLayout->addLayout(hLayout3);
    vLayout->addWidget(m_textEdit_log);
    setLayout(vLayout);

}

void TaskWidget::connections()
{
    connect(m_client->in(),&QMqttClient::stateChanged,this,&TaskWidget::on_state_changed);
    connect(m_btn_publish, &QPushButton::clicked,this,&TaskWidget::on_btn_publish_clicked);
    connect(this,&TaskWidget::sig_publish_one,this,&TaskWidget::on_log_publish);
    qRegisterMetaType<PublishItem*>();
    connect(m_client->in(), &QMqttClient::messageStatusChanged,this,&TaskWidget::handleMessageStatusChanged);
}

void TaskWidget::on_btn_publish_clicked()
{
    QString topic = m_lineEdit_topic->text();
    QByteArray msgByte;
    QString msgStr = m_lineEdit_message->text();
    PublishItem::DataType type;
    if(m_checkBox_hex->isChecked()) {
        QString msgStrhex  = msgStr.replace(" ", "").trimmed();
        msgByte = QByteArray::fromHex(msgStrhex.toLatin1());
        type = PublishItem::DataType::hex;
    } else {
        msgByte = msgStr.toUtf8();
        type = PublishItem::DataType::string;
    }
    quint8 qos = static_cast<quint8>(m_comboBox_qos->currentIndex());
    bool retain = m_checkBox_retain->isChecked();
    qint32 ret = m_client->in()->publish(QMqttTopicName(topic),  \
                            msgByte,\
                            qos,\
                            retain);
    QSharedPointer<PublishItem> item = QSharedPointer<PublishItem>(new PublishItem(topic,msgByte,type,qos,retain,ret));
    m_publishPointerList.append(item);
//    PublishItem *item = new PublishItem(topic,msgByte,type,qos,retain,ret);
//    PublishItem item(topic,msgByte,type,qos,retain,ret);
    emit sig_publish_one(item.get());
}

void TaskWidget::on_state_changed(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::ClientState::Connected:
        m_btn_publish->setEnabled(true);
        break;
    default:
        m_btn_publish->setEnabled(false);
        break;
    }
}

void TaskWidget::handleMessageStatusChanged(qint32 id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties)
{
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss zzz");
    QString msgStr;
    switch (s) {
    case QMqtt::MessageStatus::Published:
        msgStr = "PUBLISH";
        break;
    case QMqtt::MessageStatus::Acknowledged:
        msgStr = "PUBACK";
        break;
    case QMqtt::MessageStatus::Received:
        msgStr = "PUBREC";
        break;
    case QMqtt::MessageStatus::Released:
        msgStr = "PUBREL";
        break;
    case QMqtt::MessageStatus::Completed:
        msgStr = "PUBCOMP";
        break;
    default:
        break;
    }
    m_textEdit_log->append(timeStr + " recv " + msgStr);
}

void TaskWidget::on_log_publish(PublishItem *item)
{
    QString dataStr;
    if(item->type == PublishItem::DataType::hex) {
        dataStr = QString::fromUtf8(item->data.toHex().toUpper());
    } else {
        dataStr = item->data;
    }
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss zzz");
    m_textEdit_log->append(timeStr + " send " +  dataStr);
}
