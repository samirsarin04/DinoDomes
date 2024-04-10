#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <QObject>
#include <QQueue>

class PlayerState : public QObject
{
    Q_OBJECT
public:
    explicit PlayerState(QObject *parent = nullptr);
    QQueue<int> soundEffects;
private:


signals:
};

#endif // PLAYERSTATE_H
