#ifndef CMD_H
#define CMD_H

#include <QObject>

class CMD : public QObject
{
    Q_OBJECT
public:
    CMD(QObject *parent = nullptr);
    CMD(const CMD& other);
    CMD& operator=(const CMD& other);
public:
    bool decode(QByteArray data);
    QByteArray encode();
    uint16_t iid();
    uint16_t iop();
public:
    uint8_t m_pri_prm;
    uint16_t m_index;
    uint16_t m_label;
    QByteArray m_source;
    QByteArray m_destination;
    uint32_t m_tag;
    QByteArray m_len;
    QByteArray m_msg_payload;

};

#endif // PROTOCOL_H
