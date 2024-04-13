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
    QPixmap otherForeground;


    int foregroundX = 0;
    int otherForegroundX = 1080;

    enum moving{right, left};
    moving direction;

    //REMOVE - ONLY FOR TESTING
    void printDinosaur();

    void moveRight();
    void moveLeft();

};

#endif // SEARCHSCENE_H
