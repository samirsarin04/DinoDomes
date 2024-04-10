#ifndef SEARCHSCENE_H
#define SEARCHSCENE_H

#include "scene.h"
#include "keystroke.h"

class SearchScene : public Scene
{
public:
    explicit SearchScene(PlayerState& player, QObject *parent = nullptr);
    virtual void keyPress(KeyStroke key);

};

#endif // SEARCHSCENE_H
