#include "model.h"
#include "digscene.h"
#include "keystroke.h"
#include "museumscene.h"
#include "searchscene.h"

//Samir
Model::Model(QObject *parent)
    : QObject{parent}, player(), digScene(player, &currentScene), museumScene(player, &currentScene), searchScene(player, &currentScene)
{
    currentScene = &searchScene;

    // Lets each scene know about the other scenes they might need to link to
    searchScene.initializePointers(digScene, museumScene);
    museumScene.initializePointers(searchScene);
    digScene.initializePointers(searchScene, museumScene);

    connect(&timer, &QTimer::timeout, this, &Model::newFrameTick);

    timer.setInterval(17);
    timer.start();
}

Model::~Model() {
}

void Model::handleKeyPress(KeyStroke key)
{
    //Sets the current input in the player object, scenes are free to modify this value as they see fit
    player.setInput(key);
}

void Model::newFrameTick()
{
    // THIS IS ACTIVELY TICKING AT 60 FPS
    player.lock.lock();
    emit sendFrameToView(currentScene->buildScene());

    //Queue of enums for sound effects that get sent to view to be played
    while (!player.soundEffects.empty()){
        emit sendSoundEffect(player.soundEffects.dequeue());
    }
    player.lock.unlock();
}
