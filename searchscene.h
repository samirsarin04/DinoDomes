#ifndef SEARCHSCENE_H
#define SEARCHSCENE_H

#include "scene.h"
#include "keystroke.h"
#include <set>
//#include "digscene.h"
//#include "museumscene.h"


///@brief the header file for the search scene of DinoDomes
/// reviewed by: Peyton Jensen
class SearchScene : public Scene
{
public:
    ///@brief constructs a search scene object
    ///@param player the PlayerState to be included in the scene
    ///@param currentScene the Scene object of the current scene
    explicit SearchScene(PlayerState& player, Scene** currentScene, QObject *parent = nullptr);

    ///@brief deconstructs a search scene object
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

    ///@brief sets up the box2d world for the game to be used in start screen,
    ///creates fixtures and bodies on ground and "DinoDomes" logo to create bouncing effect
    virtual void setupBox2D();
private:
    ///@brief the body for the startscreen "DinoDomes" art
    b2Body* startBody;

    ///@brief the body for the startscreen art bounce location
    b2Body* groundBody;

    ///@brief the box2d world
    b2World* world;

    ///@brief pointer to the current DigScene
    DigScene* digPtr;

    ///@brief pointer to the current MuseumScene
    MuseumScene* museumPtr;

    ///@brief "DinoDomes" startscreen art pixmap
    QPixmap dinoDomes;

    ///@brief startscreen wooden backdrop pixmap
    QPixmap startBackdrop;

    ///@brief game background pixmap
    QPixmap background;

    ///@brief main game foreground pixmap
    QPixmap foreground;

    ///@brief other game foreground pixmap, shifted depending on the way the player is moving to create sidescrolling
    QPixmap otherForeground;

    ///@brief game foreground cacti pixmap
    QPixmap cactus;

    ///@brief the pixmap of the current character set depending on animation step
    QPixmap currentCharacter;

    ///@brief pixmap of character when in "idleRight" position
    QPixmap rightIdleCharacter;

    ///@brief pixmap of first step made in right direction
    QPixmap rightStep1Character;

    ///@brief pixmap of second step made in right direction
    QPixmap rightStep2Character;

    ///@brief pixmap of character when in "idleLeft" position
    QPixmap leftIdleCharacter;

    ///@brief pixmap of first step made in left direction
    QPixmap leftStep1Character;

    ///@brief pixmap of second step made in left direction
    QPixmap leftStep2Character;

    ///@brief dig pixmap used to indicate where a dig site is
    QPixmap digImage;

    ///@brief dino museum pixmap
    QPixmap museum;

    ///@brief the question mark image displayed on the found bones ui before finding a bone
    QPixmap questionMark;

    ///@brief the name of the current Dinosaur to be searched for
    DinosaurName currentDinosaur;

    ///@brief the current bone to be searched for
    DinosaurBone currentBone;

    ///@brief step counter used to calculate which character image to draw
    int stepCounter = 0;

    ///@brief movement index used to calculate which character image to draw
    int spriteMovementIndex = 0;

    ///@brief frame counter ensuring character moves for 15 frames per input
    int movementFrameCounter = 0;

    ///@brief the X coord of the foreground image
    int foregroundX = 0;

    ///@brief the X coord of the other foreground image
    int otherForegroundX = 1080;

    ///@brief the X coord of the museum, used to allow player to walk into museum
    int museumX = 0;

    ///@brief the X coord of the next DigLocation
    int digLocationX = 0;

    ///@brief used to determine which character image to draw
    enum moving{right, left, idleRight, idleLeft};

    ///@brief the current moving direction of the player
    moving direction;

    ///@brief the last moving direction of the player
    moving prevDirection;

    ///@brief flag indicating if the player is moving
    bool isMoving;

    ///@brief flag indicating if the player is colliding with a dig spot
    bool digSpot;

    ///@brief flag indicating if the current dig spot has been passed
    bool bonePassed;

    ///@brief flag indicating if the dig sound has been played when brushing
    bool digSoundPlayed;

    ///@brief flag indicating if the start key has been pressed
    bool startPressed = false;

    ///@brief flag indicating if the start key is allowed to be pressed yet
    bool startAllowed = false;

    //REMOVE - ONLY FOR TESTING
    void printDinosaur();

    ///@brief moves the player right by switching direction to right
    void moveRight();

    ///@brief moves the player left by switching direction to left
    void moveLeft();

    ///@brief updates the player's movement variables
    void updatePlayerMovement();

    ///@brief updates the foreground(s) to create sidescrolling effect
    void updateForeground();

    ///@brief checks if the player is colliding with a dig site
    void checkDigCollision();

    ///@brief checks if the player is colliding with the museum
    void checkMuseumCollision();

    ///@brief draws the UI that shows bone progress for current dino
    void drawUI();

    ///@brief draws the box2d startscreen logo bounce effect
    void drawStartScreen();

    ///@brief spawns the current bone for the player to collect
    void spawnBone();

    ///@brief checks if a box2d body is within a certain movement threshold, used to delay
    /// "press F to start" string at the beginning of the game
    /// @param body the body to be checked against
    /// @param threshold the movement threshold
    bool isBodyStill(b2Body* body, float threshold);
};

#endif // SEARCHSCENE_H
