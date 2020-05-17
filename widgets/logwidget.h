#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTextEdit>
#include <QLabel>

class LogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogWidget(QWidget *parent = nullptr);
    void on_choose();
    void on_start();
    void set_current_edit_key(uint16_t test);
    void add_log(uint16_t test, QString text);
private:
    void refresh();
private:
    QLabel *label;
    QTextEdit *edit;
    QHash<uint16_t, QStringList> hash;
    int cur_lines = 0;
    uint16_t cur_test = -1;

};

#endif // LOGWIDGET_H
