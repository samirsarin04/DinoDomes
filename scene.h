#ifndef SCENE_H
#define SCENE_H

#include "keystroke.h"
#include "playerstate.h"
#include <QPixmap>
#include <Box2D/Box2D.h>

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(PlayerState& player, QObject *parent = nullptr);
    virtual void keyPress(KeyStroke key);
    /// @brief Builds the current image to be shown on the screen
    /// @return The current scene as a QImage
    virtual QPixmap buildScene();

private:
    PlayerState* player;
    b2World world;
    b2Body* body;

    void setupBox2D();

signals:
};

#endif // SCENE_H
