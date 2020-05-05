#ifndef HELPER_H
#define HELPER_H

#include <QObject>
#include <QTreeWidgetItem>

class Helper : public QObject
{
    Q_OBJECT
public:
    static QString B2S(const QByteArray &hex);
    static QString B2DS(const QByteArray &hex);
    static QByteArray S2B(const QString& str);
    static void recursivelyDelTreeItem(QTreeWidgetItem *item);

};

#endif // HELPER_H
