#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMenuBar>
#include "client.h"
#include "toolbar.h"
#include "taskwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    static MainWindow* instance();
    static MainWindow *smw;
private:
    explicit MainWindow(QWidget *parent = nullptr);
    void initView();
    void initData();
    void connections();
    void on_start_ret(int ret);
private:
    QMenuBar *m_menuBar;
    ToolBar *m_toolBar;
    TaskWidget *m_task;
private:
    Client *m_client;
};

#endif // MAINWINDOW_H
