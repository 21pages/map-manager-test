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
    m_publish = new TaskWidget(m_client,this);
    m_tab->addTab(m_publish,"发布");
    m_message = new MessageWidget(m_client,this);
    m_tab->addTab(m_message,"日志");
}

void MainWindow::initData()
{
    m_client = new Client(this);
}

void MainWindow::connections()
{
    connect(m_publish,&TaskWidget::sig_publish_one,m_message,&MessageWidget::slot_publish_one);
}
