#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMenuBar>
#include <QToolBar>
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
private:
    QMenuBar *m_menuBar;
    ToolBar *m_toolBar;
    QTabWidget *m_tab;
    TaskWidget *m_task;
private:
    Client *m_client;
};

#endif // MAINWINDOW_H
