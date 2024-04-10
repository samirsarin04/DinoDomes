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
    /// @brief Builds the current image to be shown on the screen
    /// @return The current scene as a QImage
    virtual QImage buildScene();

signals:
};

#endif // SCENE_H
