#include "helper.h"

QString Helper::B2S(const QByteArray &hex)
{
    return QString::fromUtf8(hex.toHex().toUpper());
}

QString Helper::B2DS(const QByteArray& hex)
{
    QString str=QString::fromUtf8(hex.toHex().toUpper());
    QString str1;
    for(int i=0;i<str.size();++i)
    {
        str1.append(str[i]);
        if(i%2==1 && i!=str.size()-1)
        {
            str1.append(" ");
        }
    }
    return str1;
}

QByteArray Helper::S2B(const QString &str)
{
    QString str2 = str;
    QString str3 = str2.replace(" ", "").trimmed();
    return QByteArray::fromHex(str.toLatin1());
}
