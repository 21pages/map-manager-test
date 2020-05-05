#include "taskwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QHeaderView>
#include <QIcon>
#include "helper.h"
#include "choosedialog.h"
#include "mainwindow.h"


struct Test_Status {
    uint16_t test;
    int8_t running;
    int32_t ret;
};

TaskWidget::TaskWidget(Client *client, QWidget *parent) : QWidget(parent)
{
    m_client = client;
    initView();
    connections();
}

void TaskWidget::reInitTree()
{
    for(int i = 0; i < tree->topLevelItemCount(); i++) {
        Helper::recursivelyDelTreeItem(tree->topLevelItem(i));
    }
    for(int i = 0; i < ChooseDialog::s_entries.size(); i++) {
        uint16_t val = ChooseDialog::s_entries.at(i);
        int type = val >> 8;
        int id = val & 0xFF;
        QTreeWidgetItem *top = tree->topLevelItem(type-1);
        QTreeWidgetItem *item = new QTreeWidgetItem (top);
        QStringList * pArr[] = {&ChooseDialog::s_function,
                               &ChooseDialog::s_performance,
                               &ChooseDialog::s_compatibility,
                               &ChooseDialog::s_interface};
        QStringList *pList = pArr[type-1];
        item->setText(0, pList->at(id-1));
        item->setIcon(1, QIcon());
        item->setData(0,Qt::UserRole, val);
    }

}

void TaskWidget::on_start()
{
    for(int i = 0; i < tree->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = tree->topLevelItem(i);
        for(int j = 0; j < item->childCount(); j++) {
            item->child(j)->setIcon(1,QIcon(":/img/wait.png"));
        }
    }
}

void TaskWidget::initView()
{
    tree = new QTreeWidget(this);
    tree->setHeaderLabels(QStringList()<<"测试项"<<"测试进度");
    tree->header()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList list;
    list<<"功能测试"<<"性能测试"<<"兼容性测试"<<"接口测试";
    for(int i = 0; i < list.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(tree);
        item->setText(0, list.at(i));
        item->setExpanded(true);
    }
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(tree);
    setLayout(vLayout);
}

void TaskWidget::connections()
{
    connect(m_client, &Client::sig_test_status, this, &TaskWidget::on_recv_test_status);
}

void TaskWidget::setIcon(uint16_t test, QString iconfile)
{
    int type = test >> 8;
    if(type <1 || type > 4) {
        return;
    }
    QTreeWidgetItem *top = tree->topLevelItem(type-1);
    for(int i = 0; i < top->childCount(); i++) {
        QTreeWidgetItem *item = top->child(i);
        if(item->data(0, Qt::UserRole).toInt() == test) {
            item->setIcon(1, QIcon(iconfile));
        }
    }
}

void TaskWidget::on_recv_test_status(QByteArray payload)
{
    if(payload.size() != sizeof(Test_Status)) {
        return;
    }
    char *data = payload.data();
    Test_Status report;
    memcpy(&report, data, sizeof(Test_Status));

    QString iconfile;
    if(report.running) {
        iconfile = ":/img/run.png";
    } else {
        if(report.ret == 0) {
            iconfile = ":/img/ok.png";
        } else {
            iconfile = ":/img/failure.png";
        }
    }
    setIcon(report.test, iconfile);

}

