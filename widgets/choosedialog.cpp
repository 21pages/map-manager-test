#include "choosedialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "mainwindow.h"

QVector<uint16_t> ChooseDialog::s_entries;
QStringList ChooseDialog::s_function = QStringList()<< "模组插拔事件" << "查询模组信息" << "查询模组槽位信息"
                       << "查询模组插拔状态" << "模组复位" << "模组重启"
                       << "文件传输" << "异常模块处理" << "模组对时" << "查询全部模组信息";
QStringList ChooseDialog::s_performance = QStringList()<<"并发请求" << "快速插拔模组" << "响应时间";
QStringList ChooseDialog::s_compatibility = QStringList()<< "共存性" << "互换性" << "配置文件读取" << "驱动接口兼容";
QStringList ChooseDialog::s_interface = QStringList()<< "通用消息接口" << "系统管理器消息接口" << "模组管理接口";

enum Tree_Item_Type
{
    Type_User = QTreeWidgetItem::UserType,
    Type_Root,
    Type_Function_Parent,
    Type_Performance_Parent,
    Type_Compatibility_Parent,
    Type_Interface_Parent,
    Type_Function,
    Type_Performance,
    Type_Compatibility,
    Type_Interface,
};

ChooseDialog::ChooseDialog(QWidget *parent) : QDialog(parent)
{
    initView();
}

QString ChooseDialog::test2name(uint16_t test)
{
    int type = test >> 8;
    int id = test & 0xFF;
    QStringList *pList[] = {&s_function, &s_performance, &s_compatibility, &s_interface};
    if(type < 4) {
        if(id < pList[type]->size()) {
            return pList[type]->at(id);
        }
    }
    return "";
}

void ChooseDialog::initView()
{
    tree = new QTreeWidget(this);
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(tree, Type_Root);
    rootItem->setText(0, "模组管理器测试项目");
    rootItem->setExpanded(true);
    rootItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    rootItem->setCheckState(0, Qt::Checked);

    QTreeWidgetItem *functionItem = new QTreeWidgetItem(rootItem, Type_Function_Parent);
    functionItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    functionItem->setCheckState(0, Qt::Checked);
    functionItem->setText(0, "功能测试");
    functionItem->setExpanded(true);
    for(int i = 0; i < s_function.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem (functionItem, Type_Function);
        item->setText(0, s_function.at(i));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(0, Qt::Checked);
    }

    QTreeWidgetItem *performanceItem = new QTreeWidgetItem(rootItem, Type_Performance_Parent);
    performanceItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    performanceItem->setCheckState(0, Qt::Checked);
    performanceItem->setText(0, "性能测试");
    performanceItem->setExpanded(true);
    for(int i = 0; i < s_performance.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem (performanceItem, Type_Performance);
        item->setText(0, s_performance.at(i));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(0, Qt::Checked);
    }

    QTreeWidgetItem *compatibilityItem = new QTreeWidgetItem(rootItem, Type_Compatibility_Parent);
    compatibilityItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    compatibilityItem->setCheckState(0, Qt::Checked);
    compatibilityItem->setText(0, "兼容性测试");
    compatibilityItem->setExpanded(true);
    for(int i = 0; i < s_compatibility.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem (compatibilityItem, Type_Compatibility);
        item->setText(0, s_compatibility.at(i));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(0, Qt::Checked);
    }

    QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(rootItem, Type_Interface_Parent);
    interfaceItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    interfaceItem->setCheckState(0, Qt::Checked);
    interfaceItem->setText(0, "接口测试");
    interfaceItem->setExpanded(true);
    for(int i = 0; i < s_interface.size(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem (interfaceItem, Type_Interface);
        item->setText(0, s_interface.at(i));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(0, Qt::Checked);
    }

    QPushButton *ok_btn = new QPushButton("确定", this);
    QPushButton *cancel_btn = new QPushButton("取消", this);
    connect(ok_btn, &QPushButton::clicked, this, &ChooseDialog::on_ok);
    connect(cancel_btn, &QPushButton::clicked, this, &ChooseDialog::close);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(tree);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addStretch();
    hlayout->addWidget(ok_btn);
    hlayout->addWidget(cancel_btn);
    vLayout->addLayout(hlayout);
    setLayout(vLayout);

    resize(400,600);
    setWindowTitle("选择测试项目");

    connect(tree, &QTreeWidget::itemClicked, this, &ChooseDialog::onItemClicked);

}

void ChooseDialog::on_ok()
{
    s_entries.clear();
    QTreeWidgetItem *rootItem = tree->topLevelItem(0);
    for(int i = 0; i < rootItem->childCount(); i++) {
        QTreeWidgetItem * pItem = rootItem->child(i);
        for(int j = 0; j < pItem->childCount(); j++) {
            QTreeWidgetItem *item = pItem->child(j);
            if(item->checkState(0) == Qt::Checked) {
                s_entries.append(((i)<<8) | (j));
            }
        }
    }
    emit sig_choose();
    close();
}

void ChooseDialog::onItemClicked(QTreeWidgetItem *item, int column)
{
    if(column != 0) {
        return;
    }
    int type = item->type();
    switch (type) {
    case Type_Root:{
            QTreeWidgetItemIterator it(item);
            while (*it) {
                  (*it)->setCheckState(0, item->checkState(0));
                  ++it;
            }
    }
        break;
    case Type_Function_Parent:
    case Type_Performance_Parent:
    case Type_Compatibility_Parent:
    case Type_Interface_Parent:
    {
        for(int i = 0; i < item->childCount(); i++) {
            item->child(i)->setCheckState(0, item->checkState(0));
        }
    }
        break;
    }
}
