#ifndef SCENE_H
#define SCENE_H

#include "keystroke.h"
#include "playerstate.h"
#include <Qimage>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(PlayerState& player, QObject *parent = nullptr);
    virtual void keyPress(KeyStroke key);
    /// @brief Builds the current image to be shown on the screen
    /// @return The current scene as a QImage
    virtual QImage buildScene();

protected:
    PlayerState& player;

signals:
};

#endif // SCENE_H
