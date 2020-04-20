#ifndef SUBSCRIBEWIDGET_H
#define SUBSCRIBEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include "client.h"

class SubscribeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SubscribeWidget(Client* client, QWidget *parent = nullptr);
private:
    void initView();
    void connections();
    void onSubscribe();
    void onUnsubscribe();
    void on_clientState_changed(QMqttClient::ClientState state);
    void on_subState_changed(QMqttSubscription::SubscriptionState state);
private:
    QLineEdit *m_lineEdit_topic;
    QComboBox *m_comboBox_qos;
    QPushButton *m_btn_subscribe;
    QPushButton *m_btn_unsubscribe;
    QTextEdit *m_textEdit_log;
private:
    Client *m_client;
};

#endif // SUBSCRIBEWIDGET_H
