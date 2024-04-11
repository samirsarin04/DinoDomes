#include "model.h"
#include "digscene.h"
#include "keystroke.h"
#include "museumscene.h"
#include "searchscene.h"

#include <QDebug>

Model::Model(QObject *parent)
    : QObject{parent}, player(), digScene(player, &currentScene), museumScene(player, &currentScene), searchScene(player, &currentScene)
{
    // PASS STATE VARIABLES BY REFERENCE INTO CONSTRUCTORS FROM MODEL SO THEY ARE SHARED BETWEEN ALL CLASSES
    // DigScene digScene(currentDinosaur, bonesArray, lock, currentScene, currentFrame etc...);
    //MuseumScene museumScene(currentDinosaur, bonesArray, lock, currentScene, currentFrame etc...);
    //SearchScene searchScene(currentDinosaur, bonesArray, lock, currentScene, currentFrame etc...);


    // PlayerState player;
    // DigScene digScene(player);
    // MuseumScene museumScene(player);
    // SearchScene searchScene(player);

    //TO COME UP WITH ELEGANT WAY TO DRAW IMAGES ONTO ANOTHER IMAGE / STORE THEM
    //MAY NEED A HELPER/FRAME CLASS TBD

    //TODO COME UP WITH STATE VARIABLES THAT BELONG TO MODEL AND NEED TO BE SHARED AMONG THE DIFFERENT CLASSES

    // locks passed by reference into classes => used whenever image generation occurs / blasting frames occurs on separate thread
    // EXAMPLE OF HOW THE IMAGE GENERATION *MIGHT* look
    // lock.lock();
    // currentImage = localImage.copy();
    // lock.unlock();

    currentScene = &searchScene;
    //currentScene = &museumScene;

    searchScene.initializePointers(digScene, museumScene);

    museumScene.initializePointers(searchScene);

    digScene.initializePointers(searchScene, museumScene);

    connect(&timer, &QTimer::timeout, this, &Model::newFrameTick);

    timer.setInterval(34);
    timer.start();
}

Model::~Model() {
    // this was causing a crash message, dont want to delete a stack allocated pointer
    //delete currentScene;
}

void Model::handleKeyPress(KeyStroke key)
{
    //Sets the current input in the player object, scenes are free to modify this value as they see fit
    player.setInput(key);
    //currentScene->keyPress(key);
}


// EVERY SCENE BLASTS A SINGLE FRAME - WHICHEVER ONE IS ACTIVE
// THIS IS ACTIVELY TICKING AT 30FPS
void Model::newFrameTick()
{
    // Queue of enums for sound effects that get sent to view to be played
    // while (!player.soundEffects.empty()){
    //  emit sendsoundEffect(player.soundEffects.pop());
    // }
    player.lock.lock();
    currentScene->buildScene();
    player.lock.unlock();
    //emit sendFrameToView(currentFrame);
}
