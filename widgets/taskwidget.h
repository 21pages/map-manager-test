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
#include "client.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>

struct Line{
    QColor color;
    QString text;
};

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
    void on_item_changed(QTreeWidgetItem *current, QTreeWidgetItem *previous);
signals:
    void lineAdded(uint16_t test);
private:
    Client *m_client;
    QTreeWidget *tree;
    QTextEdit *edit;
    QHash<uint16_t, QVector<Line>> hash;
};

#endif // TASKWIDGET_H
