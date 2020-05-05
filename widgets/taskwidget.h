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
#include <QTreeWidget>
#include <QTreeWidgetItem>


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
    void setIcon(uint16_t test, QString iconfile);
    void on_recv_test_status(QByteArray payload);
private:
    Client *m_client;
    QTreeWidget *tree;
};

#endif // TASKWIDGET_H
