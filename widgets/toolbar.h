#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include "client.h"

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(Client *client,QWidget *parent = nullptr);
private:
    void initView();
    void connections();
    void on_client_state_changed(QMqttClient::ClientState state);
    void on_btn_conn_clicked();
    void on_btn_extend_clicked();
    void on_checkbox_userpwd_clicked();
    void on_checkbox_will_clicked();
    void setState(bool disconnect);
private:
    QLineEdit *m_lineEdit_ip;
    QSpinBox *m_spinBox_port;
    QComboBox *m_comboBox_ver;
    QPushButton *m_btn_conn;
    QPushButton *m_btn_extend;
    QFrame *m_frame_extend;
    QCheckBox *m_checkBox_userpwd;
    QLineEdit *m_lineEdit_user;
    QLineEdit *m_lineEdit_pwd;
    QCheckBox *m_checkBox_cleanSession;
    QCheckBox *m_checkBox_will;
    QLineEdit *m_lineEdit_willTopic;
    QComboBox *m_comboBox_willQoS;
    QLineEdit *m_lineEdit_willMessage;
    QCheckBox *m_checkBox_willMessageHex;
    QCheckBox *m_checkBox_willRetain;

private:
    Client *m_client;

};

#endif // TOOLBAR_H
