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
    QHBoxLayout *hLayout = new QHBoxLayout;
    edit = new QTextEdit(this);
    hLayout->addWidget(tree);
    hLayout->addWidget(edit);
    edit->setVisible(false);
    setLayout(hLayout);
}

void TaskWidget::connections()
{
    connect(m_client, &Client::sig_test_status, this, &TaskWidget::on_recv_test_status);
    connect(tree, &QTreeWidget::currentItemChanged, this, &TaskWidget::on_item_changed);
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
    if(payload.size() < 3)
        return;
    int index = 0;
    int type = static_cast<quint8>(payload[index++]) + 1;
    int id = static_cast<quint8>(payload[index++]) + 1;
    int status = static_cast<quint8>(payload[index++]);
    int test = type << 8 | id;

    if (0 == status) {
        setIcon(test, ":/img/run.png");
    } else if(1 == status) {
        bool ret;
        if(payload.size() < index + 1)
            return;
        ret = !!payload[index++];
        if (ret) {
            setIcon(test, ":/img/ok.png");
        } else {
            setIcon(test, ":/img/failure.png");
        }
    } else if(2 == status) {
        if(payload.size() < index + 1)
            return;
        Line line;
        uint8_t color = static_cast<quint8>(payload[index++]);
        if(color == 1) {
            line.color = QColor("#d9455f");
        } else if(color == 2){
            line.color = QColor("#21bf73");
        } else {
            line.color = QColor("#000000");
        }
        int datalen;
        int lenlen = CMD::get_length_buffer(payload.mid(index), datalen);
        if(index + lenlen + datalen <= payload.size()) {
            index += lenlen;
            const char* visible = payload.mid(index, datalen).data();
            line.text = QString(visible);
            emit lineAdded(test);
        }
    }
}

void TaskWidget::on_item_changed(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
//    uint16_t test
}

