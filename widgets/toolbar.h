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
    void on_choose_btn_clicked();
    void setState(bool disconnect);
signals:
    void sig_start();
    void sig_stop();
    void sig_choose();
private:
    QLineEdit *m_lineEdit_ip;
    QSpinBox *m_spinBox_port;
    QPushButton *m_btn_choose;
    QPushButton *m_btn_start;
private:
    Client *m_client;

};

#endif // TOOLBAR_H
