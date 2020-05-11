#include "taskwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QHeaderView>
#include <QIcon>
#include <QSplitter>
#include "helper.h"
#include "choosedialog.h"
#include "mainwindow.h"


enum Tree_Item_Type
{
    Type_User = QTreeWidgetItem::UserType,
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
        QTreeWidgetItem *top = tree->topLevelItem(type);
        QTreeWidgetItem *item = new QTreeWidgetItem (top, Type_User);
        QStringList * pArr[] = {&ChooseDialog::s_function,
                               &ChooseDialog::s_performance,
                               &ChooseDialog::s_compatibility,
                               &ChooseDialog::s_interface};
        QStringList *pList = pArr[type];
        item->setText(0, pList->at(id));
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
    edit->on_start();
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
    edit = new LogWidget(this);
    QSplitter *split = new QSplitter(this);
    split->addWidget(tree);
    split->addWidget(edit);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(split);
    setLayout(hLayout);
}

void TaskWidget::connections()
{
    connect(m_client, &Client::sig_test_status, this, &TaskWidget::on_recv_test_status);
    connect(tree, &QTreeWidget::itemClicked, this, &TaskWidget::on_item_clicked);
    connect(m_client, &Client::sig_log, this, &TaskWidget::on_log);
}

enum ICON_SOURCE {
    ICON_SOURCE_RUN,
    ICON_SOURCE_OK,
    ICON_SOURCE_FAIL,
};

void TaskWidget::setIcon(uint16_t test, int icon)
{
    int type = test >> 8;
    if(type >= 4) {
        return;
    }
    QString iconfile;
    switch (icon) {
    case ICON_SOURCE_RUN:
        iconfile = ":/img/run.png";
        break;
    case ICON_SOURCE_OK:
        iconfile = ":/img/ok.png";
        break;
    case ICON_SOURCE_FAIL:
        iconfile = ":/img/failure.png";
        break;
    default:
        return;
    }
    QTreeWidgetItem *top = tree->topLevelItem(type);
    for(int i = 0; i < top->childCount(); i++) {
        QTreeWidgetItem *item = top->child(i);
        if(item->data(0, Qt::UserRole).toInt() == test) {
            item->setIcon(1, QIcon(iconfile));
            if(i == top->childCount()-1 && icon != ICON_SOURCE_RUN) {
                emit sig_finished();
            }
        }
    }
}

void TaskWidget::on_recv_test_status(QByteArray payload)
{
    if(payload.size() < 3)
        return;
    int index = 0;
    int type = static_cast<quint8>(payload[index++]);
    int id = static_cast<quint8>(payload[index++]);
    int status = static_cast<quint8>(payload[index++]);
    uint16_t test = type << 8 | id;

    if (0 == status) {
        setIcon(test, ICON_SOURCE_RUN);
        edit->set_current_edit_key(test);
        cur_run_test = test;
    } else if(1 == status) {
        bool ret;
        if(payload.size() < index + 1)
            return;
        ret = !!payload[index++];
        if (ret) {
            setIcon(test, ICON_SOURCE_OK);
        } else {
            setIcon(test, ICON_SOURCE_FAIL);
        }
    }
}

void TaskWidget::on_item_clicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if(item->type() == Type_User) {
        edit->set_current_edit_key(item->data(0, Qt::UserRole).toUInt());
    }
}

void TaskWidget::on_log(QByteArray payload)
{
    int index = 0;
    uint16_t test = cur_run_test;

    if(payload.size() < index + 1)
        return;
    int datalen;
    int lenlen = CMD::get_length_buffer(payload.mid(index), datalen);
    if(index + lenlen + datalen <= payload.size()) {
        index += lenlen;
        const char* visible = payload.mid(index, datalen).data();
        QString log = QString::fromLatin1(visible, datalen);
        log = QDateTime::currentDateTime().toString("[HH:mm:ss]") + log;
        edit->add_log(test, log);
    }
}


