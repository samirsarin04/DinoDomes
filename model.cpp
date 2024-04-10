#include "model.h"
#include "digscene.h"
#include "keystroke.h"
#include "museumscene.h"
#include "searchscene.h"

#include <QDebug>

Model::Model(QObject *parent)
    : QObject{parent}
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

    currentScene = search;

    connect(&timer, &QTimer::timeout, this, &Model::newFrameTick);

    timer.setInterval(34);
    timer.start();
}

void Model::handleKeyPress(KeyStroke key)
{
    Scene beforeScene = currentScene;
    //Each scene handles key inputs as they need to, even changing the currentScene if neccessary
    switch (currentScene) {
    case dig:
        digScene.keyPress(key);
        break;
    case search:
        searchScene.keyPress(key);
        break;
    case museum:
        museumScene.keyPress(key);
        break;
    default:
        return;
    }
    // Sets state for new scene if scene detection set...maybe not neccessary?
    // I think (hope) this won't be neccessary
    detectSceneChange(beforeScene);
}

void Model::detectSceneChange(Scene beforeScene)
{
    if (beforeScene == currentScene) {
        //qDebug() << "No scene change";
        return;
    }
    switch (currentScene) {
    case dig:
        //digScene.activate();
        break;
    case search:
        //searchScene.activate();
        break;
    case museum:
        //museumScene.activate();
        break;
    default:
        return;
    }
}

// EVERY SCENE SHARES A SINGLE FRAME WHICHEVER ONE IS ACTIVE IS THE ONE UPDATING / BROADCASTING THE QIMAGE
// THIS IS ACTIVELY TICKING AT 30FPS
void Model::newFrameTick()
{
    // Queue of enums for sound effects that get sent to view to be played
    // while (!player.soundEffects.empty()){
    //  emit sendsoundEffect(player.soundEffects.pop());
    // }

    lock.lock();
    currentFrame = currentScene.buildScene();
    lock.unlock();

    emit sendFrameToView(currentFrame);
}
