#ifndef HELPER_H
#define HELPER_H

#include <QObject>

class Helper : public QObject
{
    Q_OBJECT
public:
    static QString B2S(const QByteArray &hex);
    static QString B2DS(const QByteArray &hex);
    static QByteArray S2B(const QString& str);
};

#endif // HELPER_H
