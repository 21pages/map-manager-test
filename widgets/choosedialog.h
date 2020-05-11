#ifndef CHOOSEDIALOG_H
#define CHOOSEDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QTreeWidgetItem>


class ChooseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseDialog(QWidget *parent = nullptr);
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
    static QVector<uint16_t> s_entries;
    static QStringList s_function;
    static QStringList s_performance;
    static QStringList s_compatibility;
    static QStringList s_interface;
private:
    void initView();
    void on_ok();
    void onItemClicked(QTreeWidgetItem *item, int column);
signals:
    void sig_choose();
private:
    QTreeWidget *tree;
};

#endif // CHOOSEDIALOG_H
