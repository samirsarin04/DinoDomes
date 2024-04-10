#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <QObject>

class PlayerState : public QObject
{
    Q_OBJECT
public:
    explicit PlayerState(QObject *parent = nullptr);

signals:
};

#endif // PLAYERSTATE_H
