#include "toolbar.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include "helper.h"

ToolBar::ToolBar(Client *client, QWidget *parent) : QToolBar(parent)
{
    m_client = client;
    initView();
    connections();
}

void ToolBar::initView()
{
    m_lineEdit_ip = new QLineEdit(this);
    m_lineEdit_ip->setInputMask("999.999.999.999;_");
    m_lineEdit_ip->setMaximumWidth(150);
    m_spinBox_port = new QSpinBox(this);
    m_spinBox_port->setPrefix("端口:");
    m_spinBox_port->setRange(1,65535);
    m_comboBox_ver = new QComboBox(this);
    m_comboBox_ver->addItems(QStringList()<<"MQTT 3.1"<<"MQTT 3.1.1"<<"MQTT 5.0");
    m_btn_extend = new QPushButton(this);
    m_btn_extend->setIcon(QIcon(QPixmap(":/img/more.png").scaled(30,30)));
    m_btn_conn = new QPushButton("连接",this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(m_lineEdit_ip);
    hLayout1->addWidget(m_spinBox_port);
    hLayout1->addWidget(m_comboBox_ver);
    hLayout1->addWidget(m_btn_extend);
    hLayout1->addWidget(m_btn_conn);
    QFrame *frame1 = new QFrame(this);
    frame1->setLayout(hLayout1);

    m_checkBox_userpwd = new QCheckBox(this);
    QLabel *userLabel = new QLabel("用户名:",this);
    m_lineEdit_user = new QLineEdit(this);
    QLabel *pwdLabel = new QLabel("密码:",this);
    m_lineEdit_pwd = new QLineEdit(this);
    m_lineEdit_pwd->setEchoMode(QLineEdit::EchoMode::Password);
    m_checkBox_userpwd->setChecked(false);
    m_lineEdit_user->setEnabled(false);
    m_lineEdit_pwd->setEnabled(false);
    m_checkBox_cleanSession = new QCheckBox("清理会话",this);
    m_checkBox_cleanSession->setChecked(true);
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(m_checkBox_userpwd);
    hLayout2->addWidget(userLabel);
    hLayout2->addWidget(m_lineEdit_user);
    hLayout2->addWidget(pwdLabel);
    hLayout2->addWidget(m_lineEdit_pwd);
    hLayout2->addWidget(m_checkBox_cleanSession);

    m_checkBox_will = new QCheckBox("遗嘱",this);
    m_checkBox_will->setChecked(false);
    QLabel *label_willTopic = new QLabel("topic:",this);
    m_lineEdit_willTopic = new QLineEdit(this);
    QLabel *label_willQos = new QLabel("QoS:",this);
    m_comboBox_willQoS = new QComboBox(this);
    m_comboBox_willQoS->addItems(QStringList()<<"0"<<"1"<<"2");
    QLabel *label_willMessage = new QLabel("message:",this);
    m_lineEdit_willMessage = new QLineEdit(this);
    m_checkBox_willMessageHex = new QCheckBox("hex?",this);
    m_checkBox_willRetain = new QCheckBox("retain",this);
    m_lineEdit_willTopic->setEnabled(false);
    m_comboBox_willQoS->setEnabled(false);
    m_lineEdit_willMessage->setEnabled(false);
    m_checkBox_willMessageHex->setEnabled(false);
    m_checkBox_willRetain->setEnabled(false);
    QHBoxLayout *hLayout3 = new QHBoxLayout;
    hLayout3->addWidget(m_checkBox_will);
    hLayout3->addWidget(label_willTopic);
    hLayout3->addWidget(m_lineEdit_willTopic);
    hLayout3->addWidget(label_willQos);
    hLayout3->addWidget(m_comboBox_willQoS);
    hLayout3->addWidget(label_willMessage);
    hLayout3->addWidget(m_lineEdit_willMessage);
    hLayout3->addWidget(m_checkBox_willMessageHex);
    hLayout3->addWidget(m_checkBox_willRetain);

    m_frame_extend = new QFrame(this);
    QVBoxLayout *vLayout2 = new QVBoxLayout;
    vLayout2->addLayout(hLayout2);
    vLayout2->addLayout(hLayout3);
    m_frame_extend->setLayout(vLayout2);
    m_frame_extend->setVisible(false);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(frame1);
    vLayout->addWidget(m_frame_extend);
    QFrame *frame = new QFrame(this);
    frame->setLayout(vLayout);
    insertWidget(new QAction(),frame);

    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);
    hLayout1->setMargin(0);
    hLayout1->setContentsMargins(0,0,0,0);
    hLayout2->setMargin(0);
    hLayout2->setContentsMargins(0,0,0,0);
    hLayout3->setMargin(0);
    hLayout3->setContentsMargins(0,0,0,0);
    vLayout2->setMargin(0);
    vLayout2->setContentsMargins(0,0,0,0);
}

void ToolBar::connections()
{
    connect(m_client->in(),&QMqttClient::stateChanged,this,&ToolBar::on_client_state_changed);
    connect(m_btn_conn,&QPushButton::clicked,this,&ToolBar::on_btn_conn_clicked);
    connect(m_btn_extend, &QPushButton::clicked,this,&ToolBar::on_btn_extend_clicked);
    connect(m_checkBox_userpwd,&QCheckBox::clicked,this,&ToolBar::on_checkbox_userpwd_clicked);
    connect(m_checkBox_will,&QCheckBox::clicked,this,&ToolBar::on_checkbox_will_clicked);
}

void ToolBar::on_client_state_changed(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::Connected:
        setState(false);
        break;
    case QMqttClient::Disconnected:
        setState(true);
        break;
    default:
        break;
    }
}

void ToolBar::on_btn_conn_clicked()
{
    if(m_btn_conn->text() == "连接") {
        QStringList ipList = m_lineEdit_ip->text().split(".");
        QString ip;
        for(int i = 0; i< ipList.size(); i++) {
            ip.append(QString("%1.").arg(ipList.at(i)));
        }
        ip.chop(1);
        m_client->in()->setHostname(ip);
        m_client->in()->setPort(static_cast<quint16>(m_spinBox_port->value()));
        m_client->in()->setProtocolVersion(static_cast<QMqttClient::ProtocolVersion>(m_comboBox_ver->currentIndex() + 3));
        if(m_checkBox_userpwd->isChecked()) {
            m_client->in()->setUsername(m_lineEdit_user->text());
            m_client->in()->setPassword(m_lineEdit_pwd->text());
        } else {
            m_client->in()->setUsername("");
            m_client->in()->setPassword("");
        }
        m_client->in()->setCleanSession(m_checkBox_cleanSession->isChecked());
        if(m_checkBox_will->isChecked()) {
            m_client->in()->setWillTopic(m_lineEdit_willTopic->text());
            m_client->in()->setWillQoS(static_cast<quint8>(m_comboBox_willQoS->currentIndex()));
            m_client->in()->setWillRetain(m_checkBox_willRetain->isChecked());
            QByteArray willData;
            if(m_checkBox_willMessageHex->isChecked()) {
                willData = Helper::S2B(m_lineEdit_willMessage->text());
            } else {
                willData = m_lineEdit_willMessage->text().toUtf8();
            }
            m_client->in()->setWillMessage(willData);
        } else {
            m_client->in()->setWillTopic("");
            m_client->in()->setWillRetain(false);
            m_client->in()->setWillMessage(QByteArray());
        }
        m_client->in()->connectToHost();
    } else {
        m_client->in()->disconnectFromHost();
    }

}

void ToolBar::on_btn_extend_clicked()
{
    if(m_frame_extend->isVisible()) {
        m_frame_extend->setVisible(false);
        m_btn_extend->setIcon(QIcon(QPixmap(":/img/more.png").scaled(30,30)));
    } else {
        m_frame_extend->setVisible(true);
        m_btn_extend->setIcon(QIcon(QPixmap(":/img/less.png").scaled(30,30)));
    }
}

void ToolBar::on_checkbox_userpwd_clicked()
{
    bool checked = m_checkBox_userpwd->isChecked();
    m_lineEdit_user->setEnabled(checked);
    m_lineEdit_pwd->setEnabled(checked);
}

void ToolBar::on_checkbox_will_clicked()
{
    bool checked = m_checkBox_will->isChecked();
    m_lineEdit_willTopic->setEnabled(checked);
    m_comboBox_willQoS->setEnabled(checked);
    m_lineEdit_willMessage->setEnabled(checked);
    m_checkBox_willMessageHex->setEnabled(checked);
    m_checkBox_willRetain->setEnabled(checked);
}

void ToolBar::setState(bool disconnect)
{
    m_lineEdit_ip->setEnabled(disconnect);
    m_spinBox_port->setEnabled(disconnect);
    m_comboBox_ver->setEnabled(disconnect);
    QString strBtn = disconnect ? "连接" :"断开";
    m_btn_conn->setText(strBtn);
}
