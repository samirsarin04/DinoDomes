#ifndef SEARCHSCENE_H
#define SEARCHSCENE_H
#include "digscene.h"
#include "museumscene.h"
#include <Box2D/Box2D.h>
#include <QDebug>
#include "scene.h"
//#include "keystroke.h"

///@brief the header file for the search scene of DinoDomes
/// reviewed by: Peyton Jensen
class SearchScene : public Scene
{
public:
    ///@brief constructs a search scene object
    ///@param player the PlayerState to be included in the scene
    ///@param currentScene the Scene object of the current scene
    explicit SearchScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);

    ~SearchScene();

    ///@brief initializes and builds the scene
    virtual QPixmap buildScene();

    ///@brief initializes the pointers for the scene
    ///@param digScene the DigScene associated with the current game
    ///@param musuemScene the MuseumScene associated with the current game
    void initializePointers(DigScene &digScene, MuseumScene &museumScene);

    ///@brief game loop that updates the world each frame
    void updateWorld();

protected:
    ///@brief activates the scene when switching to it
    virtual void activate();

    ///@brief deactivates the scene when switching from it
    virtual void deactivate();

    ///@brief handles and processes player key inputs
    virtual void processPlayerInput();

    ///@brief sets up the box2d world for the game
    void setupBox2D();


    //b2PolygonShape charBox;
    // b2PolygonShape foregrnd1Box;
    // b2PolygonShape foregrnd2Box;

    // b2FixtureDef foregrnd1Fixture;
    // b2FixtureDef foregrnd2Fixture;

private:
    b2Body* startBody;
    b2Body* groundBody;
    b2World* world;
    DigScene* digPtr;
    MuseumScene* museumPtr;

    QPixmap dinoDomes;
    QPixmap startBackdrop;
    QPixmap background;

    QPixmap foreground;
    QPixmap otherForeground;
    QPixmap cactus;
    QPixmap currentCharacter;

    QPixmap rightIdleCharacter;
    QPixmap rightStep1Character;
    QPixmap rightStep2Character;

    QPixmap leftIdleCharacter;
    QPixmap leftStep1Character;
    QPixmap leftStep2Character;

    QPixmap digImage;

    QPixmap museum;

    DinosaurName currentDinosaur;
    DinosaurBone currentBone;

    //int cactusX = -9999;
    int stepCounter;
    int spriteMovementIndex = 0;
    int movementFrameCounter = 0;
    int foregroundX = 0;
    int otherForegroundX = 1080;
    int museumX = 0;

    int digLocationX = 0;


    enum moving{right, left, idleRight, idleLeft};
    moving direction;
    moving prevDirection;

    bool isMoving;

    bool digSpot;

    bool bonePassed;

    bool digSoundPlayed;

    bool startPressed = false;
    bool startAllowed = false;

    //REMOVE - ONLY FOR TESTING
    void printDinosaur();

    void moveRight();
    void moveLeft();
    void updatePlayerMovement();
    void updateForeground();
    void checkDigCollision();
    void checkMuseumCollision();
    void drawUI();

    void drawStartScreen();

    void spawnBone();

    bool isBodyStill(b2Body* body, float threshold);

};

#endif // SEARCHSCENE_H
