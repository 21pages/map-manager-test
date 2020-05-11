#include "toolbar.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QThread>
#include <QTimer>
#include <QMessageBox>
#include "choosedialog.h"
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
    m_lineEdit_ip->setInputMask("999.999.999.999; ");
    m_lineEdit_ip->setText("192.168.0.109");
    m_lineEdit_ip->setMaximumWidth(150);
    m_spinBox_port = new QSpinBox(this);
    m_spinBox_port->setPrefix("端口:");
    m_spinBox_port->setRange(1,65535);
    m_spinBox_port->setValue(1883);
    m_btn_choose = new QPushButton("选择测试项目",this);
    m_btn_start = new QPushButton("开始测试",this);
    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(m_lineEdit_ip);
    hLayout1->addWidget(m_spinBox_port);
    hLayout1->addWidget(m_btn_choose);
    hLayout1->addWidget(m_btn_start);
    QWidget *widget = new QWidget(this);
    widget->setLayout(hLayout1);
    this->addWidget(widget);

}

void ToolBar::connections()
{
    connect(m_client->in(),&QMqttClient::stateChanged,this,&ToolBar::on_client_state_changed);
    connect(m_btn_start,&QPushButton::clicked,this,&ToolBar::on_btn_conn_clicked);
    connect(m_btn_choose, &QPushButton::clicked,this, &ToolBar::on_choose_btn_clicked);
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
    if(m_btn_start->text() == "开始测试") {
        QStringList ipList = m_lineEdit_ip->text().split(".");
        QString ip;
        for(int i = 0; i< ipList.size(); i++) {
            ip.append(QString("%1.").arg(ipList.at(i)));
        }
        ip.chop(1);
        m_client->in()->setHostname(ip);
        m_client->in()->setPort(static_cast<quint16>(m_spinBox_port->value()));
        m_client->in()->setProtocolVersion(QMqttClient::MQTT_3_1);
        m_client->in()->setUsername("");
        m_client->in()->setPassword("");
        m_client->in()->setCleanSession(true);
        m_client->in()->setWillTopic("");
        m_client->in()->setWillRetain(false);
        m_client->in()->setWillMessage(QByteArray());
        m_client->in()->connectToHost();
        emit sig_start();
    } else {
        m_client->subscribe(false);
        m_client->in()->disconnectFromHost();
        emit sig_stop();
    }

}

void ToolBar::on_choose_btn_clicked()
{
    if(!choose) {
        choose = new ChooseDialog(this);
        connect(choose, &ChooseDialog::sig_choose, this, &ToolBar::sig_choose);
    }
    choose->exec();
}

void ToolBar::setState(bool disconnect)
{
    m_lineEdit_ip->setEnabled(disconnect);
    m_spinBox_port->setEnabled(disconnect);
    m_btn_choose->setEnabled(disconnect);
    QString strBtn = disconnect ? "开始测试" :"停止测试";
    m_btn_start->setText(strBtn);

    m_client->subscribe(true);
    if(!disconnect) {
        QTimer::singleShot(1000, [&](){
            m_client->send_start();
        });
    }
}

void ToolBar::on_finish()
{
    if(m_btn_start->text() == "停止测试") {
        m_btn_start->click();
    }
    QMessageBox::information(nullptr, "提示", "测试完成");

}
