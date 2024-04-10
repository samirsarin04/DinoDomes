#ifndef SCENE_H
#define SCENE_H

#include "keystroke.h"
#include <QObject>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    virtual void keyPress(KeyStroke key);

signals:
};

#endif // SCENE_H
