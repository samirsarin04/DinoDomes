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

    QPixmap currentCharacter;

    QPixmap rightIdleCharacter;
    QPixmap rightStep1Character;
    QPixmap rightStep2Character;

    QPixmap leftIdleCharacter;
    QPixmap leftStep1Character;
    QPixmap leftStep2Character;


    int stepCounter = 0;
    int spriteMovementIndex = 0;
    int movementFrameCounter = 0;
    int foregroundX = 0;
    int otherForegroundX = 1080;


    enum moving{right, left, idleRight, idleLeft};
    moving direction;
    moving prevDirection;

    bool isMoving;

    //REMOVE - ONLY FOR TESTING
    void printDinosaur();

    void moveRight();
    void moveLeft();
    void updatePlayerMovement();
    void updateForeground();

};

#endif // SEARCHSCENE_H
