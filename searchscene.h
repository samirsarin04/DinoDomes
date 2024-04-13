#ifndef SEARCHSCENE_H
#define SEARCHSCENE_H

#include "scene.h"
#include "keystroke.h"
//#include "digscene.h"
//#include "museumscene.h"

//class MuseumScene;

class SearchScene : public Scene
{
public:
    explicit SearchScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);
    virtual QPixmap buildScene();
    void initializePointers(DigScene &digScene, MuseumScene &museumScene);
    void updateWorld();

private:
    DigScene* digPtr;
    MuseumScene* museumPtr;

    QPixmap background;
    QPixmap foreground;

    //REMOVE - ONLY FOR TESTING
    void printDinosaur();

};

#endif // SEARCHSCENE_H
