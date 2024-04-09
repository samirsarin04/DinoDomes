#ifndef SEARCHSCENE_H
#define SEARCHSCENE_H

#include "keystroke.h"

class SearchScene
{
public:
    SearchScene();
    void activate();
    void deactivate();
    void keyPress(KeyStroke key);
};

#endif // SEARCHSCENE_H
