#ifndef MUSEUMSCENE_H
#define MUSEUMSCENE_H

#include "keystroke.h"

class MuseumScene
{
public:
    MuseumScene();
    void activate();
    void deactivate();
    void keyPress(KeyStroke key);
};

#endif // MUSEUMSCENE_H
