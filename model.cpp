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

    currentScene = &searchScene;
    //currentScene = &museumScene;

    // Lets each scene know about the other scenes they might need to link to
    searchScene.initializePointers(digScene, museumScene);
    museumScene.initializePointers(searchScene);
    digScene.initializePointers(searchScene, museumScene);

    connect(&timer, &QTimer::timeout, this, &Model::newFrameTick);

    //timer.setInterval(34);
    timer.setInterval(17);
    timer.start();
}

Model::~Model() {
}

void Model::handleKeyPress(KeyStroke key)
{
    //Sets the current input in the player object, scenes are free to modify this value as they see fit
    switch(key){
    case KeyStroke::test:   //TODO remove for final submission
        qDebug() << "Switching to dig scene";
        currentScene = &digScene;
        return;
    default: break;
    }

    player.setInput(key);
}


// EVERY SCENE BLASTS A SINGLE FRAME - WHICHEVER ONE IS ACTIVE
// THIS IS ACTIVELY TICKING AT 30FPS
void Model::newFrameTick()
{
    player.lock.lock();
    emit sendFrameToView(currentScene->buildScene());
    player.lock.unlock();

    //Queue of enums for sound effects that get sent to view to be played
    while (!player.soundEffects.empty()){
        // qDebug() << "sending effect";
        emit sendSoundEffect(player.soundEffects.dequeue());
    }
}
