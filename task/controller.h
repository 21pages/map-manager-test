#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "map_def.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

public:
    static QVector<TMAP_TASK*> sTasks;

};

#endif // CONTROLLER_H
