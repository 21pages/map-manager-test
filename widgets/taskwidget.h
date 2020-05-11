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
#include <QTextEdit>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "client.h"
#include "logwidget.h"


class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(Client *client ,QWidget *parent = nullptr);
    void reInitTree();
    void on_start();
private:
    void initView();
    void connections();
    void setIcon(uint16_t test, int icon);
    void on_recv_test_status(QByteArray payload);
    void on_item_clicked(QTreeWidgetItem *item, int column);
    void on_log(QByteArray payload);
signals:
    void lineAdded(uint16_t test);
    void sig_finished();
private:
    Client *m_client;
    QTreeWidget *tree;
    LogWidget *edit;
    uint16_t cur_run_test;
};

#endif // TASKWIDGET_H
