#include "mainwindow.h"

MainWindow* MainWindow::smw = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    initData();
    initView();
    connections();
}

MainWindow *MainWindow::instance()
{
    if(!smw) {
        smw = new MainWindow();
    }
    return smw;
}

void MainWindow::initView()
{
    m_menuBar =new QMenuBar(this);
    setMenuBar(m_menuBar);
    m_toolBar = new ToolBar(m_client,this);
    addToolBar(Qt::TopToolBarArea,m_toolBar);
    m_tab = new QTabWidget(this);
    setCentralWidget(m_tab);
    m_task = new TaskWidget(m_client,this);
    m_tab->addTab(m_task,"测试");
    resize(600,600);
}

void MainWindow::initData()
{
    m_client = new Client(this);
}

void MainWindow::connections()
{
    connect(m_toolBar, &ToolBar::sig_choose, m_task, &TaskWidget::reInitTree);
    connect(m_toolBar, &ToolBar::sig_start, m_task, &TaskWidget::on_start);
}
