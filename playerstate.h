#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include <QObject>
#include <QQueue>
#include "keystroke.h"
#include <mutex>

class PlayerState : public QObject
{
    Q_OBJECT
public:
    explicit PlayerState(QObject *parent = nullptr);
    QQueue<int> soundEffects;
    void setInput(KeyStroke key);
    KeyStroke getInput();
    std::mutex lock;

protected:
    std::mutex inputLock;
    KeyStroke currentInput;


signals:
};

#endif // PLAYERSTATE_H
