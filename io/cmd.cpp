#include "cmd.h"

CMD::CMD(QObject *parent) : QObject(parent)
{

}

CMD::CMD(const CMD &other)
{
    this->m_pri_prm = other.m_pri_prm;
    this->m_index = other.m_index;
    this->m_label = other.m_label;
    this->m_source = other.m_source;
    this->m_destination = other.m_destination;
    this->m_tag = other.m_tag;
    this->m_len = other.m_len;
    this->m_msg_payload = other.m_msg_payload;
}

CMD &CMD::operator=(const CMD &other)
{
    this->m_pri_prm = other.m_pri_prm;
    this->m_index = other.m_index;
    this->m_label = other.m_label;
    this->m_source = other.m_source;
    this->m_destination = other.m_destination;
    this->m_tag = other.m_tag;
    this->m_len = other.m_len;
    this->m_msg_payload = other.m_msg_payload;
    return *this;
}

static QByteArray set_length_buffer(int dataLen)
{
    QByteArray len;

    if (dataLen < 128) {
        len.append(static_cast<uint8_t>(dataLen));
    }
    else if (dataLen < 256) {
        len.append(static_cast<uint8_t>(0x81));
        len.append(static_cast<uint8_t>(dataLen));
    }
    else {
        len.append(static_cast<uint8_t>(0x82));
        len.append(static_cast<uint8_t>(dataLen >> 8));
        len.append(static_cast<uint8_t>(dataLen));
    }

    return len;
}

static int get_length_buffer(QByteArray data, int &dataLen)
{
    uint8_t bySize;
    int lenlen = 0;

    if (data[0] & 0x80) {
        bySize = data[0] & 0x7f;
        if (bySize == 1)
        {
            lenlen = 2;
            dataLen = data[1];
        }
        else if (bySize == 2)
        {
            lenlen = 3;
            dataLen = data[1] * 256 + data[2];
        }
    }
    else {
        lenlen = 1;
        dataLen = data[0] & 0x7f;
    }

    return lenlen;
}

bool CMD::decode(QByteArray data)
{
    if(data.size() < 13) {
        return false;
    }
    int index = 0;
    m_pri_prm = data[index++];
    m_index = data[index] * 256 + data[index + 1]; index+=2;
    m_label = data[index] * 256 + data[index + 1]; index+=2;
    int tmpIndex = data.indexOf('\0', index);
    if(tmpIndex == -1) {
        return false;
    }
    m_source = data.mid(index, tmpIndex-index+1); index = tmpIndex + 1;
    tmpIndex = data.indexOf('\0', index);
    if(tmpIndex == -1) {
        return false;
    }
    m_destination = data.mid(index, tmpIndex-index+1); index = tmpIndex + 1;
    if(data.size() < index + 4) {
        return false;
    }
    m_tag = (data[index+3] << 24) | (data[index+2] << 16) | (data[index+1] << 8) | data[index];
    index += 4;
    int datalen;
    int lenlen = get_length_buffer(data.mid(index), datalen);
    m_len = data.mid(index, lenlen); index+= lenlen;
    if(index + datalen != data.size()) {
        return false;
    }
    m_msg_payload = data.mid(index, datalen);
    return true;
}

QByteArray CMD::encode()
{
    m_len = set_length_buffer(m_msg_payload.size());
    QByteArray hex;
    hex.append(m_pri_prm);
    hex.append(m_index >> 8);
    hex.append(m_index);
    hex.append(m_label >> 8);
    hex.append(m_label);
    hex.append(m_source);
    hex.append(m_destination);
    hex.append(m_tag);
    hex.append(m_tag>>8);
    hex.append(m_tag>>16);
    hex.append(m_tag>>24);
    hex.append(m_len);
    hex.append(m_msg_payload);
    return hex;
}

uint16_t CMD::iid()
{
    return static_cast<uint16_t>(m_tag >> 16);
}

uint16_t CMD::iop()
{
    return static_cast<uint16_t>(m_tag);
}
