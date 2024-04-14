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

protected:
    virtual void activate();
    virtual void deactivate();
    virtual void processPlayerInput();


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

    QPixmap digImage;

    DinosaurName currentDinosaur;
    DinosaurBone currentBone;

    int stepCounter = 0;
    int spriteMovementIndex = 0;
    int movementFrameCounter = 0;
    int foregroundX = 0;
    int otherForegroundX = 1080;

    int digLocationX = 0;


    enum moving{right, left, idleRight, idleLeft};
    moving direction;
    moving prevDirection;

    bool isMoving;

    bool digSpot;

    bool bonePassed;

    bool digSoundPlayed;

    //REMOVE - ONLY FOR TESTING
    void printDinosaur();

    void moveRight();
    void moveLeft();
    void updatePlayerMovement();
    void updateForeground();
    void checkDigCollision();

    void spawnBone();

};

#endif // SEARCHSCENE_H
