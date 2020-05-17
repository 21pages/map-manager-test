#include "logwidget.h"
#include <QVBoxLayout>
#include "choosedialog.h"
#include <QMutexLocker>
#include <QMutex>

static QMutex mutex;

LogWidget::LogWidget(QWidget *parent) : QWidget(parent)
{
    label = new QLabel(this);
    edit = new QTextEdit(this);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(label);
    vLayout->addWidget(edit);
    setLayout(vLayout);
    QFont font = edit->font();
    font.setPointSize(14);
    edit->setFont(font);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);
}

void LogWidget::on_choose()
{
    QMutexLocker lock(&mutex);
    cur_test = -1;
    cur_lines = 0;
    hash.clear();
    label->setText("");
    edit->clear();
}

void LogWidget::on_start()
{
    on_choose();
}

void LogWidget::set_current_edit_key(uint16_t test)
{
    QMutexLocker lock(&mutex);
    if(cur_test != test) {
        cur_test = test;
        cur_lines = 0;
        edit->clear();
        label->setText(ChooseDialog::test2name(cur_test));
        refresh();
    }
}

void LogWidget::add_log(uint16_t test, QString text)
{
    QMutexLocker lock(&mutex);
    hash[test].append(text);
    if(test == cur_test) {
        refresh();
    }
}

void LogWidget::refresh()
{
    if(!hash.contains(cur_test)) {
        return;
    }
    const QStringList list = hash.value(cur_test);
    int old_line = cur_lines;
    for(int i = old_line; i < list.size(); i++) {
        edit->append(list.at(i));
        cur_lines++;
    }
}
