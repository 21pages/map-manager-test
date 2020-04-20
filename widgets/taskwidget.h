#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QMetaType>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QDebug>
#include "client.h"

class PublishItem {
public:
    enum class DataType {
        hex,
        string,
    };
    ~PublishItem() {
        qDebug()<<"~PublishItem()";
    }
    explicit PublishItem(const QString& topic, const QByteArray &data, \
                         DataType type,quint8 qos,bool retain, qint32 ret):
        topic(topic),data(data),type(type),qos(qos),retain(retain),ret(ret) {}
    const QString topic;
    const QByteArray data;
    const DataType type;
    const quint8 qos;
    const bool retain;
    const qint32 ret;
};
Q_DECLARE_METATYPE(PublishItem*)


class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(Client *client ,QWidget *parent = nullptr);
private:
    void initView();
    void connections();
    void on_btn_publish_clicked();
    void on_state_changed(QMqttClient::ClientState state);
    void handleMessageStatusChanged(qint32 id, QMqtt::MessageStatus s, const QMqttMessageStatusProperties &properties);
    void on_log_publish(PublishItem *item);
signals:
    void sig_publish_one(PublishItem *item);
private:
    QLineEdit *m_lineEdit_topic;
    QLineEdit *m_lineEdit_message;
    QCheckBox *m_checkBox_hex;
    QComboBox *m_comboBox_qos;
    QCheckBox *m_checkBox_retain;
    QPushButton *m_btn_publish;
    QTextEdit *m_textEdit_log;
private:
    Client *m_client;
    QList<QSharedPointer<PublishItem>> m_publishPointerList;
};

#endif // TASKWIDGET_H
