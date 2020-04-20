#include "subscribewidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>

SubscribeWidget::SubscribeWidget(Client *client, QWidget *parent) : QWidget(parent)
{
    m_client = client;
    initView();
    connections();
}

void SubscribeWidget::initView()
{
    QLabel *label_topic = new QLabel("主题:",this);
    m_lineEdit_topic = new QLineEdit(this);
    m_comboBox_qos = new QComboBox(this);
    m_comboBox_qos->addItems(QStringList()<<"QoS 0"<<"QoS 1"<<"QoS 2");
    m_btn_subscribe = new QPushButton("订阅",this);
    m_btn_unsubscribe = new QPushButton("取消订阅",this);
    m_btn_subscribe->setEnabled(false);
    m_btn_unsubscribe->setEnabled(false);
    m_textEdit_log = new QTextEdit(this);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(label_topic);
    hLayout->addWidget(m_lineEdit_topic);
    hLayout->addWidget(m_comboBox_qos);
    hLayout->addWidget(m_btn_subscribe);
    hLayout->addWidget(m_btn_unsubscribe);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout);
    vLayout->addWidget(m_textEdit_log);
    setLayout(vLayout);
}

void SubscribeWidget::connections()
{
    connect(m_btn_subscribe,&QPushButton::clicked,this, &SubscribeWidget::onSubscribe);
    connect(m_btn_unsubscribe,&QPushButton::clicked,this,&SubscribeWidget::onUnsubscribe);
    connect(m_client->in(),&QMqttClient::stateChanged,this,&SubscribeWidget::on_clientState_changed);

}

void SubscribeWidget::onSubscribe()
{
    QMqttTopicFilter topic(m_lineEdit_topic->text());

    QMqttSubscription* sub = m_client->in()->subscribe(topic,    \
                              static_cast<quint8>(m_comboBox_qos->currentIndex()));
    if(sub) {
        connect(sub,&QMqttSubscription::stateChanged,this,&SubscribeWidget::on_subState_changed);
    } else {
        m_textEdit_log->append("Subscription is null");
    }
}

void SubscribeWidget::onUnsubscribe()
{
    m_client->in()->unsubscribe(m_lineEdit_topic->text());
}

void SubscribeWidget::on_clientState_changed(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::ClientState::Connected:
        m_btn_subscribe->setEnabled(true);
        m_btn_unsubscribe->setEnabled(true);
        break;
    default:
        m_btn_subscribe->setEnabled(false);
        m_btn_unsubscribe->setEnabled(false);
        break;
    }
}

void SubscribeWidget::on_subState_changed(QMqttSubscription::SubscriptionState state)
{
    QString timeStr = QDateTime::currentDateTime().toString("HH:mm:ss zzz");
    QString stateStr;
    switch (state) {
    case QMqttSubscription::SubscriptionState::Subscribed:
        stateStr = "SUBACK";
        break;
    case QMqttSubscription::SubscriptionState::Unsubscribed:
        stateStr = "UNSUBACK";
        break;
    case QMqttSubscription::SubscriptionState::Error:
        stateStr = "Error";
        break;
    default:
        return;
    }
    m_textEdit_log->append(timeStr + " " + stateStr);
}
