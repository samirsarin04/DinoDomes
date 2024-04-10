#ifndef DIGSCENE_H
#define DIGSCENE_H

#include "scene.h"

class DigScene : public Scene
{
public:
    explicit DigScene(PlayerState& player, QObject *parent = nullptr);
};

#endif // DIGSCENE_H
