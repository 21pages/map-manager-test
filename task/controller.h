#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "client.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(Client *client, QObject *parent = nullptr);

};

#endif // CONTROLLER_H
