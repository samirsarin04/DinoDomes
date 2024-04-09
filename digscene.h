#ifndef DIGSCENE_H
#define DIGSCENE_H

#include "keystroke.h"

class DigScene
{
public:
    DigScene();
    void activate();
    void deactivate();
    void keyPress(KeyStroke key);
};

#endif // DIGSCENE_H
