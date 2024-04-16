#ifndef SCENE_H
#define SCENE_H

#include "keystroke.h"
#include "playerstate.h"
#include "dinosaurbone.h"
#include "dinosaurname.h"
#include <QPixmap>
#include <Box2D/Box2D.h>
#include <QPainter>

#include <QDebug>

// Forward declaration of classes to avoid recursive includes with pointers
class DigScene;
class SearchScene;
class MuseumScene;

class Scene : public QObject
{
    Q_OBJECT
public:
    explicit Scene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);
    /// @brief Builds the current image to be shown on the screen
    /// @return The current scene as a QImage
    virtual QPixmap buildScene();

protected:
    PlayerState* player;
    b2World world;

    QPixmap frame;
    QPainter painter;

    Scene** currentScene;

    float32 timeStep;

    bool activated;

    virtual void activate();
    virtual void deactivate();
    virtual void processPlayerInput();
    virtual void setupBox2D();

signals:
};

#endif // SCENE_H
